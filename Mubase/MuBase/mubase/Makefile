#target: dependencies
#[tab] system command

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include
INPUT = input
TEMP = bin
CFLAGS = -c -I$(INCDIR)
CC = g++
CP = cp

all: $(BINDIR)/exec

$(BINDIR)/exec	:	$(OBJDIR)/dm.o \
		$(OBJDIR)/bm.o \
		$(OBJDIR)/sm.o \
		$(OBJDIR)/smutil.o \
		$(OBJDIR)/bt.o \
		$(OBJDIR)/test.o
		$(CC) $(OBJDIR)/dm.o $(OBJDIR)/bm.o $(OBJDIR)/sm.o $(OBJDIR)/smutil.o $(OBJDIR)/bt.o $(OBJDIR)/test.o -o $(BINDIR)/exec -I$(INCDIR)
		$(CP) $(INPUT)/test.txt $(TEMP)
	
$(OBJDIR)/dm.o	:	$(SRCDIR)/dm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/dm.o $(SRCDIR)/dm.cpp 

$(OBJDIR)/bm.o	:	$(SRCDIR)/bm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/bm.o $(SRCDIR)/bm.cpp
		
$(OBJDIR)/sm.o	:	$(SRCDIR)/sm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/sm.o $(SRCDIR)/sm.cpp

$(OBJDIR)/smutil.o	:	$(SRCDIR)/smutil.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/smutil.o $(SRCDIR)/smutil.cpp
		
				
$(OBJDIR)/bt.o	:	$(SRCDIR)/bt.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/bt.o $(SRCDIR)/bt.cpp
		
$(OBJDIR)/test.o	:	$(SRCDIR)/test.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/test.o $(SRCDIR)/test.cpp
		
clean:
		rm -rf $(OBJDIR)/*.o $(BINDIR)/exec $(BINDIR)/test.txt 
