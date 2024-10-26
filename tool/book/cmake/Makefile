CC = gcc   
CFLAGS = -Wall -Wextra -g   
LDFLAGS =   
TARGET = bin/myprogram   
SRCDIR = src   
INCDIR = include   
OBJDIR = obj   
SOURCES := $(wildcard $(SRCDIR)/*.c)   
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))   
.PHONY: all clean   
all: $(TARGET)   
$$(TARGET): $$(OBJECTS)   
	$(CC) $(LDFLAGS) $^ -o $@   
$$(OBJDIR)/%.o: $$(SRCDIR)/%.c   
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@   
clean:   
	rm -f $(OBJECTS) $(TARGET)   