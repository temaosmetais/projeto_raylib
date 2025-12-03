CC = gcc

SRC = src/main.c src/utils.c src/screens.c
CFLAGS = -I include/ \
		 -Wall \
		 -std=c99

ifeq ($(OS),Windows_NT)
    TARGET = app.exe
    
    LIB_PATH = lib/windows
    
    LDFLAGS = -L $(LIB_PATH) \
			  -lraylib \
			  -lopengl32 \
			  -lgdi32 \
			  -lwinmm
    
    CLEAN_CMD = del /Q
    RUN_CMD = $(TARGET)
else
    TARGET = app
    UNAME_S := $(shell uname -s)
    
    ifeq ($(UNAME_S),Darwin)
        LIB_PATH = lib/macos
        
        LDFLAGS = -L $(LIB_PATH) \
				  -lraylib \
				  -framework CoreVideo \
				  -framework IOKit \
				  -framework Cocoa \
				  -framework GLUT \
				  -framework OpenGL
    endif
    
    CLEAN_CMD = rm -f
    RUN_CMD = ./$(TARGET)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: all
	$(RUN_CMD)

clean:
	$(CLEAN_CMD) $(TARGET)

.PHONY: all run clean
