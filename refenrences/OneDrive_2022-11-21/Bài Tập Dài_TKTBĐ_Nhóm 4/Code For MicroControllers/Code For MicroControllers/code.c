// Declare necessary libraries and short form for pins and modes.
#include <REGX52.H>
#include <stdio.h>
sbit LED = P1 ^ 3;
sbit SOC = P3 ^ 0;
sbit EOC = P3 ^ 1;
sbit SELECT = P3 ^ 4;
sbit MODE = P1 ^ 2;
sbit MODEOUT = P1 ^ 3;
sbit LCD_RS = P1 ^ 4;
sbit LCD_EN = P1 ^ 6;
sbit LCD_RW = P1 ^ 5;
#define LCD_DATA P2 // Declare the data pin.
#define CURRENT 0x01
#define VOLTAGE 0x00


/*----------------------------------------------------------------------*/


// Below are necessary functions to work with LCD.

// Delay Function.
void Delay_ms(unsigned int t)    
{
	unsigned int x, y;
	for (x = 0; x < t; x++)
	{
		for (y = 0; y < 123; y++)
			;
	}
}

// Function to send instructions to the LCD.
void Lcd_cmd(unsigned char cmd)  
{
	LCD_RW = 0;		   // Select writing mode.
	LCD_RS = 0;		   // Select writing instruction mode.
	LCD_DATA = cmd;  // cmd is the data to send.
	LCD_EN = 0;
	LCD_EN = 1;
	if (cmd <= 0x02)
	{
		Delay_ms(2);
	}
	else
	{
		Delay_ms(1);
	}
}


// Function to display an arbitrary character at the position of the cursor.
void Lcd_Char_Cp(char c)      
{	
	LCD_RW = 0;
	LCD_RS = 1;	 	  			 // Select the mode to send data. 
	LCD_DATA = c;          // c is the character to send.
	LCD_EN = 0;
	LCD_EN = 1;
	Delay_ms(1);
}


// Function to send strings at the position of the cursor.
void Lcd_Out_Cp(char *str)      
{
	unsigned char i = 0;
	while (str[i] != 0) 					// Send the characters of the string until get the NULL value.
	{
		Lcd_Char_Cp(str[i]);
		i++;
	}
}


// Function to move the position of the cursor and display the string to the screen.
void Lcd_Out(unsigned char row, unsigned char col, char *str) 											          
{
	unsigned char cmd;
	// Move the cursor to the necessary position.
	cmd = (row == 1 ? 0x80 : 0xC0) + col - 1;
	Lcd_cmd(cmd);
	Lcd_Out_Cp(str);
}


// Function to init the LCD.
void Lcd_Init()   						 	
{
	Lcd_cmd(0x30);
	Delay_ms(5);
	Lcd_cmd(0x30);
	Delay_ms(1);
	Lcd_cmd(0x30);
	Lcd_cmd(0x38); // Initialization of 16X2 LCD in 8bit mode.
	Lcd_cmd(0x01); // Clear display.
	Lcd_cmd(0x0C); // Display ON Cursor OFF.
}


/*----------------------------------------------------------------------*/


// Below is the main program.
// Define necessary variables.
unsigned char sel = 0;
char arr[30];
unsigned int value = 0;
unsigned int value1 = 0;
int result;
float voltage = 0;
// Main program.
int main()
	
{
	Lcd_Init();
	TMOD = 0x01;
	while (1)
	{
		if (MODE == 1)
		{
			MODEOUT = CURRENT;
			sel = CURRENT;
		}
		else
		{
			MODEOUT = VOLTAGE;
			sel = VOLTAGE;
		}
		TH0 = 0xFC;
		TL0 = 0x00;
		SELECT = 0;
		SOC = 1;
		Delay_ms(1);
		SOC = 0;
		TR0 = 1;
		while (!TF0);
		TF0 = 0;
		TR0 = 0;
		SELECT = 1;
		TH0 = 0xFC;
		TL0 = 0x00;
		TR0 = 1;
		while (EOC);
		TR0 = 0;
		value1 = TH0;
		value = TL0;
		TH0 = 0xFC;
		TL0 = 0x00;
		TF0 = 0;
		result = ((value1 - 252) << 8) + value;
		voltage = (((float)result / (float)1030) * 5);
		if (sel == CURRENT)
			sprintf(arr, "Voltage:%0.002fV", voltage);
		else
			sprintf(arr, "Current:%dmA", (int)((voltage / 5) * 1000));
		Lcd_Out(1, 1, arr);
		Delay_ms(100);
	}
}
