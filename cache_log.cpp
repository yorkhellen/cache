#include <stdio.h>
#include <time.h>
#include <malloc.h>
using namespace std;
#include "cache_log.h"
FILE *fp;
int openlog(char * file)
{
 fp = fopen(file,"at");
  if(fp)
     return EXEC_CORRECT;
   return ERROR_OPENFILE;
  
}

inline void print_time()
{
 char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
 time_t timep;
 struct tm *p;
 time(&timep);
 p=localtime(&timep);
 fprintf (fp,"[%d-%d-%d %s %d:%d:%d]: ", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday,wday[p->tm_wday],p->tm_hour, p->tm_min, p->tm_sec);
}

int writelog(int fag)
{	
    print_time();
    switch( fag)
	{
		case ERROR_OPENFILE :
			fprintf(fp,"open log file error\n");
			break;
		case ERROR_INIT_CACHE :
			fprintf(fp, "Init cache faild \n" );
                        break;
                case ERROR_MALLOC_CACHE :
			fprintf(fp, "malloc cache faild \n");
			break;
		case ERROR_CLOSE :
			fprintf(fp,"close log file faild \n");
			break;
		case ERROR_WRITEFILE:
			fprintf(fp,"write log file faild \n");
                        break;
                case EXIT_CORRECT:
                        fprintf(fp,"exit correct \n");
                        break;
                case ERROR_SEND_FILE_FAILED:
                        fprintf(fp,"send file failed\n");
                        break;
                case ERROR_CREATE_SOCKET_FAILED:
                        fprintf(fp,"create socket failed\n");
                        break;
                case ERROR_SERVER_BIND_PORT_FAILED:
                        fprintf(fp,"bind port failed\n");
                        break;
                case ERROR_SERVER_LISTEN_FAILED :
                        fprintf(fp,"Listen failed\n");
                        break;
                case ERROR_SERVER_ACCEPT_FAILED:
                        fprintf(fp,"accept failed\n");
                        break;
                case ERROR_SERVER_RECIEVE_DATA_FAILED:
                        fprintf(fp,"recieve data failed\n");
        	        break;
                case ERROR_INIT_SERVER_DAEMON:
                        fprintf(fp,"daemon start failed\n");
                        break;
                case ERROR_SERVER_CREATE_CHILD_THREAD:
                        fprintf(fp,"create child thread failed\n");
                        break;
		default :
			 break;
	}	
}
 time_t lcurrent_time()
{ 
  time_t c_time=0;
  time(&c_time);
  return c_time;
}
int closelog()
{
  return  fclose(fp);
}
