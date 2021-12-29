

#include	<reg52.h>
#include	<delay.h>
#include	<1602.h>
#include	<DHT11.h>
#include	<shot.h>
#include	<uart.h>
#include <24c02.h>
//#include <string.h>
#include <intrins.h>

#define MAX_R 50




void SendByte(char dat);
void SendStr(char *s);


//led��Ϊp1.0
//������ΪP1.1
//���ΪP1.2
//DHT11ΪP3.6
//ģʽ����Ϊp3.2
//��ʷ��¼���� p3.3��
//�洢����ΪSCL p2.0��SDA p2.1


sbit led=P1^0;
sbit dianji=P1^2;
sbit key=P3^2;		//����ģʽ����
sbit key1=P3^3;		//�鿴��һ����¼		�ⲿ�ж�1
sbit key3=P1^4;		
//sbit test_led=P1^7; 		//���Ե�




extern char rec_dat[9];   	//ȫ�ֱ�����������ʪ�ȵ��ַ���
char eep_dat[6];		//��ȡ�洢���Ļ�����

char eep_write[6];			//�洢�ַ���

unsigned int mode;		//ģʽ
unsigned int eep_w=0;		//�洢λ��
unsigned int eep_r=0;		//��ȡλ��
unsigned int time=0;  //ʱ��
unsigned int x=0;			//��¼��˫��
static int xnum=0;						//��¼��ʪ�ȴ���
	int i=0;
	static int num=0;				//��¼��ǰ�鿴����ʷ��¼λ�á�
	unsigned char eep_his[6];		//��ʷ��¼�鿴

void main()   
{   
	
	int RH;
	int compRH;
	
//	int j=0;
	LCD_Init();
	led=1;
	
	dianji=1;
  DHT11_delay_ms(100);    //DHT11
	
	//�ⲿ�жϳ�ʼ��
	EX0=1;IT0=1; 
	EX1=1;IT1=1;
	EA=1;
	TR1=1;
	
	//���ڳ�ʼ��
	PCON=0;
	SCON=0x50;//MOD 2 9600,can receive;
	ES=1;		//chuan kou Init
	TMOD=0x20;
	TL1=0xFD;
	TH1=0xFD;	//���ڳ�ʼ��
	
	
	
	while(1)   
	{  	
			switch(mode){
				case 0 :
					
					
					RH=(rec_dat[2]-'0')*10+(rec_dat[3]-'0');
					compRH=(eep_dat[0]-'0')*10+(eep_dat[1]-'0');
					mode=0;
					if(xnum==99){
							xnum=0;
							eep_w=0;
					}
					eep_write[0]=rec_dat[2];		//��ȡ��ʪ��
					eep_write[1]=rec_dat[3];
					eep_write[2]=rec_dat[7];
					eep_write[3]=rec_dat[8];	
					eep_write[4]=(xnum/10)+'0';		//��¼������ʮλ��
					eep_write[5]=(xnum%10)+'0';		//��λ��
					
					
					LCD_Write_Char(10,1,eep_his[0]);
					LCD_Write_Char(11,1,eep_his[1]);
					LCD_Write_Char(12,1,eep_his[2]);
					LCD_Write_Char(13,1,eep_his[3]);
					//��ʾ���һ����ʪ�ȼ�¼
				/*	LCD_Write_Char(10,1,eep_dat[0]);
					LCD_Write_Char(11,1,eep_dat[1]);	
					LCD_Write_Char(10,0,rec_dat[2]);
					LCD_Write_Char(11,0,rec_dat[3]);*/
					
		

					
					IRcvStr(0xae,eep_r*6,eep_dat,6);		//��ȡ���ݲ���ʾ
					for(i=0;i<4;i++){
						LCD_Write_Char(i,1,eep_dat[i]);
					}
					
								
					if(compRH!=RH)
						{
							ISendStr(0xae,eep_w*6,eep_write,6);  
							DelayMs(125);					//��ʱ125���룬�ȴ�д��������
							xnum++;				//����+1
							eep_w++;					//λ��+1
							eep_r=eep_w-1;//д���λ���Ѿ�+1��Ӧ-1���ڸ�ֵ����ȡ��λ��
							
					}
						
					LCD_Write_Char(7,1,(xnum%100)/10+'0');			//ʮλ��
					LCD_Write_Char(8,1,(xnum%10)+'0');			//��λ��
					
				
				
					
				
			
				
					SendStr(rec_dat);
					
					DHT11_receive();	
					
				//��ʾ��ǰ��ʪ��
					for(i=0;i<9;i++){
						LCD_Write_Char(i,0,rec_dat[i]);	
					}
					
					//�ж��Ƿ񳬳���ֵ
					if(RH>MAX_R){
						shout();
						led=~led;
					}
					else{
						dianji=1;
						led=1;
					}
				
					break;
					
					
					
				case 1:
					dianji=0;
					mode=1;
					RH=(rec_dat[2]-'0')*10+(rec_dat[3]-'0');
					DHT11_receive();
					//LCD_Write_String(0,0,rec_dat);
					for(i=0;i<9;i++){
						LCD_Write_Char(i,0,rec_dat[i]);	
					}
					LCD_Write_Char(0,1,mode+'0');
					if(RH>MAX_R){
						shout();
						led=~led;
					}
					else{
						led=1;
					}
					break;
			}
		
	}   
	
}   

void it0 (void) interrupt 0			//�ⲿ�ж�0
{
		DelayMs(10);
		mode=(mode+1)%2;
}




void it1 (void) interrupt 2			//�ⲿ�ж�1
{
	if(num==xnum){
			num==0;
	}
		num++;
		eep_r=eep_r-num-1;
		IRcvStr(0xae,eep_r*6,eep_his,6);
		eep_r=eep_r+num+1;
		
}


void ck (void	) interrupt 4				//�����ж�
{
	if(RI)
	{
		mode=SBUF;
		RI=0;
	}
}
