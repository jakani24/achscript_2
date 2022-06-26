#include <iostream>
#include <windows.h>
#include <conio.h>
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
		case 8: //buffer to big
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured. Charbuffer is to big for file operation (buffer to big): %s\n",message);
			SetConsoleTextAttribute(console_color, 7);					
		break;		
		case 9: //code scanner error
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured. The script tried to run a function that is considered dangerous.\nSkipping the function\n");
			SetConsoleTextAttribute(console_color, 7);					
		break;
		case 10:
			SetConsoleTextAttribute(console_color, 4);
			printf("\n[ERROR]: An error occured. Expected char varialbe, but ineger given (wrong datatype)\n");
			SetConsoleTextAttribute(console_color, 7);				
		break;
	}
}

void bluescreen(char message[500],int crash_id)
{
	system("color 30");
	printf("\n\nThere was an unexpected and maybe dangerous error that could not get handled by achscript\n");
	printf("\nFurter information: %s\n",message);
	printf("\nCrash ID: %d\n",crash_id);
	printf("\nError type: ");
	switch(crash_id)
	{
		case 1:
			printf("buffer_overflow\n");
		break;
		default:
			printf("err_not_known/unknown_err\n");
		break;
	}
	printf("press any key to continue:...\n");
	getch();
	exit(2);
}
#endif