#include <stdio.h>
#include <time.h>
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
               */
        pvfsInit();
        char * name="/mnt/pvfs2/server.c";
       	pvfsread(1,name);
        printf("\n\n\n\n\n\n\n\n\n");
        pvfsread(1,name);
        pvfsDestory();
        //return 0;

//InitCache();
//DestoryCache();
}
