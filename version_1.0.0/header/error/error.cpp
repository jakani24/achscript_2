#include <iostream>
#include <windows.h>
#ifndef error_cpp
#define error_cpp
void error(char message[300],int error_id)
{
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(error_id)
	{
		case 1: //comand not found
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while executing command (command not found): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);	
		break;	
		case 2: //variable not found
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while loading variable (variable not found): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);			
		break;
		case 3: //not allowed_commands
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while executing command (cmd commands are not allowed): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);				
		break;
		case 4: //unknown math operation
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while executing operation (operation not found): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);				
		break;
		case 5: //file not fund or no permission
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while opening file (couldn't open file): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);					
		break;
		case 6: //jakach\system file
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while opening file (file is a protected systemfile): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);					
		break;
		case 7: //file not found or no permission
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured while running function (functionfile not found): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);					
		break;			
	}
}
#endif