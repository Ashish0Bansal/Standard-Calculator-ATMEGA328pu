/*
 * main.c
 *
 * Created: 5/20/2023 7:30:37 PM
 *  Author: Ashish Bansal
 */ 
#include "LCD16x2.h"
#include <stdio.h>
#include <string.h>
#include <xc.h>

//#define SCIENTIFIC_CALCULATOR
//#define STATIC_CALCULATOR

//function declarations
char keysense(void);
void expression_input();
float Str_to_float(char str[],uint8_t size);
float solve(float,float,char);
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);

//variable declarations
char expression[30];
char result[20];
char input; //global variable to store the value of the key sensed from key
char debug_buffer[30];
const char calc_pad[4][4] = {        //changed the keypad layout according to the Proteus models
	{'7', '8', '9', '/'}, // 7 8 9 /
	{'4', '5', '6', '*'}, // 4 5 6 *
	{'1', '2', '3', '-'}, // 1 2 3 -
	{'.', '0', '=', '+'}, // . 0 = +
};

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

int main(void)
{	
	//initialization related to calculator pad
	DDRB = 0x0F; //1st 4 bits are for ground last for are for sensing
	PORTB = 0x0f;
	PORTB |= 0;
	DDRC |= 0x10; 
	
	LCD_Init();
	LCD_Command(0x0C);          // display on, cursor off
	LCD_String("Calculator");
	LCD_Clear();
    
	while(1)
    {
		expression_input();
		/*putch(strlen(result));*/
    }
}


/**************************************************************************
	To take the char by char input from keypad 4x4
**************************************************************************/
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
/**************************************************************************
	To take the expression input from keypad 4x4
**************************************************************************/
float op1=0;
float op2=-1;
uint8_t cal_flag=0; //used to check if the calculation is done. if yes it stores 1 else 0
float r; //result in float
char oper = 0; //operation to perform
uint8_t prev_in;
/*char debug_buffer[4];*/
void expression_input(){
	
	
	uint8_t decimal_count1[2] = {0,0};
	uint8_t d = 0;
	uint8_t dec = 0;
	
	do{
		while(keysense()==expression[strlen(expression)]);
		_delay_ms(10);
		
		if(dec == 1)
			decimal_count1[d]++;
		
		if(input == '.'){
			dec = 1;
		}
		
		if(input=='+' || input=='-' || input=='*' || input=='/' )
		{
			op1 = r;
			if(oper==0)
				op1 = Str_to_float(expression, strlen(expression));

			oper = input;
			Lcd_set_cursor(0,0);
			LCD_Char(input);
			memset(expression,0,strlen(expression));
			dec = 0;
			d=1;
			continue;
		}
		
		if(input!='='){
			expression[strlen(expression)] = input;
			cal_flag = 0;
		}
		
		if(oper==0 || cal_flag == 1){
			LCD_String_xy(0,15-strlen(expression), "             ");
			LCD_String_xy(0,16-strlen(expression),expression);
		}
		else{
			LCD_String_xy(1,15-strlen(expression), "             ");
			LCD_String_xy(1,16-strlen(expression),expression);
		}
		
	}while(input!='=' && input!=0);
	
	if(input=='='){
		if(cal_flag==0)
			op2 = Str_to_float(expression, strlen(expression));

			
		r = solve(op1, op2, oper);
		
		if(oper=='-' && op2>op1)
			result[0] = '-';
		else
			result[0] = ' ';
		
		if(oper == '/' && op2==0){
			LCD_String_xy(1,0, "INVALID INPUT");
			return;
		}
		
		if(r==0){
			result[1] = '0';
			result[2] = '\0';
		}else
			ftoa(r, result+1, decimal_count1[0]>decimal_count1[1]?decimal_count1[0]:decimal_count1[1]);
		
		LCD_String_xy(1,0, "ANS.");
		LCD_String_xy(1,16-strlen(result), result);
		op1 = r;
		cal_flag=1;
		prev_in = input;
		memset(expression,0,strlen(expression));
		return;
	}
}

float solve(float operand1, float operand2, char oper){
	switch(oper){
		case '+': return operand1+operand2;
		
		case '-': if(operand1>operand2)
					return operand1-operand2;
				  else
					return operand2-operand1;
		
		case '*': return operand1*operand2;
		
		case '/': return operand1/operand2;
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
			vl += (str[i]-48)/pow(10,++div);
		}
	}
	return vl;
}


void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

void ftoa(float n, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
