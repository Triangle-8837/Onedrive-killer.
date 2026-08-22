CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lkernel32 -luser32 -lgdi32 -ladvapi32 -mwindows

TARGET = OneDriveController.exe
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	del *.exe
