CC = clang
CFLAGS = -W -Wall
LDFLAGS = -lfftw3 -lm
 


SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
AOUT = out
 
all : out
 
out : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
clean :
	@rm *.o
cleaner : clean
	@rm $(AOUT)