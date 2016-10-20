# Makefile for Saliency using libsaliency.a

.IGNORE:

.KEEP_STATE:

.SUFFIXES: .o .c

.c.o: 
	$(CC) $(CCFLAGS) -c $< -o $@

CC = gcc

CCFLAGS += -g -Wall

LIB_PNGDEC = libpngdec.a
LIB_Z = libz.a
BIN = pngdec

SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c=.o)

SUBDIR_PNGDEC = "../png_decoder_lib"

all: $(LIB_PNGDEC) $(BIN)

$(BIN): $(OBJS) $(LIB_PNGDEC) 
	$(CC) $(CCFLAGS) -o $(BIN) $(OBJS) -I . -L . $(LIB_PNGDEC) $(LIB_Z)

$(LIB_PNGDEC): 
	@cd $(SUBDIR_PNGDEC) ; make
	
clean:
	rm -rf $(OBJS) $(BIN) $(LIB_PNGDEC) $(BIN).gdb
	cd $(SUBDIR_PNGDEC); make clean


