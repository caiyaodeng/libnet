.SUFFIXES:.cpp.o 
CC=clang++ 	
OBJK1=$(SRC1:.cpp=.o)
EXEC1=server
SRC1=main.cpp \
	 tcp_task_pool.cpp \
	 net_work.cpp \
	 ../base/buffer.cpp \
	 ../base/socket_util.cpp \
	 tcp_socket.cpp

start:$(OBJK1) 
	$(CC) -o $(EXEC1) $(OBJK1) -lpthread
	@echo ---------ok------- 

.cpp.o:$(SRC1)
	$(CC) -g -Wall -std=c++11 -o $@ -c $<  
clean:  
	rm -f $(OBJK1) 
	rm -f $(EXEC1) 
	rm -f core.* 
