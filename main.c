/*
 * main.c
 *
 * Created: 5/20/2023 7:30:37 PM
 *  Author: lenovo
 */ 
#include "LCD16x2.h"
#include <string.h>
#include <xc.h>

//#define SCIENTIFIC_CALCULATOR
//#define STATIC_CALCULATOR

// declarations
char keysense(void);
void expression_input();
float Str_to_float(char str[],uint8_t size);
float solve(float,float,char);

#ifdef STATIC_CALCULATOR
struct dictionary{
	char key;
	float value;		
}values[26];

void assign_pair(uint8_t counter, float vl){
	values[counter].key = (char)counter;
	values[counter].value = vl;
}

float key_value(char c){
	for (uint8_t i =0; i<26; ++i)
	{
		if(values[i].key==c)
			return values[i].value;
	}
	return -1;
}
#endif

char input; //global variable to store the value of the key sensed from key 
const char calc_pad[4][4] = { //changed the keypad layout according to the proteus models
								{'7', '8', '9', '/'}, // 7 8 9 /
								{'4', '5', '6', '*'}, // 4 5 6 *
								{'1', '2', '3', '-'}, // 1 2 3 -
								{'.', '0', '=', '+'}, // . 0 = +
							};
					  				  
char expression[20];

int main(void)
{	
	//initialization related to calculator pad
	DDRB = 0x0F; //1st 4 bits are for ground last for are for sensing
	PORTB = 0x0f;
	PORTB |= 0;
	DDRC |= 0x10; 
	
	LCD_Init();
	LCD_Command(0x0C);          // display on, cursor off
	LCD_String("Standard Calculator");
	LCD_Clear();
    
	while(1)
    {
		expression_input();
    }
}

char keysense(){ //get input key by key
	PORTB = 0;
	for(short i = 0; i<4 ; ++i){
		
		PORTB = 1<<i;
		char c = PINB>>4;

		#ifdef SCIENTIFIC_CALCULATOR
			if((PINC & 0x10)==1){
				return calc_pad[i][5];
			}
		#endif

		switch(c){ // to check bitwise instead of a for-loop and store the value in 'input' accordingly
			case 1: input = calc_pad[i][0];
					return input;
					break;
			case 2: input = calc_pad[i][1];
					return input;
					break;
			case 4: input = calc_pad[i][2];
					return input;
					break;
			case 8: input = calc_pad[i][3];
					return input;
					break;
		}
		PINB &= 0x00;
	}
	input = 0;
	return 0;
}

void expression_input(){
	float op1;
	float op2;
	char oper = 0;
	char op_pos = 0;
	
	do{
		while(keysense()==expression[strlen(expression)]);
		Lcd_set_cursor(0,0);
		LCD_Char(strlen(expression)|0x30);
		if(input=='+' || input=='-' || input=='*' || input=='/' )
		{
			oper = input;
			op_pos = strlen(expression);
			break;
		}	
		expression[strlen(expression)] = input;
		LCD_String_xy(1,1,expression);
	}while(input!='=' && input!=0);
	
	
	if(input=='='){
		op1 = Str_to_float(expression, op_pos);
		op2 = Str_to_float(expression+op_pos, strlen(expression+op_pos));
		solve(op1, op2, oper);
	}
}

float solve(float operand1, float operand2, char oper){
	switch(oper){
		case '+': return operand1+operand2;
		case '-': return operand1+operand2;
		case '*': return operand1+operand2;
		case '/': return operand1+operand2;
	}
	return 0;
}

float Str_to_float(char str[],uint8_t size){
	float vl = 0;
	uint8_t factor = 1;
	uint8_t div = 0;
	for (uint8_t i = 0; i<size;++i)
	{
		if(str[i]=='.'){
			factor = 0;
			continue;
		}
		
		if(factor==1){
			vl *= 10;
			vl += str[i]-48;
		}
		else{
			vl += (str[i]-48)/pow(10,div++);
		}
	}
	return vl;
}

void float_to_str(char str[], float value){
	float temp = value;
	uint8_t factor = 0;
	if(temp<1){
		factor = 1;
		str[0] = '0';
		str[1] = '.';
	}
	
	uint8_t i =0;
	while(temp!=0){
		
	}
}