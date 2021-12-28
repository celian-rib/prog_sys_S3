TARGET = main

CC = gcc
CFLAGS = --std=c11 -g -Wall -Wextra -Wunused -pedantic -D_XOPEN_SOURCE=700

SOURCEDIR = ./src
SUBDIR = $(shell find $(SOURCEDIR) -maxdepth 1 -type d)
DIR_OBJ = ./obj

HEADERS = $(wildcard *.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
SOURCES = $(wildcard *.c $(foreach fd, $(SUBDIR), $(fd)/*.c))

OBJECTS = $(addprefix $(DIR_OBJ)/, $(SOURCES:c=o))
INC_DIRS = -I./ $(addprefix -I, $(SUBDIR))

PHONY := $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

$(DIR_OBJ)/%.o: %.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< $(INC_DIRS)

PHONY += clean
clean:
	rm -rf $(TARGET) $(DIR_OBJ)/*

PHONY += debug
debug:
	@echo "HEADERS: $(HEADERS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "INC_DIRS: $(INC_DIRS)"
	@echo "SOURCEDDIR: $(SOURCEDIR)"
	@echo "SUBDIR: $(SUBDIR)"

.PHONY = $(PHONY)
