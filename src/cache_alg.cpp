#include "cache_alg.h"
#include "cache_log.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <wchar.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
using namespace std;
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
const long threadhold = 0.5;
char * cache(NULL);
Node * List(NULL);
FILE *ft ;

Node * 
NodeMalloc(bool f, char * filename ,size_t st,size_t en, size_t sz, time_t at,time_t ct , int co)
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

void 
InitCache()
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

void 
DestoryCache()
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

Node * 
IsCached(char  * s)
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

Node * 
CacheIn(char * s,long size)
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

Node * 
Cache_LRU(char * s,int BlockNeed)
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
void 
RearrangeCache()
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
int 
CacheRead(char * buffer, int block_id)
{
    memcpy(buffer,cache+BLOCK_SIZE*block_id,BLOCK_SIZE);
    return BLOCK_SIZE;
}

int 
CacheWrite(int block, int offset, char *buffer, int size)
{

    memcpy(cache+block*BLOCK_SIZE+offset,buffer,size);
    return size;
}

vector<struct recode> user_recode;

int 
opendmdata(const char * filename)
{
     ft=fopen(filename,"a");
}
int 
closedmdata()
{
    fclose(ft);
}
int 
writerecode(const char * filename ,const double time, const size_t block_id)
{
    fprintf(ft,"%s,%0.0lf,%d\n", filename,time,block_id);
}
int 
readrecode(const char * filename)
{
    fclose(ft);
    ft = fopen(filename,"r");
    //char buffer[50];
    //time_t time(0);
    //int block_id(0);
    struct recode tmp;
    int number_user_recode = user_recode.size();
    for(int i = 0 ; i <number_user_recode ; i++ )
      fscanf(ft,"%[^,],%ld,%d\n",tmp.filename,&(tmp.time),&(tmp.block_id));
    while(fscanf(ft,"%[^,],%ld,%d\n",tmp.filename,&(tmp.time),&(tmp.block_id)) > 0 )
    {
      user_recode.push_back(tmp);
    }
    fclose(ft);
    ft = fopen(filename,"a");
    return 0 ;
}
int 
printrecode()
{
    for (auto it :user_recode)
    {
        printf("%s %d  %ld\n",it.filename,it.block_id, it.time);
    }
}
struct cmpobject{
    bool operator()(const struct object & ob1, const struct object & ob2 )const
    {
      if (strcmp(ob1.filename,ob2.filename) == 0  && ob1.block_id == ob2.block_id )
          return true;
      else
          return false;
    }

};

void 
calculate(const int * matrix,const int object_num, const int recode_num ,
  const int index_key_object ,int * confidence ,int * improve,long *confidence_degree , long * improve_degree  )
{
  for(int i = 0 ; i <recode_num ; i++)
  {
    if(1 == *(matrix +i*object_num+ index_key_object))
    { 
       for(int j = 0 ; j < object_num ; j++)
       *(confidence+j) += *(matrix+i*object_num+j);
    }
    else
    {
        for(int j = 0 ; j<object_num ; j ++)
            *(improve+j)+= *(matrix+i*object_num+j);
    }
  }
  for(int j = 0 ; j <object_num ; j ++)
  {
      *(improve+j)+=*(confidence+j);
  }

      if(recode_num > 0 || *(confidence+index_key_object))
        for(int i = 0 ; i <object_num ; i ++)
        {
         *(confidence_degree+i) = *(confidence+i) / *(confidence+index_key_object);
         *(improve_degree +i ) = *(improve_degree + i) / recode_num ; 
        }

}

vector<struct object>&
aprior_alg(const vector<struct recode> & user_recode, const object & key_object)
{
  int recode_num(0),object_num(0);
  map<recode_time,int> recode_set ;
  map<object,int,cmpobject> object_set;
  for(auto it:user_recode)
  {
      recode_set[it.time]++;
      object tmp;
      memcpy(tmp.filename,it.filename,50);
      tmp.block_id = it.block_id;
      object_set[tmp]++;
  }
  recode_num=recode_set.size();
  object_num=object_set.size();
  int itmp(0);
 
  for(map<recode_time,int>::iterator it=recode_set.begin();it != recode_set.end() ; it++)
  {
      it->second =itmp;
      itmp++;
  }
  for (map<recode_time,int>::iterator it = recode_set.begin() ; it != recode_set.end(); it++)
  { 
      cout <<it->first << "   "<<it->second<<endl;
  }
  itmp=0;
  for(map<object,int,cmpobject>::iterator it=object_set.begin(); it!=object_set.end(); it++)
  {
      it->second == itmp;
      itmp++;
  }
// create matrix 
  int * recode_matrix = (int *) malloc(sizeof(int)*recode_num*object_num);
  memset(recode_matrix,0,recode_num*object_num*sizeof(int));
  for(auto it :user_recode)
  {
      object tmp;
      memcpy(tmp.filename,it.filename,50);
      tmp.block_id = it.block_id;
      int x = recode_set[it.time];
      int y = object_set[tmp];
      *(recode_matrix+x*object_num + y) = 1;
  }
  //calculate the degree of confidence
  int * confidence = (int *) malloc(sizeof(int)*object_num);
  int * improve = (int *) malloc(sizeof(int)*object_num);
  memset(confidence,0,object_num*sizeof(int));
  memset(improve,0,object_num*sizeof(int));
  //get key index of the key object
  int index_key_object = object_set[key_object];
  // count all object number 
  // caluate the confidence and the improve
  long *confidence_degree = (long *)malloc(sizeof(long)*object_num);
  long *improve_degree=(long *)malloc(sizeof(long)*object_num);
  memset(confidence_degree,0,object_num*sizeof(long));
  memset(improve_degree,0,sizeof(long)*object_num);
  calculate(recode_matrix,object_num,recode_num,index_key_object,confidence,improve,confidence_degree, improve_degree);
  
  // sort 
  vector<struct object > result ; 
  
  for(map<object,int,cmpobject>::iterator it=object_set.begin(); it!=object_set.end(); it++)
  {
     if(*(confidence_degree+it->second) > threadhold && ( *(confidence_degree+ it->second)/ (*(improve_degree+ it->second))))
         result.push_back(it->first);
  }
  free(recode_matrix);
  free(confidence);
  free(confidence_degree);
  free(improve_degree);
  free(improve);
  return result ;
}
vector <struct object> & aprior(const object & key_object)
{
    return aprior_alg(user_recode , key_object);
}
