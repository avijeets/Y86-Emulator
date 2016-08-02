#Variable definition
CC  = gcc 
CCFLAGS = -Wall 

#rules definition
all : y86emul

y86emul : y86emul.c y86emul.h
	$(CC) $(CCFLAGS) -o y86emul y86emul.c  

#This rule cleans up executable file
clean: 
	rm -f y86emul