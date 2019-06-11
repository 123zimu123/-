#ifndef __EEPROM_H_
#define  __EEPROM_H_
#include<intrins.h>
#define BYTE unsigned char 
#define  WORD unsigned int 
/*-------定义ISP/IAP/EEPROM 命令*/
#define  CMD_IDLE      0   //stand_BY
#define  CMD_READ      1   //读字节
#define  CMD_WRITE     2           //字节写
#define  CMD_ERASE     3        //擦除

/*----定义ISP/IAP/EEPROM 的IAP_CONTPTR寄存器操作吗--*/
//#define ENABLE_IAP 0X80  //晶振<30M
//#define ENABLE_IAP 0X81  //晶振<24M
#define ENABLE_IAP 0X82  //晶振<20M
//#define ENABLE_IAP 0X83  //晶振<12M
//#define ENABLE_IAP 0X84  //晶振<6M
//#define ENABLE_IAP 0X85  //晶振<3M
//#define ENABLE_IAP 0X86  //晶振<2M
//#define ENABLE_IAP 0X87  //晶振<1M

/*--------定义起始地址—————————————*/
#define IAP_ADDRESS                 0x03fe

/****************函数声明***********/
void Delay_EEPROM(BYTE n);          //延时函数
void IAPIDLE();       
BYTE IAPReadByte(WORD ADDR);//读字节
void IAPWriteByte(WORD addr,BYTE dat);//写入字节
void IAPErase(WORD addr);         //扇区擦除

void Delay_EEPROM(BYTE n)
{
      WORD x;
          while(n--)
          {
                     x=0;
                   while(++x);
          }
}
void IAPIDLE()
{
          IAP_CONTR=0;//  close IAP fuction
          IAP_CMD=0;  //  CLEAR COMMAND to standby
          IAP_TRIG=0; //Clear tigger regester
          IAP_ADDRH=0x80;//data ptr to nom_eeprom area
          IAP_ADDRL=0; //clear iap address to prevent misuse
}
BYTE IAPReadByte(WORD addr)          //读字节
{
          BYTE dat;
          IAP_CONTR=ENABLE_IAP;//open iap fuction
          IAP_CMD=CMD_READ;
          IAP_ADDRL=addr;
          IAP_ADDRH=addr>>8;
          IAP_TRIG=0X5A;                //每次必须要有这两句
          IAP_TRIG=0XA5;                //不然操作不成功
          _nop_();
          dat=IAP_DATA;
          IAPIDLE();
          return dat;
}
void IAPWriteByte(WORD addr,BYTE dat)
{
         IAP_CONTR=ENABLE_IAP;//open iap fuction
         IAP_CMD=CMD_WRITE;
          IAP_ADDRL=addr;
          IAP_ADDRH=addr>>8;
          IAP_DATA=dat;
          IAP_TRIG=0X5A;                //每次必须要有这两句
          IAP_TRIG=0XA5;                //不然操作不成功
           _nop_();
          IAPIDLE();
}
void IAPErase(WORD addr)         //扇区擦除
{
          IAP_CONTR=ENABLE_IAP;//open iap fuction
          IAP_CMD=CMD_ERASE;
          IAP_ADDRH=addr;
          IAP_ADDRH=addr>>8;
          IAP_TRIG=0X5A;                //每次必须要有
          IAP_TRIG=0XA5;                //不然操作不成功

          IAPIDLE();                    //闲置等待状态，不知道是不是这么翻译的，大概是这意思吧，英语水平不行。
}
#endif
