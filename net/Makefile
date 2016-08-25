#subdir and objects in current dir
SUBDIRS	= 
OBJECTS	= net_work.o \
		  tcp_socket.o \
		  tcp_task_pool.o


all:subdirs ${OBJECTS}

	
clean:cleansubdirs
	rm -f ${OBJECTS}
	
include ${MAKEINCLUDE}
