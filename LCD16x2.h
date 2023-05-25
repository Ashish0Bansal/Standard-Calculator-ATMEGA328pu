/*
 * LCD16x2.h
 *
 * Created: 20-05-2023 20:56:45
 *  Author: lenovo
 */ 


#ifndef LCD16X2_H_
#define LCD16X2_H_

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 32000000UL
#endif
#include <util/delay.h>

#define LCD_Data_Dir DDRD	/* Define LCD data port direction */
#define LCD_Command_Dir DDRC	/* Define LCD command port direction register */
#define LCD_Data_Port PORTD	/* Define LCD data port */
#define LCD_Command_Port PORTC	/* Define LCD data port */
#define RS PC0			/* Define Register Select signal pin */
#define RW PC1			/* Define Read/Write signal pin */
#define EN PC2			/* Define Enable signal pin */

void LCD_Command(unsigned char cmnd);
void LCD_Char (unsigned char char_data);
void LCD_Init (void);
void LCD_String (const char *str);
void Lcd_set_cursor(unsigned int row, uint8_t pos);
void LCD_String_xy (char row, char pos,const char *str) ;
void LCD_Clear();


#endif /* LCD16X2_H_ */