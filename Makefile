CC = gcc
GP = g++
INCLUDE = /usr/local/pvfs/include
LIBPATH = /usr/local/pvfs/lib
CFLAGS = -I$(INCLUDE) -L$(LIBPATH)
CXXFLAGS = $(CFLAGS)
EXES = read write
OBJS = read.o write.o
.PHONY : all  clean dynamic test  testpvfs pvfs help server
help :
	@echo "      make all :create pvfs read and write and pvfsapi.so "
	@echo "      make test : for test the log function open close write log "
	@echo "      make server : for cache daemon process"
all : $(EXES)  dynamic

$(EXES) : % : %.o
	@$(CC) $(CFLAGS) -o $@ $< -lpvfs2 -lpthread -lcrypto

$(OBJS) : %.o : %.c
	@$(CC) $(CFLAGS) -c $<
clean : 
	@-rm $(EXES) $(OBJS) 
dynamic : pvfsapi.c pvfsapi.h
	@$(CC)  $(CFLAGS) -c   pvfsapi.c -lpvfs2 -lpthread -lcrypto
pvfs : pvfsapi.c pvfsapi.h
	@$(CC)  $(CFLAGS) -o pvfsapi.o  pvfsapi.c -lpvfs2 -lpthread -lcrypto

test :  test.*  cache_log.*
	@$(CC)  -std=c++0x -Istdc++ $(CFLAGS) -o  test test.cpp pvfsapi.c cache_log.cpp  cache_alg.cpp -g -lpvfs2 -lpthread -lcrypto
testpvfs : test.cpp  pvfsapi.c
	@$(CC)    $(CFLAGS)  -o test test.cpp  pvfsapi.c  -lpvfs2 -lpthread -lcrypto
server : server.cpp
	@$(CC)  -std=c++0x -Istdc++ $(CFLAGS) -o server server.cpp  pvfsapi.c cache_log.cpp cache_alg.cpp -lpvfs2 -lpthread -lcrypto -g
	
