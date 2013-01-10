#
# TrackInsanity Makefile
#
SRCDIR=src
BINDIR=bin
SRCS=$(SRCDIR)/tiTiles.c \
	 $(SRCDIR)/tiBoard.c \
	 $(SRCDIR)/tiPlayer.c \
	 $(SRCDIR)/tiGame.c \
	 $(SRCDIR)/tiCoords.c \
	 $(SRCDIR)/tiRenderSDL.c \
	 $(SRCDIR)/tiComputerAI.c \
	 $(SRCDIR)/tiMain.c
OBJS=$(SRCDIR)/tiTiles.o \
	 $(SRCDIR)/tiBoard.o \
	 $(SRCDIR)/tiPlayer.o \
	 $(SRCDIR)/tiGame.o \
	 $(SRCDIR)/tiCoords.o \
	 $(SRCDIR)/tiRenderSDL.o \
	 $(SRCDIR)/tiComputerAI.o \
	 $(SRCDIR)/tiMain.o
CC=gcc
CFLAGS=-O2
LDFLAGS=


all:	trackInsanity

nokia:  CFLAGS=-O2 -D_NOKIA_N800_
nokia:	trackInsanity

$(SRCS):
	$(CC) $(CFLAGS) -c $*.c
	
trackInsanity: $(OBJS)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$@ $(OBJS) -lSDL -lSDL_image
	
clean:
	-rm -f trackInsanity *~ *.o *.bak $(SRCDIR)/*~ $(SRCDIR)/*.o $(SRCDIR)*.bak core $(BINDIR)/trackInsanity $(BINDIR)/core* $(BINDIR)/*~ $(BINDIR)/data/*~

	
	

	
