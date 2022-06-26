#include <iostream>
#include <string.h>
#include <conio.h>
#include "../error/error.cpp"
#include "../security/security.cpp"
#include "../functions/functions.cpp"
#ifndef variables_cpp
#define variables_cpp
#define num_of_int 1000
#define num_of_char 1000
#define size_of_char 505
#define size_of_file 500
#define size_of_name 505
int integers[num_of_int];
char integers_name[size_of_name][num_of_int];
int integers_cnt;

char chars[size_of_char][num_of_char];
char chars_name[size_of_name][num_of_int];
int chars_cnt;

FILE*files[size_of_file][num_of_int];
char files_name[size_of_name][num_of_int];
int files_cnt;

int load_int(char name[size_of_name], int value_true_false);
int load_char(char name[size_of_name],int value_true_false,char*output);
void new_int(char name[size_of_name])
{
	for(int i=0;i<size_of_name-5;i++)
	{
		integers_name[i][integers_cnt]=name[i];
	}
	integers_cnt++;	
}
void new_char(char name[size_of_name])
{
	for(int i=0;i<size_of_name-5;i++)
	{
		chars_name[i][chars_cnt]=name[i];
	}
	chars_cnt++;	
}
void new_file(char name[size_of_name])
{
	for(int i=0;i<size_of_name-5;i++)
	{
		files_name[i][files_cnt]=name[i];
	}
	files_cnt++;	
}
int is_var(char name[size_of_name])
{
	int is_var=true;
	for(int i=0;i<integers_cnt;i++)
	{
		is_var=true;
		for(int b=0;b<size_of_name-3;b++)
		{
			if(name[b]=='\0')
			{
				b=size_of_name;
			}
			else if(integers_name[b][i]!=name[b])
			{
				is_var=false;
			}
		}
		if(is_var==true)
		{
			return 1;
		}
	}
	is_var=true;
	for(int i=0;i<chars_cnt;i++)
	{
		is_var=true;
		for(int b=0;b<size_of_name-3;b++)
		{
			if(name[b]=='\0')
			{
				b=size_of_name;
			}
			else if(chars_name[b][i]!=name[b])
			{
				is_var=false;
			}
		}
		if(is_var==true)
		{
			return 2;
		}
	}	
	is_var=true;
	for(int i=0;i<files_cnt;i++)
	{
		is_var=true;
		for(int b=0;b<size_of_name-3;b++)
		{
			if(name[b]=='\0')
			{
				b=size_of_name;
			}
			else if(files_name[b][i]!=name[b])
			{
				is_var=false;
			}
		}
		if(is_var==true)
		{
			return 3;
		}
	}
	return 0;
}

void set_int(char name[size_of_name],int value)
{
	int id;
	id=load_int(name,false);
	integers[id]=value;
	
}
void set_char(char name[size_of_name],char value[500])
{
	int id;
	id=load_char(name,false,NULL);
	for(int i=0;i<strlen(value);i++)
	{
		chars[i][id]=value[i];
	}
}
void math(int op_id,char var1[size_of_name],int y,int z) //id= 0:=,1 :+=, 2:*=, 3:/=, 4:-=, 5:+, 6:*, 7:/, 8:-
{
	char input[300];
	int x=load_int(var1,true);
	switch(op_id)
	{
		case 1:
			set_int(var1,x+y);
		break;
		case 2:
			set_int(var1,x*y);
		break;
		case 3:
			if(y==0)
			{
				printf("div by 0; continue? (y/n)\n");
				scanf("%3s",&input);
				if((strcmp(input,"y"))==0)	
					set_int(var1,x/y);
			}
			else
			{
				set_int(var1,x/y);
			}
		break;
		case 4:
			set_int(var1,x-y);
		break;
		case 5:
			set_int(var1, y+z);
		break;
		case 6:
			set_int(var1, y*z);
		break;
		case 7:
			if(z==0)
			{
				printf("div by 0; continue? (y/n)\n");
				scanf("%3s",&input);
				if((strcmp(input,"y"))==0)	
					set_int(var1,x/z);
			}
			else
			{
				set_int(var1,x/z);
			}
		break;
		case 8:
			set_int(var1, y-z);
		break;
	}
}

void print_var(int i)
{
	printf("%d",i);	
}
void print_var(char c[500])
{
	printf("%s",c);	
}
int load_int(char name[size_of_name],int value_true_false)
{
	int is_var,b=0;
	for(int i=0;i<integers_cnt;i++)
	{
		is_var=true;
		b=0;
		while(b<size_of_name-1 && name[b]!='\0')
		{
			if(integers_name[b][i]!=name[b])
			{
				is_var=false;
			}
			b++;
		}
		if(is_var==true)
		{
			if(value_true_false==true)
				return integers[i];
			else
				return i;
		}
	}
	error(name,2);
	//printf("\nERROR: variable \"%s\" not found\n",name);
	return 0;	
}
int load_buf_int(char buf[500])
{
	int is_dig=true,z;
	for(int i=0;i<strlen(buf);i++)
	{
		if(!isdigit(buf[i]))
			is_dig=false;
	}
	if(is_dig==false)
	{
		z=load_int(buf,true);
	}
	else
	{
		z=std::atoi(buf);
	}
	return z;
}
int load_char(char name[size_of_name],int value_true_false,char*output)
{
	int is_var;
	int b=0;
	for(int i=0;i<chars_cnt;i++)
	{
		is_var=true;
		b=0;
		//for(int b=0;b<size_of_name-1;b++)
		while(b<size_of_name-1 && name[b]!='\0')
		{
			if(chars_name[b][i]!=name[b])
			{
				is_var=false;
			}
			b++;
		}
		if(is_var==true)
		{
			if(value_true_false==true)
			{
				for(int z=0;z<size_of_char-3;z++)
				{
					output[z]=chars[z][i];
				}
				return i;
			}
			else
				return i;
		}
	}
	error(name,2);
	//printf("\nERROR: variable \"%s\" not found\n",name);
	return 0;	
}

int is_num(char buf[500])
{
	int is_dig=true;
	for(int i=0;i<strlen(buf);i++)
	{
		if(!isdigit(buf[i]))
			is_dig=false;
	}
	return is_dig;
}

int load_var_buf(char input[500],char*output_c,int*output_i)//,FILE*fp,fpos_t pos_buf
{
	int type;
	char buf[500];
	switch(is_var(input))
	{
		case 1:
			*output_i=load_int(input,true);
			type=1;
		break;
		case 2:
			load_char(input,true,output_c);
			type=2;
		break;
		case 0:
			if(is_num(buf))
			{
				*output_i=atoi(buf);
				type=1;
			}
				else
				{
					if(is_num(buf))
					{
						*output_i=atoi(buf);
						type=3;
						//integer operations: ==,!=,<,>,<=,>=
					}
					else
					{
						type=4;
						strcpy(output_c,buf);
					}
				}			
		break;
	}
	return type;
}
int load_file_index(char name[size_of_name])
{
	int is_var;
	int b=0;
	for(int i=0;i<files_cnt;i++)
	{
		is_var=true;
		b=0;
		//for(int b=0;b<size_of_name-1;b++)
		while(b<size_of_name-1 && name[b]!='\0')
		{
			if(files_name[b][i]!=name[b])
			{
				is_var=false;
			}
			b++;
		}
		if(is_var==true)
		{
			return i;
		}
	}
	error(name,2);
	return 0;		
}
int create_missing_dir(char path[500])
{
	char path2[500];
	int len=strlen(path);
	int cnt=len;
	int ch;
	int l2_cnt;
	char buf[505];
	strcpy_s(path2,path);
	while(ch!='\\' && cnt!=0)
	{
		ch=path2[cnt];
//		printf("@debug: ch:%d,%c cnt:%d\n",ch,ch,cnt);
		cnt--;
	}
	//cnt=index von hinten
	while(l2_cnt!= cnt+1)
	{
		buf[l2_cnt]=path2[l2_cnt];
		l2_cnt++;
	}
//	printf("@debug buf:%s, path2:%s\n",buf,path2);
	if(!dirExists(buf))
	{
		strcpy(path2,"mkdir ");
		strcat(path2,buf);
//		printf("@debug: path:%s\n",path);
		if(strlen(path2)>500)
		{
			sleep(2);
			bluescreen("ERR_BUFFER_TO_LOW",1);			
		}
		system(path2);
	}
}
void file_open(char name[305],char location[305],char mode[15])
{
	int cnt;
	char buf[500];
	while(cnt!=303)
	{
		location[cnt]=tolower(location[cnt]);
		cnt++;
	}
	if(!sdbox_mode())
	{
		if((strstr(location,"system\\settings.jf")) or ((strstr(location,"system")) && (strstr(location,"settings.jf"))))
		{
			error(location,6);
			strcpy(location,"c:\\jakach\\achscript_2\\buf.jf");
		}
		create_missing_dir(location);
		int i=load_file_index(name);
		if((*files[i]=fopen(location,mode))==0)
		{
			error(location,5);
		}
	}
	else
	{
		if(location[1]==':')
		{
			location[1]='c';
		}
		strcpy_s(buf,"c:\\jakach\\achscript_2\\sdbox\\1\\emulated\\");
		strcat_s(buf,location);
		if((strstr(location,"system\\settings.jf")))
		{
			error(location,6);
			strcpy(buf,"c:\\jakach\\achscript_2\\sdbox\\1\\emulated\\buf.jf");
		}
		create_missing_dir(buf);
		int i=load_file_index(name);
		if((*files[i]=fopen(buf,mode))==0)
		{
			error(buf,5);
		}
	}
}
void rewind_file(char name[305])
{
	int i=load_file_index(name);
	rewind(*files[i]);
}
void file_close(char name[305])
{
	fclose(*files[load_file_index(name)]);	
}
void fscan(char filename[305],char*output)
{
	fscanf(*files[load_file_index(filename)],"%495s",output);
}
void fscan(char filename[305],int*output)
{
	fscanf(*files[load_file_index(filename)],"%d",output);	
}
void fprint(char filename[305],int buf)
{
	fprintf(*files[load_file_index(filename)],"%d",buf);	
}

void fprint(char filename[305],char buf[500])
{
	fprintf(*files[load_file_index(filename)],"%s",buf);		
}

bool chek_file_ptr(char name[500])
{
	int i=load_file_index(name);
	if(*files[i]==0)
		return 0;
	else
		return 1;
}
#endif