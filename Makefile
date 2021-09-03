.PHONY:clean build all

EMULATOR = fami

CC = g++
G++_OPTIONS = -Wall -std=c++11 -g -O0  -I ${INCLUDE_DIR}
G++_OPTIONS += -lSDL2

SOURCE_DIR      = ./src/
INCLUDE_DIR     = ./include/

vpath %.cpp ${SOURCE_DIR}

HEADERS = $(wildcard ${SOURCE_DIR}*.h)
SOURCES = $(wildcard ${SOURCE_DIR}*.cpp)
OBJECTS = ${SOURCES:.cpp=.o}

all:${EMULATOR}

%.o : %.cpp ${HEADERS}
	${CC} ${G++_OPTIONS} -o $@ -c $<

${EMULATOR} : ${OBJECTS} 
	${CC} ${LD_FLGS} ${G++_OPTIONS} -o ${EMULATOR} $^

clean :
	rm ${EMULATOR}
	rm ${SOURCE_DIR}*.o