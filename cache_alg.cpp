#include "cache_alg.h"
#include "cache_log.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
const unsigned long long CACHE_SIZE=1024*1024*1024;
const unsigned int BLOCK_NUMBER= 256;
const unsigned int BLOCK_SIZE=1024*1024*4;
#define INITCache(m)  \
	do \
 {  \
	m=(char *)malloc(sizeof(char)*CACHE_SIZE); \
 } while(0);

#define FREECache(m) \
	do \
{ \
	free(m); \
} while(0);

char * cache(NULL);
Node * List(NULL);
Node * NodeMalloc(bool f, char * filename ,size_t st,size_t en, size_t sz, time_t at,time_t ct , int co)
{
    Node * tmp = (Node *)malloc(sizeof(struct Node) *1);
    if(tmp)
    {
        tmp->flag=f;
        tmp->file_name=filename;
        tmp->block_start=st;
        tmp->block_end=en;
        tmp->file_size=sz;
        tmp->file_atime=at;
        tmp->file_ctime=ct;
        tmp->file_count =co;
        tmp->next =NULL;
    }
    return tmp;
}

void InitCache()
{
	INITCache(cache);
	if(NULL == cache)
	{
          writelog(ERROR_INIT_CACHE);  
          exit(1);
	}
        char * s="cache";
        List =  NodeMalloc(false,s,0,BLOCK_NUMBER,CACHE_SIZE,0,0,0);
        if( List == NULL)
        {
            writelog(ERROR_INIT_CACHE);
            exit(1);
        }
        List->next=List;
}

void DestoryCache()
{
    Node * p =List;
    while(List)
    {
      p=List;
      List=List->next;
      free(p);
    }
    FREECache(cache);
}

Node * IsCached(char  * s)
{  
  Node * p=List;
  while(p->next != List)
  {
   if( p->flag == true && strcmp(p->file_name,s) == 0)
       return p;
   else
       p=p->next;
  }
   return NULL;
}

Node * CacheIn(char * s,long size)
{ // search for enough space for cache file
   int BlockNeed = (int)(size / BLOCK_SIZE)+1;
   Node * p = List;

   while(p ->next == List)
   {
     if( false == p->flag ) 
     {
        if( BlockNeed <= p->block_end - p->block_start + 1 )
        {
           break;
        }
     }
     p=p->next;
   }
   // if there is no enough space exec LRU alg
   if( p->next == List && BlockNeed > p->block_end - p->block_start +1  )
   {// exec LRU alg.
     p=Cache_LRU(s,BlockNeed);
   }
   //every file count ++ for this file access
     Node *q = List;
     while(q)
     {
         if( true == q->flag )
         { 
            q->file_count++;
         }
         if(q->next == List) break;
         q=q->next;
     }
    
   // here we have get enough space no mater it from lru alg or original
   // if the free space > BlockNeed then creat a new node for recording the remain free space
      if (BlockNeed <= p->block_end - p->block_start)
     {
      Node * node = NodeMalloc(false,s,p->block_start+BlockNeed,p->block_end,0,0,0,0);
     // sep p 
      node->next = p->next;
      p->next = node ;
     }
      // here we create the file cache node and set file count = 0
     p->flag= true;
     p->file_name = s;
     p->block_end = p->block_start+BlockNeed-1;
     p->file_size = size;
     p->file_count = 0 ;
     // return the file cache p
     return p;
// search enough space for s
}

Node * Cache_LRU(char * s,int BlockNeed)
{
    
  Node * tmp(List),*LRU(List);
  while( tmp->next != List )
  {
      if( BlockNeed <= tmp->block_end - tmp->block_start +1  && tmp ->file_count <LRU->file_count )
        LRU=tmp;
      tmp=tmp->next;
  }
  if(LRU->block_end - LRU->block_start +1 < BlockNeed)
    return NULL;
  return LRU;
}
void RearrangeCache()
{
  Node * p(List);
  Node * q;
  while ( p->next != List)
  {
      if(false == p->flag  && false == p->next->flag && p->block_end+1 == p->next->block_start)
      {
       q = p->next;
       p->next=q->next;
       p->block_end =q->block_end;
       free(q);
       continue;
      }
      p = p->next;
  }
}
int CacheRead(char * buffer, int size)
{
    memcpy(buffer,cache,size);
    return size;
}

int CacheWrite(int offset,char *buffer, int size)
{
    memcpy(cache+offset,buffer,size);
    return size;
}

