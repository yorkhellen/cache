#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include "cache_log.h"
#include "cache_alg.h"
extern "C"
{
#include "pvfsapi.h"
}
using namespace std;
int main (int argc, char ** argv)
{
/*
    ret == openlog(argv[1]);
	if (ERROR_OPENFILE == ret )
	     return  ERROR_OPENFILE ;
	writelog(ERROR_OPENFILE);
	writelog(ERROR_INIT_CACHE);
	writelog(ERROR_WRITEFILE);
	writelog(ERROR_MALLOC_CACHE);

	closelog();
               
        pvfsInit();
        pfile fp;

        char * name="/mnt/pvfs2/server.c";
        pvfsopen(&fp,name,"r");
        char *buffer;
        buffer = (char *)malloc(sizeof(char)*1024);
        pvfsread(&fp,buffer,1024,0);
        printf("%s",buffer);
        pvfsclose(& fp);
       	//pvfsread(1,name);
        printf("\n\n\n\n\n\n\n\n\n");
        //pvfsread(1,name);
        pvfsDestory();
        */
        //return 0;

/*InitCache();
 char * name = "/mnt/pvfs2/server.c";
 Node * p  = IsCached(name);
 if(NULL == p )
 {

     Node *q = CacheIn(name,6000);
     char * buffer  = (char *) malloc( sizeof(char )*6000);
     CacheWrite(q->block_start*1024*1024*4,buffer,6000);
}
*/

 //DestoryCache();

   opendmdata("/home/york/cacheback/dm.data");
   writerecode("/mnt/pvfs2/server.c", lcurrent_time(),2);
   closedmdata();

}
