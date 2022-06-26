#include <iostream>
#include <time.h>
#include "../file_functions/file_functions.cpp"
#include "../variables/variables.cpp"
#include "../settings/settings.cpp"
#include "../error/error.cpp"
#include "../security/security.cpp"
#include "../functions/uff.cpp"
#include "../var_loader/var_loader.cpp"
using namespace std;
int reader_start(char path[300]);
void reader(char path[300]);
int compare(char command[500],FILE*fp);
FILE*fp_s;
bool cmd_r,cmd_not_r,c_scan_r;
bool imported[100];

//Variabeln für compare
char buf[500];
char buf2[500];
char buf3[500];
char buf4[500];
int ret_value,op,run_op;
int if_cnt,while_cnt,else_cnt;
int cnt_buf;
int sleep_dur;
int x,y,z,is_dig;
int if_op,if_kl_cnt[100000];
int else_kl_cnt[100000];
int while_op,while_kl_cnt[100000];
int else_arr[100000];
int if_type;
int if_old_cnt;
fpos_t pos_if[1000];
fpos_t pos_while[1000];
fpos_t pos_print[3];
int buffer_check_cnt=0;
char buffer_chk[]="          ";

int start_script(char path[300])
{
	srand (time(NULL));
	printf("setting up workspace\n");
	if((fp_s=fopen(path,"r"))==0)
	{
		cout<<"[ERROR]: file not found"<<endl;
		return 1;
	}
	else
	{
		fclose(fp_s);
		load_settings(&cmd_r,&cmd_not_r,&c_scan_r);
		for(int i;i<99;i++)
		{
			imported[i]=false;
		}
		reader_start(path);
	}
	return 0;
}

int reader_start(char path[300])
{
	char buf[505];
	if((fp_s=fopen(path,"r"))==0)
	{
		cout<<"[ERROR]: file not found"<<endl;
		return 1;
	}
	else
	{
		//file überprüfung
		while(!feof(fp_s))
		{
			fscanf(fp_s,"%500s",&buf);
		}
		if((strcmp(buf,"end_"))!=0)
		{
			fclose(fp_s);
			fp_s=fopen(path,"a");
			fprintf(fp_s,"\nend_");
		}
		fclose(fp_s);
		reader(path);
	}
	return 0;
}

void reader(char path[300])
{
	char command[500];
	fp_s=fopen(path,"r");
	while(!feof(fp_s))
	{
		fscanf(fp_s,"%495s",&command);
		compare(command,fp_s);
	}
	fclose(fp_s);
}

int compare(char command[500],FILE*fp)
{
	fpos_t pos_buf[5];
	char l_buf[500];
	char buf_else[500];
	char buf_if[500];
	char buf2_if[500];
	char buf3_if[500];
	char buf4_if[500];
	char buf_while[500];
	char buf1_while[500];
	char buf2_while[500];
	char buf3_while[500];
	char buf4_while[500];
	char var1_while[500],var2_while[500];
	char op_while[500];
	int ib1,ib2,ib3;
	FILE*function;
	
	buffer_check_cnt++;
	if(buffer_check_cnt>=15)
	{
		buffer_check_cnt=0;
		if((strcmp(buffer_chk,"          "))!=0)
		{
			sleep(2);
			bluescreen("ERR_BUFFER_TO_LOW",1);
		}
	}
	if(c_scan_r==true)
	{
		switch(code_scanner(fp,command))
		{
			case 0:
				//no violation
			break;
			case 1:
				error("",9);
				return 0;
			break;
			case 2:
				error("",9);
				printf("This risk is to high to continue, terminating process\n");
				pause();
				exit(0);
			break;
		}
	}
	if((strcmp(command,"print"))==0)
	{
		fgetpos(fp,&pos_print[0]);
		ret_value=get_line(fp,buf,500);
		if(ret_value==0)
			printf("%s",buf);
		else if(ret_value==1)
		{
			fsetpos(fp,&pos_print[0]);
			fscanf(fp,"%300s",&buf);
			switch(is_var(buf))
			{
				case 1:
					x=load_int(buf,true);
					print_var(x);
				break;
				case 2:
					load_char(buf,true,buf3);
					print_var(buf3);
				break;
				case 0:
					print_var(buf);
				break;
			}
		}

	}
	else if((strcmp(command,"run_func"))==0 or (strcmp(command,"run"))==0)
	{
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%300s",&buf); //file/function location
		if(is_var(buf))
		{
			load_char(buf,true,buf);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf,495);
		}		
		if((function=fopen(buf,"r"))==0)
		{
			error(buf,7);
		}
		else
		{
			while(!feof(function))
			{
				fscanf(function,"%495s",&l_buf);
				compare(l_buf,function);
			}
		}
	}
	else if((strcmp(command,"scan"))==0 or (strcmp(command,"input"))==0  )
	{
		fscanf(fp,"%500s",&buf);
		switch(is_var(buf))
		{
			case 0:
				//printf("ERROR: variable \"%s\" not found\n",buf);
				error(buf,2);
			break;
			case 1://integer
				scanf("%d",&ib3);
				set_int(buf,ib3);
			break;
			case 2://char
				scanf("%495s",&buf2);
				set_char(buf,buf2);
			break;
		}
	}
	else if((strcmp(command,"rename"))==0)
	{
		ib3=load_content(fp,buf,&ib1);
		ib2=load_content(fp,buf2,&ib1);
		if(ib2 !=2 or ib3!=2)
		{
			error("function: \"rename\"",10);
		}
		else
			rename(buf,buf2);
		//printf("@debug: buf,buf2; %s:%s\n",buf,buf2);
	}
	else if((strcmp(command,"rewind"))==0)
	{
		fscanf(fp,"%295s",&buf);
		rewind_file(buf);
	}
	else if((strcmp(command,"remove"))==0)
	{
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%300s",&buf);
		if(is_var(buf)==2)
		{
			load_char(buf,true,buf);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf,300);
		}	
		remove(buf);		
	}
	else if((strcmp(command,"test#var_loader"))==0)
	{
		switch(load_content(fp,buf,&ib1))
		{
			case 1:
				printf("@debug: num: %d\n",ib1);
			break;
			case 2:
				printf("@debug: str: %s\n",buf);
			break;
		}
	}
	else if((strcmp(command,"fscan"))==0)
	{
		fscanf(fp,"%295s",&buf2);//file
		fscanf(fp,"%295s",&buf);//output variable
		switch(is_var(buf))
		{
			case 1:
				fscan(buf2,&cnt_buf);
				set_int(buf,cnt_buf);
			break;
			case 2:
				fscan(buf2,buf4);
				set_char(buf,buf4);
			break;
		}
	}
	else if((strcmp(command,"fprint"))==0)
	{
		fscanf(fp,"%295s",&buf2);//file
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%295s",&buf);//output variable
		switch(is_var(buf))
		{
			case 1:
				ib1=atoi(buf);
				fprint(buf2,ib1);
			break;
			case 2:
				load_char(buf,true,buf);
				fprint(buf2,buf);
			break;
			case 0:
				if(is_num(buf))
				{
					fprint(buf2,atoi(buf));
				}
				else
				{
					fsetpos(fp,&pos_buf[0]);
					get_line(fp,buf,495);
					fprint(buf2,buf);
				}
			break;
		}
	}
	else if((strcmp(command,"//"))==0)
	{
		fgets(buf,500,fp);
		//this is a note: scipping the line;
	}
	else if((strcmp(command,"setpos"))==0 or (strcmp(command,"setcursorpos"))==0)
	{
		fscanf(fp,"%d",&ib1);
		fscanf(fp,"%d",&ib2);
		gotoxy(ib1,ib2);
	}
	else if((strcmp(command,"msgbox"))==0)
	{
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%300s",&buf);
		if(is_var(buf)==2)
		{
			load_char(buf,true,buf);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf,300);
		}	
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%300s",&buf2);
		if(is_var(buf2)==2)
		{
			load_char(buf2,true,buf2);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf2,300);
		}
		MessageBox(NULL,buf,buf2,MB_OK);
	}
	else if((strcmp(command,"using_cmd"))==0)
	{
		if(cmd_not_r==true)
		{
			if((MessageBox(NULL,"This script uses cmd commands. Please let a script only execute cmd commands, if you trust the script! Press \"YES\" to activate cmd commands or \"NO\" to block them","Information",MB_YESNO))==IDYES)
			{
				imported[0]=true;
			}
		}
		else
			imported[0]=true;
	}
	else if((strcmp(command,"exec"))==0)
	{
		get_line(fp,buf,300);
		if(cmd_r==true && imported[0]==true && sdbox_mode()==false)
		{
			if(is_var(buf))
			{
				load_char(buf,true,buf);
			}
			system(buf);
		}
		else
		{
		//	printf("\nERROR: cmd commands are not allowed\n");
			error(buf,3);
		}
		//cmd command
	}
	else if((strcmp(command,"if"))==0)
	{
		if_cnt++;
		fgetpos(fp,&pos_if[0]);
		fscanf(fp,"%500s",&buf_if);
		switch(is_var(buf_if))
		{
			case 1:
				ib1=load_int(buf_if,true);
				if_type=1;
				//integer
				//integer operations: ==,!=,<,>,<=,>=
			break;
			case 2:
				strcpy_s(buf2_if,buf_if);
				load_char(buf2_if,true,buf_if);
				if_type=2;
				//char
				//char operations: ==,!=
			break;
			case 3:
				if(chek_file_ptr(buf_if))
				{
					if_type=1;
					ib1=1;
				}
				else
				{
					if_type=1;
					ib1=0;
				}
			break;
			case 0:
				//novar;
				fsetpos(fp,&pos_if[0]);
				fscanf(fp,"%495s",&buf_if);
				if(is_num(buf_if))
				{
					ib1=atoi(buf_if);
					if_type=1;
				}
				else
				{
					fsetpos(fp,&pos_if[0]);
					get_line(fp,buf_if,495);
					if(is_num(buf_if))
					{
						ib1=atoi(buf_if);
						if_type=1;
						//integer operations: ==,!=,<,>,<=,>=
					}
					else
					{
						if_type=2;
						//buf=buf;
						//char operations: ==,!=
					}
				}
			break;
		}
		//loaded the first variables, now the same with the second var. And the op
		fscanf(fp,"%5s",buf4_if);//the op
		fgetpos(fp,&pos_if[0]);
		fscanf(fp,"%500s",&buf3_if);//second var
		switch(is_var(buf3_if))
		{
			case 1:
				ib3=load_int(buf3_if,true);
				//integer
				//integer operations: ==,!=,<,>,<=,>=
			break;
			case 2:
				strcpy_s(buf2_if,buf3_if);
				load_char(buf2_if,true,buf3_if);
				//char
				//char operations: ==,!=
			break;
			case 0:
				//novar;
				fsetpos(fp,&pos_if[0]);
				fscanf(fp,"%495s",&buf3_if);
				if(is_num(buf3_if))
				{
					ib3=atoi(buf3_if);
				}
				else
				{
					fsetpos(fp,&pos_if[0]);
					get_line(fp,buf3_if,495);
					if(is_num(buf3_if))
					{
						ib3=atoi(buf3_if);
						//integer operations: ==,!=,<,>,<=,>=
					}
					else
					{
						//buf=buf;
						//char operations: ==,!=
					}
				}
			break;
		}
		//now we have all the vars
		if(if_type==1)//1= integer if, 2= char if
		{
			//buf4=op
			if((strcmp(buf4_if,"=="))==0)
			{
				if(ib1==ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}			
			}
			else if((strcmp(buf4_if,"!="))==0)
			{
				if(ib1!=ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else if((strcmp(buf4_if,"<="))==0)
			{
				if(ib1<=ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else if((strcmp(buf4_if,">="))==0)
			{
				if(ib1>=ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else if((strcmp(buf4_if,"<"))==0)
			{
				if(ib1<ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else if((strcmp(buf4_if,">"))==0)
			{
				if(ib1>ib3)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else
			{
			//	printf("\n[ERROR]: unknown operation: %s\n",buf4_if);
				error(buf4_if,4);
			}	
		}
		else if(if_type==2)//buf_if,buf3_if
		{
			if((strcmp(buf4_if,"=="))==0)
			{
				if((strcmp(buf_if,buf3_if))==0)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
					//		if_cnt--;
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else if((strcmp(buf4_if,"!="))==0)
			{
				if((strcmp(buf_if,buf3_if))!=0)
				{
					else_arr[if_cnt]=true;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{	if_kl_cnt[if_cnt]++;	}
						else if((strcmp(buf_if,"}"))==0)
						{	if_kl_cnt[if_cnt]--;	}
						else {
						
							compare(buf_if,fp);
						}
					//	printf("\n@debug: true: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);
				}	
				else
				{
					else_arr[if_cnt]=false;
					do
					{
						fscanf(fp,"%500s",&buf_if);
						if((strcmp(buf_if,"{"))==0)
						{
							if_kl_cnt[if_cnt]++;
						}
						else if((strcmp(buf_if,"}"))==0)
						{	
							if_kl_cnt[if_cnt]--;
						}
						//printf("\n@debug: false: if_kl_cnt:%d,buf:%s, if_cnt:%d\n",if_kl_cnt[if_cnt],buf_if,if_cnt);
					}while(if_kl_cnt[if_cnt]!=0);			
				}							
			}
			else
			{
			//	printf("\n[ERROR]: unknown operation: %s\n",buf4_if);
				error(buf4_if,4);
			}
		}
		if_cnt--;
	}
	else if((strcmp(command,"else"))==0)
	{
		if_old_cnt=if_cnt+1;
		else_cnt++;
		if(else_arr[if_old_cnt]==false)
		{
			do{
				fscanf(fp,"%500s",&buf_else);
				if((strcmp(buf_else,"{"))==0)
				{	else_kl_cnt[else_cnt]++;	}
				else if((strcmp(buf_else,"}"))==0)
				{	else_kl_cnt[else_cnt]--;	}
				else {
					compare(buf_else,fp);
				}	
			}while(else_kl_cnt[else_cnt]!=0);
		}
		else
		{
			do{
				fscanf(fp,"%500s",&buf_else);
				if((strcmp(buf_else,"{"))==0)
				{	else_kl_cnt[else_cnt]++;	}
				else if((strcmp(buf_else,"}"))==0)
				{	else_kl_cnt[else_cnt]--;	}	
			}while(else_kl_cnt[else_cnt]!=0);			
		}
		else_cnt--;
	}
	else if((strcmp(command,"while"))==0)
	{
		int v1_t,v2_t;
		while_cnt++;
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%495s",&var1_while);
//		while_op=load_var_buf(var1_while,buf1_while,&ib1);
		switch(is_var(var1_while))
		{
			case 1:
				ib1=load_int(var1_while,true);
				v1_t=1;
			break;
			case 2:
				load_char(var1_while,true,buf1_while);
				v1_t=2;
			break;
			case 0:
				fsetpos(fp,&pos_buf[0]);
				fscanf(fp,"%495s",&var1_while);
				if(is_num(var1_while))
				{
					ib1=atoi(var1_while);
					v1_t=3;
				}
				else
				{
					fsetpos(fp,&pos_buf[0]);
					get_line(fp,var1_while,495);
					if(is_num(var1_while))
					{
						ib1=atoi(var1_while);
						v1_t=3;
						//integer operations: ==,!=,<,>,<=,>=
					}
					else
					{
						v1_t=4;
						strcpy_s(buf1_while,var1_while);
					}
				}
			break;
		}
		fscanf(fp,"%5s",&op_while);
		fgetpos(fp,&pos_buf[1]);
		fscanf(fp,"%495s",&var2_while);
//		load_var_buf(var2_while,buf2_while,&ib2);
		switch(is_var(var2_while))
		{
			case 1:
				ib2=load_int(var2_while,true);
				v2_t=1;
			break;
			case 2:
				load_char(var2_while,true,buf2_while);
				v2_t=2;
			break;
			case 0:
				fsetpos(fp,&pos_buf[1]);
				fscanf(fp,"%495s",&var2_while);
				if(is_num(var2_while))
				{
					ib2=atoi(var2_while);
					v2_t=3;
				}
				else
				{
					fsetpos(fp,&pos_buf[1]);
					get_line(fp,var2_while,495);
					if(is_num(var2_while))
					{
						ib2=atoi(var2_while);
						v2_t=3;
						//integer operations: ==,!=,<,>,<=,>=
					}
					else
					{
						v2_t=4;
						strcpy_s(buf2_while,var2_while);
					}
				}
			break;
		}	
		//loaded all the vars
		if(v1_t==1 or v1_t==3)
			while_op=1;
		else
			while_op=2;
		if(while_op==1)//integer
		{
			if((strcmp(op_while,"=="))==0)
			{
				if(ib1==ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1==ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			if((strcmp(op_while,"!="))==0)
			{
				if(ib1!=ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1!=ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			if((strcmp(op_while,"<="))==0)
			{
				if(ib1<=ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1<=ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			if((strcmp(op_while,">="))==0)
			{
				if(ib1>=ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1>=ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			if((strcmp(op_while,"<"))==0)
			{
				if(ib1<ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1<ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			if((strcmp(op_while,">"))==0)
			{
				if(ib1>ib2)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while(ib1>ib2)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==1)
							ib1=load_int(var1_while,true);
						if(v2_t==1)
							ib2=load_int(var2_while,true);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
		}
		else if(while_op==2)
		{
			if((strcmp(op_while,"=="))==0)
			{
				if((strcmp(buf1_while,buf2_while))==0)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while((strcmp(buf1_while,buf2_while))==0)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==2)
							load_char(var1_while,true,buf1_while);
						if(v2_t==2)
							load_char(var2_while,true,buf2_while);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}
			else if((strcmp(op_while,"!="))==0)
			{
				if((strcmp(buf1_while,buf2_while))!=0)
				{
					fgetpos(fp,&pos_while[while_cnt]);
					while((strcmp(buf1_while,buf2_while))!=0)
					{	
						do{
								fscanf(fp,"%500s",&buf_while);
								if((strcmp(buf_while,"{"))==0)
								{	
									while_kl_cnt[while_cnt]++;
								
								}
								else if((strcmp(buf_while,"}"))==0)
								{	
									while_kl_cnt[while_cnt]--;
									fsetpos(fp,&pos_while[while_cnt]);
								}
								else 
								{				
									compare(buf_while,fp);
								}	
						}while(while_kl_cnt[while_cnt]!=0);
						if(v1_t==2)
							load_char(var1_while,true,buf1_while);
						if(v2_t==2)
							load_char(var2_while,true,buf2_while);
					}
					do{
					fscanf(fp,"%500s",&buf_while);
					if((strcmp(buf_while,"{"))==0)
					{	while_kl_cnt[while_cnt]++;	}
					else if((strcmp(buf_while,"}"))==0)
					{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
				else
				{
					do{
						fscanf(fp,"%500s",&buf_while);
						if((strcmp(buf_while,"{"))==0)
						{	while_kl_cnt[while_cnt]++;	}
						else if((strcmp(buf_while,"}"))==0)
						{	while_kl_cnt[while_cnt]--;	}
					}while(while_kl_cnt[while_cnt]!=0);
				}
			}						
		}
		while_cnt--;
	}
	else if((strcmp(command,"int"))==0)
	{
		fscanf(fp,"%300s",&buf);
		new_int(buf);
	}
	else 
	if((strcmp(command,"FILE"))==0 or (strcmp(command,"file"))==0)
	{
		fscanf(fp,"%300s",&buf);
		new_file(buf);
	}
	else if((strcmp(command,"char"))==0)
	{
		fscanf(fp,"%300s",&buf);
		new_char(buf);		
	}
	else if((strcmp(command,"file_open"))==0 or (strcmp(command,"fopen"))==0)
	{
		fscanf(fp,"%300s",&buf2);//filevarname
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%300s",&buf); //file location
		if(is_var(buf))
		{
			load_char(buf,true,buf);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf,495);
		}
		fgetpos(fp,&pos_buf[0]);
		fscanf(fp,"%295s",&buf3);//file mode
		strcpy_s(buf4,buf3);
		if(is_var(buf3))
		{
			load_char(buf3,true,buf3);
		}
		else
		{
			fsetpos(fp,&pos_buf[0]);
			get_line(fp,buf3,495);
		}
/*		else if(buf3[0]=='\"')
		{
			buf3[0]=buf3[1];
			buf3[2]='\0';
		}
*/
		if(strlen(buf3)>10)
		{
			error(buf4,8);
		}
		else
			file_open(buf2,buf,buf3);		
	}
	else if((strcmp(command,"file_close"))==0 or (strcmp(command,"fclose"))==0)
	{
		fscanf(fp,"%295s",&buf);
		file_close(buf);
	}
	else if((strcmp(command,"pause"))==0 or (strcmp(command,"sleep"))==0)
	{
		fscanf(fp,"%d",&sleep_dur);
		sleep(sleep_dur);
	}
	else if((strcmp(command,"end_"))==0)
	{
		printf("reached the end of the script\nterminating process\n");
		printf("Press any key to continue\n");
		getch();
		exit(0);
	}
	else //the rest
	{
		//check if it is a variable
		switch(is_var(command))
		{
			case 2: // it is a char variable so we do some char macig.
				fscanf(fp,"%300s",&buf);
				if((strcmp(buf,"="))==0)
				{
					fgetpos(fp,&pos_buf[3]);		//get the first var
					if(get_line(fp,buf2,500)==1)
					{
						fsetpos(fp,&pos_buf[3]);
						fscanf(fp,"%300s",&buf2);
						load_char(buf2,true,buf2);
						//printf("@debug: a var\n");
					}
					fgetpos(fp,&pos_buf[0]);
					fscanf(fp,"%5s",&buf3);
					if((strcmp(buf3,"+"))==0)		//check for second var
					{
						fgetpos(fp,&pos_buf[1]);		//get the second var
						if(get_line(fp,buf,500)==1)
						{
							fsetpos(fp,&pos_buf[1]);
							fscanf(fp,"%300s",&buf);
							load_char(buf,true,buf);
						}						
						strcat_s(buf2,buf);
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
					}
					set_char(command,buf2);
				}
				else if((strcmp(buf,"+="))==0)
				{
					load_char(command,true,buf4);
					fgetpos(fp,&pos_buf[3]);		//get the first var
					if(get_line(fp,buf2,500)==1)
					{
						fsetpos(fp,&pos_buf[3]);
						fscanf(fp,"%300s",&buf2);
						load_char(buf2,true,buf2);
						//printf("@debug: a var\n");
					}
					fgetpos(fp,&pos_buf[0]);
					fscanf(fp,"%5s",&buf3);
					if((strcmp(buf3,"+"))==0)		//check for second var
					{
						fgetpos(fp,&pos_buf[1]);		//get the second var
						if(get_line(fp,buf,500)==1)
						{
							fsetpos(fp,&pos_buf[1]);
							fscanf(fp,"%300s",&buf);
							load_char(buf,true,buf);
						}						
						strcat_s(buf2,buf);
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
					}
					strcat_s(buf4,buf2);
					if(strlen(buf4)>=502)
					{
						sleep(2);
						bluescreen("ERR_BUFFER_TO_LOW",1);
					}
					set_char(command,buf4);
				}				
			break;
			case 1:									//1 equals its an integer variable so we will do some math :)
				run_op=true;
				strcpy_s(buf4,"");
				buf4[0]='\0';
			//	printf("is a variable\n");
				//this is an integer value, scanning the math operation
				fscanf(fp,"%300s",&buf);//operation
				if((strcmp(buf,"="))==0)
				{
					op=0;
					fscanf(fp,"%300s",&buf2);//first variable
					//printf("@debug: buf2:%s\n",buf2);
					if((strcmp(buf2,"rand"))==0)
					{
						run_op=0;
						load_content(fp,buf,&ib1);
						set_int(command,rand()%ib1);
					}
					else
					{
						for(int i;i<strlen(buf2);i++)
						{
							if(!isdigit(buf2[i]))
								is_dig=false;
						}
						if(is_dig==true)
						{
							z=0;
							fgetpos(fp,&pos_buf[0]);
							fscanf(fp,"%5s",&buf3);	//operation					
								if((strcmp(buf3,"+"))==0)
								{
									op=5;
									fscanf(fp,"%300s",&buf4);//last variable
								}
								else if((strcmp(buf3,"*"))==0)
								{
									op=6;
									fscanf(fp,"%300s",&buf4);//last variable
								}
								else if((strcmp(buf3,"/"))==0)
								{
									op=7;
									fscanf(fp,"%300s",&buf4);//last variable
								}
								else if((strcmp(buf3,"-"))==0)
								{
									op=8;
									fscanf(fp,"%300s",&buf4);//last variable
								}
								else
								{
									fsetpos(fp,&pos_buf[0]);
									op=5;
									strcpy_s(buf4,"");
									buf4[0]='\0';
								}											
						}
						else
						{
							z=0;
							fgetpos(fp,&pos_buf[0]);
							fscanf(fp,"%5s",&buf3);//operation
							if((strcmp(buf3,"+"))==0)
							{
								op=5;
								fscanf(fp,"%300s",&buf4);//second variable
							}
							else if((strcmp(buf3,"*"))==0)
							{
								op=6;
								fscanf(fp,"%300s",&buf4);//second variable
							}
							else if((strcmp(buf3,"/"))==0)
							{
								op=7;
								fscanf(fp,"%300s",&buf4);//second variable
							}
							else if((strcmp(buf3,"-"))==0)
							{
								op=8;
								fscanf(fp,"%300s",&buf4);//second variable
							}
							else
							{
								fsetpos(fp,&pos_buf[0]);
								op=5;
								strcpy_s(buf4,"");
								buf4[0]='\0';
							}
						}
					}
				}
				else if((strcmp(buf,"+="))==0)
				{
					op=1;
					fscanf(fp,"%300s",&buf2);//second variable
					fgetpos(fp,&pos_buf[0]);
					strcpy_s(buf3,"");
					fscanf(fp,"%5s",&buf3);//operation
					if((strcmp(buf3,"+"))==0)
					{
						fscanf(fp,"%300s",&buf4);//third variable
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1+ib2;
					}
					else if((strcmp(buf3,"*"))==0)
					{
						fscanf(fp,"%300s",&buf4);//third variable
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1*ib2;						
						
					}
					else if((strcmp(buf3,"/"))==0)
					{
						fscanf(fp,"%300s",&buf4);//third variable
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1/ib2;						
					}
					else if((strcmp(buf3,"-"))==0)
					{
						fscanf(fp,"%300s",&buf4);//third variable
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1-ib2;						
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
						op=1;
						strcpy_s(buf4,"");
						buf4[0]='\0';
					}	
					if(run_op==false)
					{
						//y=load_buf_int(command);
						math(op,command,z,0);
					}		
				}
				else if((strcmp(buf,"*="))==0)
				{
					op=2;
					fscanf(fp,"%300s",&buf2);//second variable
					fgetpos(fp,&pos_buf[0]);
					strcpy_s(buf3,"");
					fscanf(fp,"%5s",&buf3);//operation
					fscanf(fp,"%300s",&buf4);//second variable
					if((strcmp(buf3,"+"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1+ib2;
					}
					else if((strcmp(buf3,"*"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1*ib2;						
						
					}
					else if((strcmp(buf3,"/"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1/ib2;						
					}
					else if((strcmp(buf3,"-"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1-ib2;						
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
						op=2;
						strcpy_s(buf4,"");
						buf4[0]='\0';
					}	
					if(run_op==false)
					{
						y=load_buf_int(command);
						math(op,command,y,z);
					}
				}
				else if((strcmp(buf,"/="))==0)
				{
					op=3;
					fscanf(fp,"%300s",&buf2);//second variable
					fgetpos(fp,&pos_buf[0]);
					strcpy_s(buf3,"");
					fscanf(fp,"%5s",&buf3);//operation
					fscanf(fp,"%300s",&buf4);//second variable
					if((strcmp(buf3,"+"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1+ib2;
					}
					else if((strcmp(buf3,"*"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1*ib2;						
						
					}
					else if((strcmp(buf3,"/"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1/ib2;						
					}
					else if((strcmp(buf3,"-"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1-ib2;						
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
						op=3;
						strcpy_s(buf4,"");
						buf4[0]='\0';
					}	
					if(run_op==false)
					{
						y=load_buf_int(command);
						math(op,command,y,z);
					}
				}
				else if((strcmp(buf,"-="))==0)
				{
					op=4;
					fscanf(fp,"%300s",&buf2);//second variable
					fgetpos(fp,&pos_buf[0]);
					strcpy_s(buf3,"");
					fscanf(fp,"%5s",&buf3);//operation
					fscanf(fp,"%300s",&buf4);//second variable
					if((strcmp(buf3,"+"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1+ib2;
					}
					else if((strcmp(buf3,"*"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1*ib2;						
						
					}
					else if((strcmp(buf3,"/"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1/ib2;						
					}
					else if((strcmp(buf3,"-"))==0)
					{
						run_op=false;
						ib1=load_buf_int(buf2);
						ib2=load_buf_int(buf4);
						z=ib1-ib2;						
					}
					else
					{
						fsetpos(fp,&pos_buf[0]);
						op=4;
						strcpy_s(buf4,"");
						buf4[0]='\0';
					}	
					if(run_op==false)
					{
						y=load_buf_int(command);
						math(op,command,y,z);
					}
				}
				else
				{
					//error();
				}
				if(run_op==true)
				{
					y=load_buf_int(buf2);
					z=load_buf_int(buf4);
					math(op,command,y,z);
				}
		}
		if(is_var(command)==false)
		{
			error(command,1);
		}
	}
	if_old_cnt=if_cnt;
	return 0;
}
