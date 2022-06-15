#include <iostream>
#include <windows.h>
char v[]="Jakach Achscript v 1.0.0, Security version 1";
void load_settings(bool*cmd_,bool*cmd_not_);
void modify_settings();
void save_settings(int cmd_,int cmd_not_);
bool dirExists(const std::string& dirName_in);
bool cmd=true,cmd_not=true;
void load_settings(bool*cmd_,bool*cmd_not_)
{
	int continue_=true;
	FILE*fp;
	if(dirExists("c:\\jakach")==false)
	{
		system("mkdir c:\\jakach");
	}
	if(dirExists("c:\\jakach\\achscript_2")==false)
	{
		system("mkdir c:\\jakach\\achscript_2");
	}
	if(dirExists("c:\\jakach\\achscript_2\\system")==false)
	{
		system("mkdir c:\\jakach\\achscript_2\\system");
	}
	if((fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","r"))==0)
	{
		continue_=false;
		if((fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","w"))==0)
		{
			printf("FATALE: couldn't recreate save file. No setting can be read. Defaults are applied\n");
		}
		else
		{
			fclose(fp);
			//continue_=true;
			printf("ERROR: couldn't open settings file. For your safety, all settings got changed to maximum security\n");
			*cmd_=false;
		}
	}
	if(continue_==true)
	{
		fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","r");
		fscanf(fp,"%d",cmd_);
		fscanf(fp,"%d",cmd_not_);
		fclose(fp);
	}
}

void modify_settings()
{
	char input_c[25];
	load_settings(&cmd,&cmd_not);
	printf("%s\n",v);
	printf("To change a setting, type the number in front of the setting and press enter. The setting will then be changed\n");
	printf("\n\n----general----\n\n");
	printf("Sorry, but there are no settings by the time\n");
	printf("\n----security----\n");
	printf("[101] Get notified, if a script imports the \"cmd\" functions\nthese functions"
	" can be used to run \"cmd or powershell\" commands. Currently: %s\n\n", cmd_not ? "ENABLED" : "DISABLED");
	printf("[102] Let a script run \"cmd or powershell\" commands. Currently: %s\n", cmd ? "ENABLED" : "DISABLED");
	scanf("%10s",&input_c);
	if((strcmp(input_c,"102"))==0)
	{
		if(cmd==true)
			cmd=false;
		else
			cmd=true;
	}
	if((strcmp(input_c,"101"))==0)
	{
		if(cmd_not==true)
			cmd_not=false;
		else
			cmd_not=true;		
	}
	save_settings(cmd,cmd_not);
}

void save_settings(int cmd_,int cmd_not_)
{
	int continue_=true;
	FILE*fp;
	if(dirExists("c:\\jakach")==false)
	{
		system("mkdir c:\\jakach");
	}
	if(dirExists("c:\\jakach\\achscript_2")==false)
	{
		system("mkdir c:\\jakach\\achscript_2");
	}
	if(dirExists("c:\\jakach\\achscript_2\\system")==false)
	{
		system("mkdir c:\\jakach\\achscript_2\\system");
	}
	if((fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","r"))==0)
	{
		continue_=false;
		if((fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","w"))==0)
		{
			printf("FATALE: couldn't recreate save file. No setting can be read. Defaults are applied\n");
		}
		else
		{
			fclose(fp);
			continue_=true;
		}
	}
	if(continue_==true)
	{
		fp=fopen("c:\\jakach\\achscript_2\\system\\settings.jf","w");
		fprintf(fp,"%d ",cmd_);
		fprintf(fp,"%d",cmd_not_);
		fclose(fp);
	}	
}

bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}