#ifndef CACHE_ALG
#define CACHE_ALG
#include <stddef.h>
#include <time.h>
struct Node
{
        bool flag;  // for free space
        char * file_name;// file name

	size_t block_start;// cache block start
        size_t block_end;  // cache block end

	size_t file_size;  // file size
	time_t file_atime; //
	time_t file_ctime; //

	int file_count;    // for cache alg
	struct Node *next;
        
};
struct recode{
    char filename[50];
    time_t time;
    int  block_id;
};
struct object{
    char filename[50];
    int block_id;
};
typedef time_t recode_time;

const unsigned long long CACHE_SIZE=1024*1024*1024;
const unsigned int BLOCK_SIZE=1024*1024*4;
const unsigned int BLOCK_NUMBER= CACHE_SIZE/BLOCK_SIZE;

recode * recodeMalloc(char * , double , size_t);
Node * NodeMalloc(bool, char *,size_t,size_t,size_t,time_t ,time_t ,int);
/*
 *malloc  cache and set cache flag
 */
void InitCache();
void InitFailure(int );
void DestoryCache();
int opendmdata(const char * );
int writerecode(const char *, const double , const size_t );
int closedmdata();
/*
 * Is or not cache for the input file
 */
Node * IsCached(char *);
/*
 *Cache file in cache
 *if there is enough space for file done.
 *else start cache_alg.
 */
Node * CacheIn(char * ,long);
void   RearrangeCache();
Node * Cache_LRU(char * ,int);
int   CacheRead(char * ,int); 
int   CacheWrite(int ,int , char * , int size);
int   readrecode(const char *);
int   printrecode();
#endif
