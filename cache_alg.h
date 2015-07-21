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
Node * NodeMalloc(bool, char *,size_t,size_t,size_t,time_t ,time_t ,int);
/*
 *malloc  cache and set cache flag
 */
void InitCache();
void InitFailure(int );
void DestoryCache();
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
#endif
