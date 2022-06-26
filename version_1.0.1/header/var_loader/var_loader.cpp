#include "../variables/variables.cpp"
#ifndef var_loader_cpp
#define var_loader_cpp
int load_content(FILE*fp,char*output_c,int*output_i)
{
	fpos_t pos;
	char buf[505];
	int type=0;
	fgetpos(fp,&pos);
	fscanf(fp,"%300s",&buf);
	switch(is_var(buf))
	{
		case 1:
			*output_i=load_int(buf,true);
			type=1;
		break;
		case 2:
			load_char(buf,true,output_c);
			type=2;
		break;
		case 0:
			fsetpos(fp,&pos);
			fscanf(fp,"%495s",output_c);
			if(is_num(output_c))
			{
				*output_i=atoi(output_c);
				type=1;
			}
			else
			{
				fsetpos(fp,&pos);
				get_line(fp,output_c,495);
				if(is_num(output_c))
				{
					*output_i=atoi(output_c);
					type=1;
				}
				else
				{
					//buf=buf
					type=2;
				}
			}			
		break;
	}
	return type;
}

#endif