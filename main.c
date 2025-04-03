//De tai 9: dong ho+mach dem san pham
//Dinh Hieu
//13/2/2025
#include <REGX52.h>
#include <stdio.h>
//==================KHAI BAO====================
sbit SDA = P1^2; 
sbit SCL = P1^1; 
#define cambien P1_0

#define mode P1_3
#define tang P1_4
#define giam P1_5

sbit ledsp1 = P3^4;
sbit ledsp2 = P3^5;

sbit ledgio1 = P2^0;
sbit ledgio2 = P2^1;

sbit ledphut1 = P2^2;
sbit ledphut2 = P2^3;

sbit ledgiay1 = P2^4;
sbit ledgiay2 = P2^5;

sbit ledngay1 = P2^6;
sbit ledngay2 = P2^7;

sbit ledthang1 = P3^0;
sbit ledthang2 = P3^1;

sbit lednam1 = P3^2;
sbit lednam2 = P3^3;

#define DS1307_ID 0xD0
char so[] = {0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};
unsigned int dem=0;
unsigned int sanpham=0;
unsigned int chucphut,lephut,chucgiay,legiay,chucgio,legio,chucngay,lengay,chucthang,lethang,chucnam,lenam=0;
unsigned int giay, phut, gio, thu, ngay, thang, nam;
unsigned int chuc, donVi;
unsigned int i;
//=============================================

void delay(unsigned int time)
{	
	time=time*10;
	while(time--); 
}

	
//================GIAO TIEP I2C====================
void I2C_start(void)
	{
		SCL	= 1;	SCL = 0;
		SDA = 1;	SCL = 1;
		SDA = 0; 
		SCL = 0; 
	}

void I2C_stop(void)
	{
		SCL = 1;	SCL = 0;
		SDA = 0;	
		SCL = 1;	
		SDA = 1;	
	}

void I2C_write(unsigned char dat)
	{
		unsigned char i;	
		for (i=0;i<8;i++)
			{
				SDA = (dat & 0x80) ? 1:0;
				SCL=1;SCL=0;
				dat<<=1;
			}
		SCL = 1;	
		SCL = 0;
	}

unsigned char I2C_read(void)
	{
		bit rd_bit;	
		unsigned char i, dat;
		dat = 0x00;	
		for(i=0;i<8;i++)	 /* For loop read data 1 byte */
			{
				SCL = 1;	 /* Set SCL */
				rd_bit = SDA;	 /* Keep for check acknowledge	*/
				dat = dat<<1;	
				dat = dat | rd_bit;	/* Keep bit data in dat */
				SCL = 0;	 /* Clear SCL */
			}
		return dat;
	}

//----------------ds1307-----------------

unsigned char DS1307_Read(unsigned char addr) 
	{
		unsigned int temp,ret;	
		I2C_start(); /* Start i2c bus */
		I2C_write(DS1307_ID); /* Connect to DS1307 */
		I2C_write(addr);	 /* Request RAM address on DS1307 */	
		I2C_start();	 /* Start i2c bus */
		I2C_write(DS1307_ID+1);	/* Connect to DS1307 for Read */
		ret = I2C_read();	 /* Receive data */
		I2C_stop();
		//*********************************************
		temp = ret;	 /*BCD to HEX*/
		ret = (((ret/16)*10)+ (temp & 0x0f));	 /*for Led 7seg*/
		//*********************************************	
		return ret;	
	}

void Time_Read()
{
        
    giay = DS1307_Read(0x00);
		phut = DS1307_Read(0x01);
		gio  = DS1307_Read(0x02);
		thu  = DS1307_Read(0x03);
		ngay = DS1307_Read(0x04);
		thang= DS1307_Read(0x05);
		nam  = DS1307_Read(0x06);   
}

void DS1307_Write(unsigned char addr,unsigned char dat)
	{
		unsigned int temp;
		//**********************************************	 /*HEX to BCD*/
		temp = dat ;	 /*for Led 7seg*/
		dat = (((dat/10)*16)|(temp %10));
		//**********************************************	
		I2C_start(); /* Start i2c bus */
		I2C_write(DS1307_ID); /* Connect to DS1307 */
		I2C_write(addr);	 /* Request RAM address on DS1307 */	
		I2C_write(dat);	/* Connect to DS1307 for Read */
		I2C_stop();
	}

void Time_Write(unsigned char h, unsigned char p, unsigned char g, unsigned char ng, unsigned char t, unsigned char n)
	{
		DS1307_Write(0x02,h);
		DS1307_Write(0x01,p);
		DS1307_Write(0x00,g);
		DS1307_Write(0x04,ng);
		DS1307_Write(0x05,t);
		DS1307_Write(0x06,n-2000); 	

	}
//==================================================================================================
//====================HAM CON===========================
void hienthigio(unsigned int x){ 
			chucgio = x/10;
			legio = x%10;
	
			ledgio1 = 0;ledgio2 = 1;
			P0 = so[chucgio];
			delay(10);
	
			ledgio1 = 1;ledgio2 = 0;
			P0 = so[legio];
			delay(10);
			ledgio1 = 1;ledgio2 = 1;
}
void hienthiphut(unsigned int x){
			chucphut = x/10;
			lephut = x%10;
	
			ledphut1 = 0;
			P0 = so[chucphut];
		delay(10);
	
			ledphut1 = 1;
			ledphut2 = 0;
			P0 = so[lephut];
			delay(10);
			ledphut2 = 1;ledphut1 = 1;
}
void hienthigiay(unsigned int x){
			chucgiay = x/10;
			legiay = x%10;
	
			ledgiay1 = 0;
			P0 = so[chucgiay];
			delay(10);
	
			ledgiay1 = 1;
			ledgiay2 = 0;
			P0 = so[legiay];
		delay(10);
			ledgiay2 = 1;
}
void hienthingay(unsigned int x){
			chucngay = x/10;
			lengay = x%10;
	
			ledngay1 = 0;
			P0 = so[chucngay];
			delay(10);
	
			ledngay1 = 1;
			ledngay2 = 0;
			P0 = so[lengay];
			delay(10);
			ledngay2 = 1;
}
void hienthithang(unsigned int x){
			chucthang = x/10;
			lethang = x%10;
	
			ledthang1 = 0;
			P0 = so[chucthang];
			delay(10);
	
			ledthang1 = 1;
			ledthang2 = 0;
			P0 = so[lethang];
			delay(10);
			ledthang2 = 1;
}
void hienthinam(unsigned int x){
			chucnam = x/10;
			lenam = x%10;
	
			lednam1 = 0;
			P0 = so[chucnam];
			delay(10);
	
			lednam1 = 1;
			lednam2 = 0;
			P0 = so[lenam];
			delay(10);
			lednam2 = 1;
}
void Fix_time(void)	
{

	//Tang
	if(giay==60)	{giay=0;phut++;	}
	if(phut==60)	{phut=0;gio++;	}
	if(gio==24) gio=0;
	if(ngay==31) {ngay=0;thang++;}
	if(thang==13) {thang=0;nam++;}
	if(nam==100) nam=0;

	//Giam
	if(giay== -1) {giay=59;phut--;}
	if(phut== -1) {phut=59;gio-- ;}
	if(gio== -1) gio= 23;
	if(ngay==-1) {ngay=30;thang--;}
	if(thang==-1) {thang=12;nam--;}
	if(nam==-1) nam=99;
}

void Setup()	 // Kiem tra phim nhan.	
{ 
	if(mode==0)
	{
			dem++;
			if(dem==8) 
			{
			    dem=0;
				DS1307_Write(0x02,gio);
				DS1307_Write(0x01,phut);
				DS1307_Write(0x00,giay); 
				DS1307_Write(0x04,ngay); 
				DS1307_Write(0x05,thang); 
				DS1307_Write(0x06,nam); 
			}
	}
	while(!mode);

}

void Setup_gio()
{
	unsigned int i;
	if (tang==0){ gio++;delay(10000);}
	if (giam==0){ gio--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
			hienthigio(gio);
}

void Setup_phut()
{
	unsigned int i;
	if (tang==0){ phut++;delay(10000);}
	if (giam==0){ phut--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
			hienthiphut(phut);
}			

void Setup_giay()
{
	unsigned int i;
	if (tang==0){ giay++;delay(10000);}
	if (giam==0){ giay--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
		hienthigiay(giay);
}
void Setup_ngay()
{
	unsigned int i;
	if (tang==0){ ngay++;delay(10000);}
	if (giam==0){ ngay--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
		hienthingay(ngay);
}
void Setup_thang()
{
	unsigned int i;
	if (tang==0){ thang++;delay(10000);}
	if (giam==0){ thang--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
		hienthithang(thang);
}
void Setup_nam()
{
	unsigned int i;
	if (tang==0){ nam++;delay(10000);}
	if (giam==0){ nam--;delay(10000);}
	Fix_time();
	for(i=0;i<10;i++)
		hienthinam(nam);
}

void hienthisanpham(unsigned int x){
		  chuc = x / 10;
      donVi = x % 10;
	
      ledsp1 = 0;
      P0 = so[chuc];
			delay(10);
	
			ledsp1 = 1;
      ledsp2 = 0;
      P0 = so[donVi];
			delay(10);	
      ledsp2 = 1;
}
unsigned int trangthaicb = 1; 

void ktra_sp() {
    
    if (trangthaicb == 1 && cambien == 0) {
        sanpham++;
        if (sanpham == 100)
            sanpham = 0;
    }
    
    trangthaicb = cambien;
}


void Setup_sanpham()
{
	unsigned int i;
	if (tang==0){ sanpham++;delay(10000);}
	if (giam==0){ sanpham--;delay(10000);}
	ktra_sp();
	for(i=0;i<10;i++)
		hienthisanpham(sanpham);
}

//=================================================================
//=========================HAM MAIN================================

void main() {
		ledgiay1=ledgiay2=ledphut1=ledphut2=ledgio1=ledgio2=ledngay1=ledngay2=ledthang1=ledthang2=lednam1=lednam2=ledsp1=ledsp2=1;
    while (1) {
			hienthisanpham(sanpham);
			Setup();
			ktra_sp();
			if(dem==0){
			Time_Read();
			hienthigio(gio);
			hienthiphut(phut);
			hienthigiay(giay);
			hienthingay(ngay);
			hienthithang(thang);
			hienthinam(nam);
			
}
			else if (dem==1)
				Setup_gio();
			else if (dem==2)
				Setup_phut();
			else if (dem==3)
				Setup_giay();
			else if (dem==4)
				Setup_ngay();
			else if (dem==5)
				Setup_thang();
			else if (dem==6)
				Setup_nam();
			else if (dem==7)
				Setup_sanpham();
}
}




