#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>   // for thread
#include "cache_log.h"
#include "cache_alg.h"
extern "C"
{
#include "pvfsapi.h"
}

using namespace std;
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define HELLO_WORLD_SERVER_PORT    6666 
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#define ERR_EXIT(m) \
do \
{ \
   perror(m); \
   exit(EXIT_FAILURE); \
} \
while (0); 
struct thread_message
{
    int net_server_socket;
    char * file_name;
};
void creat_daemon(void);
void *handle_client(void *data);
void PvfsCall(int);
int main(int argc, char **argv)
{
    /*
     *Init log
     */
    int rt(0);
    rt = openlog("/home/york/cache/cache.log");
    if(ERROR_OPENFILE == rt)
    {
        ERR_EXIT("log open failed");
    }
    
    if(daemon(0,0) == -1 )
    {
        writelog(ERROR_INIT_SERVER_DAEMON);
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
 
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        writelog(ERROR_CREATE_SOCKET_FAILED);
        closelog();
        exit(1);
    }
      int opt =1;
      setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); 
    
      if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        writelog(ERROR_SERVER_BIND_PORT_FAILED);
        closelog();
        exit(1);
    }

    if( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        writelog(ERROR_SERVER_LISTEN_FAILED);
        closelog();
        exit(1);
    }
    /*   
     * Init Cache
     */
    InitCache();
    // loop for handle client request
    while (1) 
    {
        //定义客户端的socket地址结构client_addr
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        // 
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            writelog(ERROR_SERVER_ACCEPT_FAILED);
            break;
        }
        
        char buffer[BUFFER_SIZE];
        length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
        if ( length < 0 )
        {
            writelog (ERROR_SERVER_RECIEVE_DATA_FAILED);
            continue ;
        }
        struct thread_message child_message;
        child_message.net_server_socket = new_server_socket;
        child_message.file_name=buffer;
       // create thread 
        pthread_t child_thread;
        pthread_attr_t child_thread_attr;
        pthread_attr_init (&child_thread_attr);
        pthread_attr_setdetachstate(&child_thread_attr,PTHREAD_CREATE_DETACHED);

        if (pthread_create(&child_thread,&child_thread_attr,handle_client,&(child_message) )<0)
        {
           writelog(ERROR_SERVER_CREATE_CHILD_THREAD);
        }
    }
    pthread_mutex_destroy(&mutex);
    close(server_socket);
    closelog();
    return 0;
}
/*Server child thread for handle child request
 * args thread_message socketnumber for comm to client and the file name of request
 * look at file name is or not in cache 
 * yes read cache 
 * no read pvfs file system then  cache it in cache
 * return  pthread_exit(NULL);
 */
void * handle_client(void * data)
{
    thread_message * message;
    message = (thread_message *) data;
   int new_server_socket=message->net_server_socket;
   pthread_mutex_lock(&mutex);
   Node * p = IsCached(message->file_name);
   if( nullptr == p)
   {
     pvfsread(1,message->file_name);
     CacheIn(message->file_name ,1);
   }
   else
   {     
       
   }
   pthread_mutex_unlock(&mutex);
  close(new_server_socket);
  pthread_exit(NULL);
}
