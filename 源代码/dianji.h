
#ifndef __dianji_H_
#define __dianji_H_


#include <reg52.h>
#include <delay.h>

sbit A1=P1^3; //���岽��������Ӷ˿�
sbit B1=P1^4;
sbit C1=P1^5;
sbit D1=P1^6;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//ȫ���ϵ�


void dianji();

#endif