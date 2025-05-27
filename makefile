CC=g++
CFLAGS=-std=c++11 -Wall -g
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
LIBS = -I/X/allegro/5.2.10.1_1/include -L/X/allegro/5.2.10.1_1/lib -lallegro_main -lallegro_primitives -lallegro
all: main 

#$(OBJ_DIR)/nova_classe.o : $(INCLUDE_DIR)/nova_classe.hpp $(SRC_DIR)/nova_classe.cpp $(INCLUDE_DIR)/classe_dependente.hpp (se houver)
#	${CC} ${CFLAGS} -c $(SRC_DIR)/nova_classe.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/nova_classe.o

$(OBJ_DIR)/personagem.o : $(INCLUDE_DIR)/personagem.hpp $(SRC_DIR)/personagem.cpp
	${CC} ${CFLAGS} -c $(SRC_DIR)/personagem.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/personagem.o

#ADCIONAR nova_classe.hpp DEPENDENTES AQUI
$(OBJ_DIR)/main.o : $(INCLUDE_DIR)/personagem.hpp $(SRC_DIR)/main.cpp
	${CC} ${CFLAGS} -c $(SRC_DIR)/main.cpp ${LIBS} -o $(OBJ_DIR)/main.o

#ADCIONAR nova_classe.o DEPENDENTES AQUI
main: $(OBJ_DIR)/main.o $(OBJ_DIR)/personagem.o
	${CC} ${CFLAGS} $(OBJ_DIR)/main.o $(OBJ_DIR)/personagem.o ${LIBS} -o bin/main.exe

clean: 
	rm -f main ${OBJ_DIR}/*.o 