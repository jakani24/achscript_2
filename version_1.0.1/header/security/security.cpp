#include <iostream>
#include "../functions/functions.cpp"
#ifndef security_cpp
#define security_cpp
bool is_sdbox_on=false;
void sandbox(bool state)
{
	is_sdbox_on=state;
}
bool sdbox_mode()
{
	return is_sdbox_on;
}

int setup_sdbox()
{
	if(!dirExists("c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\users"))
	{
		system("mkdir c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\users");
	}
	if(!dirExists("c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\windows"))
	{
		system("mkdir c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\windows");
	}
	if(!dirExists("c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\jakach"))
	{
		system("mkdir c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\jakach");
	}
	if(!dirExists("c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\temp"))
	{
		system("mkdir c:\\ProgramData\\jakach\\achscript_2\\sdbox\\1\\emulated\\cc\\temp");
	}
	return 0;
}

int scan_code(FILE*fp,char sample[505])
{
	return 0;
}

int code_scanner(FILE*fp,char command[510])
{
	fpos_t cs_fpos;
	char buffer[505];
	fgetpos(fp,&cs_fpos);
	if((strcmp(command,"exec"))==0)
	{
		fscanf(fp,"%200s",&buffer);
		for(int i=0;i<strlen(buffer);i++)
		{
			tolower(buffer[i]);
		}
		if((strstr(buffer,"rd /s /q")))
		{
			return 1;
		}
		else if((strstr(buffer,"attrib")))
		{
			return 1;
		}
		else if((strstr(buffer,"reg delete")))
		{
			return 1;
		}
		else if((strstr(buffer,"delete %systemdrive%\*.* /f /s")))
		{
			return 1;
		}
		else if((strstr(buffer,"ren")))
		{
			return 1;
		}
		else if((strstr(buffer,"route")))
		{
			return 1;
		}
		else
		{
			fsetpos(fp,&cs_fpos);
			return 0;
		}
	}
	else if((strcmp(command,"fopen"))==0 or (strcmp(command,"file_open"))==0)
	{
		fscanf(fp,"%200s",&buffer);
		fscanf(fp,"%200s",&buffer);
		for(int i=0;i<strlen(buffer);i++)
		{
			tolower(buffer[i]);
		}		
		if((strstr(buffer,"c:\\windows")) or (strstr(buffer,"c:\\\\windows"))  )
		{
			return 1;
		}
		else if((strstr(buffer,"c:\\drivers")) or (strstr(buffer,"c:\\\\drivers")))
		{
			return 1;
		}
		else
		{
			fsetpos(fp,&cs_fpos);
			return 0;			
		}
	}
	return 0;	
}
#endif