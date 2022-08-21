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
		$(OBJDIR)/sputil.o \
		$(OBJDIR)/smutil.o \
		$(OBJDIR)/schema.o \
		$(OBJDIR)/record.o \
		$(OBJDIR)/bt.o \
		$(OBJDIR)/relation.o \
		$(OBJDIR)/sp.o \
		$(OBJDIR)/base_rel_iter.o \
		$(OBJDIR)/iterator.o \
		$(OBJDIR)/sort.o \
		$(OBJDIR)/minheap.o \
		$(OBJDIR)/main.o 
		$(CC) $(OBJDIR)/dm.o $(OBJDIR)/bm.o $(OBJDIR)/sm.o $(OBJDIR)/sputil.o $(OBJDIR)/smutil.o $(OBJDIR)/schema.o $(OBJDIR)/record.o $(OBJDIR)/bt.o $(OBJDIR)/relation.o $(OBJDIR)/sp.o $(OBJDIR)/base_rel_iter.o $(OBJDIR)/iterator.o $(OBJDIR)/sort.o $(OBJDIR)/minheap.o $(OBJDIR)/main.o  -o $(BINDIR)/exec -I$(INCDIR)
		$(CP) $(INPUT)/populate.dat $(TEMP)
	
$(OBJDIR)/dm.o	:	$(SRCDIR)/dm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/dm.o $(SRCDIR)/dm.cpp 

$(OBJDIR)/bm.o	:	$(SRCDIR)/bm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/bm.o $(SRCDIR)/bm.cpp
		
$(OBJDIR)/sm.o	:	$(SRCDIR)/sm.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/sm.o $(SRCDIR)/sm.cpp

$(OBJDIR)/sputil.o	:	$(SRCDIR)/sputil.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/sputil.o $(SRCDIR)/sputil.cpp

$(OBJDIR)/smutil.o	:	$(SRCDIR)/smutil.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/smutil.o $(SRCDIR)/smutil.cpp

$(OBJDIR)/schema.o	:	$(SRCDIR)/schema.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/schema.o $(SRCDIR)/schema.cpp

$(OBJDIR)/record.o	:	$(SRCDIR)/record.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/record.o $(SRCDIR)/record.cpp
				
$(OBJDIR)/bt.o	:	$(SRCDIR)/bt.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/bt.o $(SRCDIR)/bt.cpp

$(OBJDIR)/relation.o	:	$(SRCDIR)/relation.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/relation.o $(SRCDIR)/relation.cpp
		
$(OBJDIR)/sp.o	:	$(SRCDIR)/sp.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/sp.o $(SRCDIR)/sp.cpp

$(OBJDIR)/base_rel_iter.o	:	$(SRCDIR)/base_rel_iter.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/base_rel_iter.o $(SRCDIR)/base_rel_iter.cpp

$(OBJDIR)/iterator.o	:	$(SRCDIR)/iterator.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/iterator.o $(SRCDIR)/iterator.cpp

$(OBJDIR)/sort.o	:	$(SRCDIR)/sort.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/sort.o $(SRCDIR)/sort.cpp

$(OBJDIR)/minheap.o	:	$(SRCDIR)/minheap.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/minheap.o $(SRCDIR)/minheap.cpp

$(OBJDIR)/main.o	:	$(SRCDIR)/main.cpp
		$(CC) $(CFLAGS) -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp
		
clean:
		rm -rf $(OBJDIR)/*.o $(BINDIR)/exec $(BINDIR)/populate.dat 
