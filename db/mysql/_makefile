.SUFFIXES: .cpp .o

CC=g++
SRCS=main.cpp\
	 db_mysql.cpp
	
OBJS=$(SRCS:.cpp=.o)
EXEC=test

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -I/usr/include/mysql  -L/usr/lib64/mysql -lmysqlclient 
	@echo '-------------ok--------------'

.cpp.o:
	$(CC) -Wall -std=c++11 -g -o $@ -c $<  

clean:
	rm -f $(OBJS)
	rm $(EXEC)
	rm -f core*

