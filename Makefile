CC      = gcc
CFLAGS  = -O2 -Wall -Wextra
LDFLAGS = -lwiringPi -lm

TARGET  = monitor
SRCS    = main.c systemMonitor.c ili9486.c

.PHONY: all run clean

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)
	@echo "Build OK → sudo ./$(TARGET)"

run: all
	sudo ./$(TARGET)

clean:
	rm -f $(TARGET)