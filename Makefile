TARGET   := twc

CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -pedantic

LINKER   := gcc -o
LFLAGS   := 

# change these to set the proper directories where each files shoould be
SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       := rm -rf


$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(LINKER) $@ $(OBJECTS) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	$(rm) $(OBJECTS)

.PHONEY: remove
remove: clean
	$(rm) $(BINDIR)/$(TARGET)

.PHONEY: mrproper
mrproper:
	$(rm) $(BINDIR) $(OBJDIR)