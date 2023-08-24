/*
 * atmega.c
 *
 * Created: 8/17/2023 10:22:19 AM
 * Author : PHONG VU
 */ 

#define F_CPU 16000000UL /* Define CPU frequency here 16MHz */
#include <avr/io.h>
#include <util/delay.h>

void push_data (){
	PORTC &=~(1<<2) ;
	PORTC |=(1<<2) ;
}

void latch_data (){
	PORTC &= 0b11111101 ;
	PORTC |= 0b00000010 ;
}

void select_layer(int layer)
{
	PORTD &= 0x00;
	switch(layer)
	{
		case 1:
		PORTD |= (1<<0);
		break;
		case 2:
		PORTD |=(1<<1);
		break;
		case 3:
		PORTD |=(1<<2);
		break;
		case 4:
		PORTD |=(1<<3);
		break;
	}
}

void update_74hc595(int temp1)
{
	latch_data();
	for(int i=0;i<16;i++){
		PORTC = PORTC | (temp1 & 0x0001) ;
		push_data();
		
		temp1 = temp1 >> 1 ;
		PORTC &= 0xFE;
	}
	latch_data();
}

void led_pos(int x, int y, int z){
	int d = (y - 1)*4 + x;
	uint16_t ledtest = 0x8000;
	update_74hc595(ledtest>>d);
		}

void display_led1(uint16_t temp2)
{
	for(int y=1;y<5;y++){
		select_layer(y);
		uint16_t temp1 = temp2;
		for(int i = 0; i<16;i++){
			update_74hc595(temp1 >> i);
			_delay_ms(200);
		}
	}
}

void display_led2(uint16_t temp2)
{
	for(int y=1;y<5;y++){
		select_layer(y);
		uint16_t temp1 = temp2;
		for(int i = 0; i<16;i = i+4){
			update_74hc595(temp1 >> i);
			_delay_ms(200);
		}
	}
}

int main (void)
{
	DDRD = 0xFF;
	DDRC = 0xFF; 
	PORTC = 0;
	//uint16_t temp1 = 0x8000;
	uint16_t temp2 = 0xF000;
	while(1)
	{	
		 display_led1( temp2);
					
	}
}


/*
select_layer(2);
update_74hc595(temp1);
temp1 = temp1 >> 1;
//display_led();
_delay_ms(500);*/