# Makefile pour Glut
#
# CC-BY Edouard.Thiel@univ-amu.fr - 12/12/2018
#
# sudo apt-get install freeglut3-dev libgl1-mesa-dev

SHELL   = /bin/bash
CC      = gcc
RM      = rm -f
CFLAGS  = -Wall -std=c99
LIBS    = -lglut -lGLU -lGL -lm

CFILES  := $(wildcard *.c)
EXECS   := $(CFILES:%.c=%)

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c

all :: $(EXECS)

$(EXECS) : % : %.o
	$(CC) -o $@ $^ $(LIBS)

clean ::
	$(RM) *.o *~ $(EXECS)

