# subdir and objects in current dir
SUBDIRS	= 
OBJECTS	= buffer.o \
		  ipc_sem.o \
		  ipc_shm.o \
		  socket_util.o \
		  timex.o


all:subdirs ${OBJECTS}

	
clean:cleansubdirs
	rm -f ${OBJECTS}
	
include ${MAKEINCLUDE}
