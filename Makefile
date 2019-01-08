# Pouzity kompiler
CC=g++
# Nastavenia kompileru
CFLAGS=-std=c++11 -w -pedantic

# Nazov projektu
#OUTPUT_DIR = output
TARGET=TCPClient

# Zoznam nalinkovanych kniznic
LIBS=\
		-pthread

# Zoznam hlavickovych suborov
HEADERS=\
		sockets/ServerSocket.h \
		sockets/ClientSocket.h \

# Zoznam zdrojovych suborov
SOURCES=\
		sockets/ServerSocket.cpp \
		sockets/ClientSocket.cpp \
		client.cpp \

.PHONY: binary all clear

build: $(TARGET)

binary: build
		mkdir -p bin
		mv $(TARGET) bin/$(TARGET)

clear:
		rm -R bin

run:
		./bin/$(TARGET)

# Pravidlo pre kompilaciu
$(TARGET): $(HEADERS) $(SOURCES)
		$(CC) $(HEADERS) $(SOURCES) $(LIBS) $(CFLAGS) -o $(TARGET)
		$(BUILD_DIR)