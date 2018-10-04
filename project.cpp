#include<REG_MPC82G516.H>
//Global variables and definition
#define PWMPIN P04
typedef enum{
  FORWARD , BACKWARD , TURNLEFT , TURNRIGHT , STOP
}DIRECTION;
DIRECTION dir = STOP;
unsigned char pwm_width = 250;
bit pwm_flag = 0;
unsigned char speed = 250;
void set_direction();
void init_uart_timer2();
void setspeed(unsigned char in_speed);
sbit SPK=P3^4;
int tone_length = 3;
unsigned char current_tone;
void init_buzzer_timer1();
void init_pmw_timer0();
bit buzzer_switch = 0;
unsigned int code tone[]={	
	65536-1000000/(2*523), // DO  
	65536-1000000/(2*587), // Re
	65536-1000000/(2*659), // MI
	65536-1000000/(2*698), //Fa
	65536-1000000/(2*784), //So
	65536-1000000/(2*880), //La
	65536-1000000/(2*988), //Si
	65536-1000000/(2*554), // DO#  8 
	65536-1000000/(2*622), // Re#  9 
	65536-1000000/(2*740), // Fa#   10
	65536-1000000/(2*831), //So#   11
	65536-1000000/(2*932),  //La#   12
	65536-1000000/(2*1046),  // DO h  13
	65536-1000000/(2*1175),  //Re   14
	65536-1000000/(2*1318),  //Mi   15
	65536-1000000/(2*1397),  //Fa   16
	65536-1000000/(2*1568),  //Sol   17
	65536-1000000/(2*1760),  //La   18                                                                                                          +
	65536-1000000/(2*1976)};
unsigned char code beat1[]={3,3,3,3,3,3,3,3,5,3,3,3,5,3,3,3,5,3,3,3,3,3,3,3,3,3,5,3,3,3,5,3,3,3,5,0};
unsigned char code music_tone1[]={15,14,15,14,15,7,14,13,6,1,3,6,7,3,11,7,13,3,15,14,15,14,15,7,14,13,6,1,3,6,7,3,13,7,6};
unsigned char code beat2[]={ 1,1,3,1,1,3,1,1,1,1,1,1,3,1,1,3,1,1,3,1,1,1,1,3,1,1,1,1,1,1,3, 1,1,1,1,1,1,3,1,1,3,1,1,3,1,1,1,1,3,0};
unsigned char code music_tone2[]={ 5,3,3,4,2,2,1,2,3,4,5,5,5,5,3,3,4,2,2,1,3,5,5,3,2,2,2,2,2,3,4,3,3,3,3,3,4,5,5,3,3,4,2,2,1,3,5,5,1};	
unsigned char code beat3[] = { 1,1,2,2, 2,4,1,1,2,2,2,4,1,1,2,2,2,2,2,1,1,2,2,2,4,0};
unsigned char code music_tone3[] ={1,1,2,1,4,3,1,1,2,1,5,4,1,1,13,6,4,3,2,7,7,6,4,5,4};
unsigned char code beat4[]={4,2,2,4,4,1,1,1,1,6,4,2,2,4,4,1,1,1,1,6,4,2,2,4,4,4,2,2,6,4,2,2,4,4,4,2,2,6,0};
unsigned char code music_tone4[]={1,1,2,3,3,2,1,2,3,1,3,3,4,5,5,4,3,4,5,3,13,7,6,5,3,13,7,6,5,6,7,13,5,3,5,4,2,1};
void pmw_timer0() interrupt 1 {
	if(!pwm_flag ) {	//Start of High level
		pwm_flag = 1;	//Set flag		
		PWMPIN = 1;		//Set PWM o/p pin
		TH0 = 255 - pwm_width;	//Load timer
		TF0 = 0;		//Clear interrupt flag
	}else {				//Start of Low level
		pwm_flag = 0;	//Clear flag
		PWMPIN = 0;		//Clear PWM o/p pin
		TH0 = pwm_width;//Load timer
		TF0 = 0;		//Clear Interrupt flag
	}
}
void delay( long x){	//delay
	long i;
	for( i = x ; i > 0 ; i-- ){
	}
}
void main(){
	init_uart_timer2();		//UART
	init_buzzer_timer1();	//buzzer
	init_pwm_timer0();		//pwm
	while(1){
		int i;
		if(dir==STOP)		//stop buzzer stop
			TR1=0;
		else{
			TR1=1;			//go buzzer go
			if(dir==FORWARD){//forward music
			  for( i = 0 ; beat1[i] != 0 ; i++){
					P1=music_tone1[i];
					current_tone = music_tone1[i];
					tone_length = beat1[i]*200;
					while( tone_length != 0 );
					ET1 = 0;
					SPK = 0;
					delay(1000);
					ET1 = 1;
					if(dir!=FORWARD){//change dir exit
						TR1=0;
						break;
					}
				}
			}
			else if(dir==BACKWARD){//backward music
			  for( i = 0 ; beat2[i] != 0 ; i++){
					P1=music_tone2[i];
					current_tone = music_tone2[i];
					tone_length = beat2[i]*200;
					while( tone_length != 0 );
					ET1 = 0;
					SPK = 0;
					delay(1000);
					ET1 = 1;
					if(dir!=BACKWARD){//change dir exit
						TR1=0;
						break;
					}
				}
			}
			else if(dir==TURNLEFT){//turnleft music
			  for( i = 0 ; beat3[i] != 0 ; i++){
					P1=music_tone3[i];
					current_tone = music_tone3[i];
					tone_length = beat3[i]*200;
					while( tone_length != 0 );
					ET1 = 0;
					SPK = 0;
					delay(1000);
					ET1 = 1;
					if(dir!=TURNLEFT){//change dir exit
						TR1=0;
						break;
					}
				}
			}
			else if(dir==TURNRIGHT){//turnright music
			  for( i = 0 ; beat4[i] != 0 ; i++){
			    P1=music_tone4[i];
					current_tone = music_tone4[i];
					tone_length = beat4[i]*200;
					while( tone_length != 0 );
					ET1 = 0;
					SPK = 0;
					delay(1000);
					ET1 = 1;
					if(dir!=TURNRIGHT){//change dir exit
						TR1=0;
						break;
					}
				}
			}
		}
	}
}
void init_pwm_timer0(){//pwm inital using timer0
	TMOD = 0x01 | TMOD;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
void init_buzzer_timer1(){//pwm initial using timer1
	TMOD = 0x10 | TMOD;
	ET1 = 1;
	EA = 1;
	TR1 = 0;
}
void init_uart_timer2(){//pwm initial UART using timer2
	SCON = 0xd0;
	ET2 = 1; 	
	T2CON = 0x30;
	RCAP2L = (65536-39)%256;
	RCAP2H = (65536-39)/256;
	TR2 = 1;
	ES = 1;
	EA = 1;
	PS = 1;
}
void buzzer_timer1() interrupt 3{//interrupt buzzer
	TH1 = tone[current_tone-1]/256;
	TL1 = tone[current_tone-1]%256;
	if( buzzer_switch == 1 ){
		tone_length--;	
		SPK = !SPK;
	}else{
		SPK = 0;
	}
}
void SCON_ISR(void) interrupt 4{//interrupt motor
	char read_data;
	if( RI == 1 ){
		RI = 0 ;
		read_data = SBUF;
		if(read_data == 1 ){
			dir = FORWARD;
		}else if( read_data == 2 ){
			dir = TURNRIGHT;
		}else if( read_data == 3 ){
			dir = BACKWARD;
		}else if( read_data == 4 ){
			dir = TURNLEFT;
		}else if( read_data == 5 ){
			dir = STOP;
		}else if( read_data == 6 ){//speed down
			if( speed-10 > 120 ){
				speed -=10;
			}else{
				speed = 120;
			}
			
		}else if( read_data == 7 ){//speed up
			if( speed+10 < 250 ){
				speed+=10;
			}else{
				speed = 250;
			}
		}else if( read_data == 8 ){//buzzer switch
			buzzer_switch = ~buzzer_switch;
		}		
		set_direction();  //direction setting
		setspeed(speed);  //speed setting
	}
}
void set_direction(){ //direction setting
	switch( dir ){
		case FORWARD:
			P00 = 0;
			P01 = 1;
			P02 = 0;
			P03 = 1;
			
			P20 = 0;
			P21 = 1;
			P22 = 0;
			P23 = 1;
			
			break;
		case BACKWARD:			
			P00 = 1;
			P01 = 0;
			P02 = 1;
			P03 = 0;
			
			P20 = 1;
			P21 = 0;
			P22 = 1;
			P23 = 0;

			break;
		case TURNLEFT:
			P00 = 0;
			P01 = 1;
			P02 = 0;
			P03 = 1;
			
			P20 = 1;
			P21 = 0;
			P22 = 1;
			P23 = 0;
					
			break;
		case TURNRIGHT:
			P00 = 1;
			P01 = 0;
			P02 = 1;
			P03 = 0;
			
			P20 = 0;
			P21 = 1;
			P22 = 0;
			P23 = 1;
	
			break;
		case STOP:
			P00 = 1;
			P01 = 1;
			P02 = 1;
			P03 = 1;
			
			P20 = 1;
			P21 = 1;
			P22 = 1;
			P23 = 1;
			
			break;
	}
}
void setspeed(unsigned char in_speed){//speed setting
	pwm_width = in_speed;
	
}
