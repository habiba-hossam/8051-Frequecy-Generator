#include <reg51.h>
#include <stdio.h>
#include <stdlib.h>

sbit c3 =P1^7;
sbit c2 =P1^6;
sbit c1 =P1^5;
sbit c0 =P1^4;

sbit r3 =P1^3;
sbit r2 =P1^2;
sbit r1 =P1^1;
sbit r0 =P1^0;
 
sbit s3 =P0^3;
sbit s2 =P0^2;
sbit s1 =P0^1;
sbit s0 =P0^0;

sbit fout=P3^7;
sbit sw=P3^2;
void seg(unsigned int);
unsigned char digit[] ={0,0,0,0};
unsigned char chr;
unsigned char display [] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void debounce(i)
{
		 int j = 255;
		 for(;i>0;i--)
		 for(;j>0;j--)
				;		 
}	
void debounce1()
{ 
		unsigned char i = 0;
	  while(	sw == 0 );		
		for( i = 255; i > 0; i--)
			if ( sw == 0 )
					i = 255;	
}
unsigned char i = 0;

#define KEYPAD_NO_NEW_DATA '-'
static char Last_valid_Key_G = KEYPAD_NO_NEW_DATA;
char pKey;

char KeyPad_Scan()
{
	static char Old_Key;
	char Key = KEYPAD_NO_NEW_DATA;
	
	r0=0;
  if(c3 ==0) {Key= 'A';}
	else if(c2==0) {Key= '3'; chr=3;}
	else if(c1==0) {Key= '2'; chr=2;}
	else if(c0==0) {Key= '1'; chr=1;}
	r0=1;
	
	r1=0;
  if(c3 ==0) {Key= 'B';}
	else if(c2==0) {Key= '6'; chr=6;}
	else if(c1==0) {Key= '5'; chr=5;}
	else if(c0==0) {Key= '4'; chr=4;}
	r1=1;
	
	r2=0;
  if(c3 ==0) {Key= 'C';}
	else if(c2==0) {Key= '9'; chr=9;}
	else if(c1==0) {Key= '8'; chr=8;}
	else if(c0==0) {Key= '7'; chr=7;}
	r2=1;
	
	r3=0;
  if(c3 ==0) {Key= 'D';}
	else if(c2==0) {Key= '#';}
	else if(c1==0) {Key= '0'; chr=0;}
	else if(c0==0) {Key= '*';}
	
	r3=1;
	debounce(40);
	
	if(Key == KEYPAD_NO_NEW_DATA)
	{
    Old_Key = KEYPAD_NO_NEW_DATA;
		Last_valid_Key_G = KEYPAD_NO_NEW_DATA;
		return 0;
	}
  
	if(Key == Old_Key)
	{
    if(Key !=Last_valid_Key_G)
		{
			pKey=Key;
			Last_valid_Key_G = Key;
			return 1;
		}
	}
	
	
  Old_Key = Key;
  return 0;	
}

bit result;

void timer0_isr (void) interrupt 1
{
	TH0=0xE0;
	TL0=0x00;
	result = KeyPad_Scan();
	if(result == 1)
			{
				seg(chr);		
			}
			
	if (i==0){
			s0=0;
			s1=1;
			s2=1;
			s3=1;
			P2=display[digit[0]];
	}
	else if (i==1){
		s0=1;
		s1=0;
		s2=1;
		s3=1;
		P2=display[digit[1]];
		}
	else if (i==2){
	s0=1;
	s1=1;
	s2=0;
	s3=1;
	P2=display[digit[2]];
	}
	else if (i==3)
	{	
	s0=1;
	s1=1;
	s2=1;
	s3=0;	
	P2=display[digit[3]];
	}
	i++;
	if (i>3) i=0;
	
}
unsigned int freq ;
float time;
unsigned int t;
int low,high;
unsigned char l,m;
void freqGenerator(void){
	freq=digit[0]+digit[1]*10+digit[2]*100+digit[3]*1000;
	//freq=2000;
	time=1.0/(freq*2);
	if(time>0.0711065){
		time=time/20.0;
	  l=20;	
	}
	else{
		l=1;
	}
	t=(time/(0.000001085))-9;
	t=65536-t;
  low=t&0xff;
  high=(t>>8)&0xff;
	m=l;
}

void EXT_INT0 (void) interrupt 0
{ 
	debounce1();
	EA=0;
	freqGenerator();
	TL1=low;
  TH1=high;
	ET1=1;
	TR1=1;
	EA=1;	
}
void timer1_isr (void) interrupt 3
{ 
	TL1=low;
  TH1=high;
	if(m==0)
	{
		m=l;
	}
	if(m==l){
		fout=~fout;
		m--;
	  }
  else
	{
		fout=fout;
    m--;
	}	
}

void main()
{
	P1= 0xF0;
	r0 = 1;
	r1 = 1;
	r2 = 1;
	r3 = 1;
	P2=0x00;
	P0=0x00;
	TMOD = (TMOD & 0xF0) | 0x11; //0001 0001
	
	TH1=high;
	TL1=low;
	fout=1;
	IT0=1;
	ET0=1;
	TH0=0xE0;
	TL0=0x00;
	EX0=1;
	TR0=1;
	EA=1;
	while(1)
	{  
		PT1=1;
	}		
}



void seg(unsigned char ch)
{
	if (digit[3] == 0){
		digit[3]=digit[2];
   	digit[2]=digit[1];
		digit[1]=digit[0];
		digit[0]=ch;	 
	}
else {
	  digit[3]=0;
   	digit[2]=0;
		digit[1]=0;
		digit[0]=ch;	 
	
}

}