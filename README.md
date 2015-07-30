### Cache
 This cache project is based on pvfs file system  and the cache is an independant mode, might it depand on pvfs api I wrote.
 and this cache project has the following modules:
 
 prefecting read and delayed write,  pre read with data mining.
 
 cache replacement alg  
 
 distributed cache consistency 
 
 log  for record all operations
 
 Rdma for communicate with other node without cpu
 
 pvfs api for access pvfs
 
 now i have finish log, pvfs api, and base cache deamon
####pvfs api
  pvfs do not provide a normally api like traditional file system such fopen fread fclose and FILE struct in C . 
  
  with this reason i provide a like traditional api based pvfs api. 
  
  you can use these api such as :
  
  ```
  c:#include "pvfsapi.h"
  c++: extern "C"
   {
     #include "pvfsapi.h"
   }
   
  struct pfile ;  // represent the pvfs file 
  pvfsInit(),pvfsDestory(); // init and destory with pvfs file system. 
  pvfsopen(pfile *, char * file_name ,char * mode)
  pvfsclose(pfile *);
  pvfsread(pfile *, char * buffer, int buffer_size, int current_offset):
  pvfswrite(pfile *, char * buffer,  int buffer_size , int current_offset);
  ```
  Here is a simple test :
  
  ```
  1 #include <stdio.h>
  2 #include <time.h>
  3 #include <malloc.h>
  4 #include "cache_log.h"
  5 #include "cache_alg.h"
  6 extern "C"
  7 {
  8 #include "pvfsapi.h"
  9 }
 10 using namespace std;
 11 int main (int argc, char ** argv)
 12 {
 13 /*
 14         ret == openlog(argv[1]);
 15         if (ERROR_OPENFILE == ret )
 16              return  ERROR_OPENFILE ;
 17         writelog(ERROR_OPENFILE);
 18         writelog(ERROR_INIT_CACHE);
 19         writelog(ERROR_WRITEFILE);
 20         writelog(ERROR_MALLOC_CACHE);
 21 
 22         closelog();
 23                */
 24         pvfsInit();  // once you want access pvfs file you should call this pvfsInit
 25         pfile fp;  // define the pvfs file
 26 
 27         char * name="/mnt/pvfs2/server.c";  // name of you pvfs file
 28         pvfsopen(&fp,name,"r");  // open it.
 29         char *buffer;  
 30         buffer = (char *)malloc(sizeof(char)*1024);  malloc a buffer memory for date from pvfs file
 31         pvfsread(&fp,buffer,1024,0); // return the data size read this time
 32         printf("%s",buffer);
 33         pvfsclose(& fp);
 34        pvfsDestory();
 35        //return 0;
 36 
 37 }
  ```
  
###
