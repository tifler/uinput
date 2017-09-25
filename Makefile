
TARGET			:= uinput
OBJS			:= uinput.o

CC              := gcc
CXX             := g++
CFLAGS			:= -Wall -pipe

.PHONY:	clean

all:	$(TARGET)

clean:
	@rm $(TARGET) $(OBJS)

$(TARGET):	$(OBJS)
	$(CC) -o $@ $^ $(LFALGS)

