#include <iostream>
#include "../error/error.h"
int get_line(FILE*fp,char*output,int max)
{
	int ascii,ascii_old,cnt=0,cnt2,ret_value=0;
	char a;
	fscanf(fp,"%c",&a);//to get the first " " (32)
	fscanf(fp,"%c",&a);
	if(a!='\"')
	{
		ret_value=1;
		output[0]=(int)a;
		cnt=1;
	}
	else
	{
		ret_value=0;
	}
	while(ascii!='\"' && cnt<max-5 && ascii!='\n')
	{
		
		ascii_old=ascii;
		fscanf(fp,"%c",&a);
		ascii=(int)a;
		if(ascii!='\n') //\n repersents the line ending
		{
			if(ascii=='n'&&ascii_old=='\\')
			{
				output[cnt]='\n';
			}
			else if(ascii=='\\'&&ascii_old=='\\')
			{
				output[cnt]='\\';
			}
			else if(ascii=='\"')
			{
				cnt--;
			}
			else if(ascii=='2'&&ascii_old=='\\')
			{
				output[cnt]='\"';
			}
			else if(ascii=='\\')
			{
				cnt--;
			}
			else if(ascii==-74&&ascii_old==-61)
			{
				cnt--;
				output[cnt]=148;
			}
			else if(ascii==-92&&ascii_old==-61)
			{
				cnt--;
				output[cnt]=132;
			}
			else if(ascii==-68&&ascii_old==-61)
			{
				cnt--;
				output[cnt]=129;
			}
			else if(ascii!='\\')
			{
				output[cnt]=ascii;
			}
		}
		cnt++;
	}
	output[cnt]='\0';
	return ret_value;
}