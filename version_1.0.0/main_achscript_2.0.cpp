#include <iostream>
#include "header/script_reader/reader.cpp"
#include "header/functions/uff.cpp"
#include "header/error/error.cpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
void main_menu();
int main(int argc, char** argv)
{
	if(argc==2)
	{
		if((strlen(argv[1]))<300)
		{
			start_script(argv[1]); 
			pause();
		}
		else
		{
			cout<<"ERROR: path to long"<<endl;
		}
	}
	else
	{
		while(true)
		{
			system("cls");
			main_menu();
		}
	} 
	return 0;
}

void main_menu()
{
	int input;
	char input_c[400];
	system("title Achscript 2.0 v 1.0.0");
	cout<<"Welcome to the Jakach scripting language\n";
	printf("Start a script [press 1]\n");
	printf ("Settings [press 2]\n");
	printf("Exit [press 9]\n");
	input=getch();
	switch(input)
	{
		case '1':
			printf ("Please type in the path of the script and press enter\n");
			scanf("%300s",&input_c);
			start_script(input_c);
			pause();
		break;
		case '2':
			modify_settings();
		break;
		case '9':
			printf("goodby ;)\n");
			exit(0);
		break;
	}
}