CC = gcc
CFLAGS = -Wall
TARGET = web_client
HEADER_FILES = http.h
OBJECT_FILES = $(TARGET).o http.o

# This built-in target with no dependencies causes intermediate
# files not to be removed.
.SECONDARY:

all: $(TARGET)

# Link.
$(TARGET): $(OBJECT_FILES)
	$(CC) $^ -o $@

# Preprocess, compile and assemble the main file (separately,
# bacause it depends on all header files).
$(TARGET).o: $(TARGET).c $(HEADER_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

# Preprocess, compile and assemble the other files.
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.i *.s *.o $(TARGET)
