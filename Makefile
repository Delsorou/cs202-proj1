# ------------------------------------------------------------------------------
# FILE NAME:    Makefile
#
# DESCRIPTION:  Makefile for Spring 2017 CS202 Project 1: Bank Account
#
# PURPOSE:      Build the Bank Account project
#
# MODIFICATION HISTORY:
#
# Author        Date          Modification(s)
# -----------   ----------    ---------------
# Aaryna Irwin  2017-02-21    0.1 - Initial version
# ------------------------------------------------------------------------------

CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=banking.cpp account.cpp aseek.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bankacct

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o :
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm $(OBJECTS) $(EXECUTABLE)
