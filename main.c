/*
 * main.c
 *
 * Created: 5/20/2023 7:30:37 PM
 *  Author: lenovo
 */ 
#include "LCD16x2.h"
#include <string.h>
#include <xc.h>
char input;
const char calc_pad[4][4] = { //changed the keypad layout according to the proteus models
								{'7', '8', '9', '/'}, // 7 8 9 /
								{'4', '5', '6', '*'}, // 4 5 6 *
								{'1', '2', '3', '-'}, // 1 2 3 -
								{'.', '0', '=', '+'}, // . 0 = +
							};
					  
char keysense(){
	char pre_in = input;
	PORTB = 0;
	LCD_Clear();
	for(short i = 0; i<4 ; ++i){
		
		PORTB = 1<<i;
		if((PINC & 0x10)==1){
			input = calc_pad[i][5];
			return input;
		}

/*		for(short j = 0; j<5; ++j){ //Bug
			if( (PINB & 1<<(4+j))>0 ){
				LCD_Char(j+48);
				input = calc_pad[i][j];
				return input;
			}
		}
*/

// 		Lcd_set_cursor(0,2); //Debug
// 		LCD_Char(i|0x30);
		switch(PINB>>4){ // to check bitwise instead of a for-loop and store the value in 'input' accordingly
			case 1: input = calc_pad[i][0];
					break;
			case 2: input = calc_pad[i][1];
					break;		
			case 4: input = calc_pad[i][2];
					break;
			case 8: input = calc_pad[i][3];
					break;
			default: input = pre_in;
					break;			
		}
		if(input!=pre_in){
			_delay_ms(100);
//			PORTB = 0x00;
			return input;
		}
	}
	return 0;
}					  

char display_buffer[1];

int main(void)
{	
	//initialization related to calculator pad
	DDRB = 0x0F; //1st 4 bits are for ground last for are for sensing
	PORTB = (1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3);
	PORTB |= 0;
	DDRC |= 0x10; 
	
	LCD_Init();
	LCD_Command(0x0C);          // display on, cursor off
	LCD_String("Standard Calci");
	LCD_Clear();
    
	while(1)
    {
		char c = keysense();
		if(c!=0){
			Lcd_set_cursor(0,0);
			LCD_Char(c);
			_delay_ms(100);
			input = 0;
		}
    }
}