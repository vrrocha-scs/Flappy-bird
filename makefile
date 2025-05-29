CC=g++
CFLAGS=-std=c++11 -Wall -g
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
#OBJETOS A SEREM LINKADOS NO MAIN (COLOQUE TODOS OS .o DAS CLASSES)
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/personagem.o $(OBJ_DIR)/objetorenderizavel.o $(OBJ_DIR)/obstaculo.o
LIBS = -I/X/allegro/5.2.10.1_1/include -L/X/allegro/5.2.10.1_1/lib
all: main 

#$(OBJ_DIR)/nova_classe.o : $(INCLUDE_DIR)/nova_classe.hpp $(SRC_DIR)/nova_classe.cpp $(INCLUDE_DIR)/classe_dependente.hpp (se houver)
#	${CC} ${CFLAGS} -c $(SRC_DIR)/nova_classe.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/nova_classe.o
(OBJ_DIR)/cadastro.o : $(INCLUDE_DIR)/cadastro.hpp $(SRC_DIR)/cadastro.cpp 
	${CC} ${CFLAGS} -c $(SRC_DIR)/cadastro.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/cadastro.o

$(OBJ_DIR)/obstaculo.o : $(INCLUDE_DIR)/obstaculo.hpp $(SRC_DIR)/obstaculo.cpp
	${CC} ${CFLAGS} -c $(SRC_DIR)/obstaculo.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/obstaculo.o

$(OBJ_DIR)/objetorenderizavel.o : $(INCLUDE_DIR)/objetorenderizavel.hpp $(SRC_DIR)/objetorenderizavel.cpp
	${CC} ${CFLAGS} -c $(SRC_DIR)/objetorenderizavel.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/objetorenderizavel.o

$(OBJ_DIR)/personagem.o : $(INCLUDE_DIR)/personagem.hpp $(INCLUDE_DIR)/objetorenderizavel.hpp $(SRC_DIR)/personagem.cpp $(OBJ_DIR)/objetorenderizavel.o
	${CC} ${CFLAGS} -c $(SRC_DIR)/personagem.cpp ${LIBS} -I$(INCLUDE_DIR) -o $(OBJ_DIR)/personagem.o

#ADICIONAR nova_classe.hpp DEPENDENTES AQUI
$(OBJ_DIR)/main.o : $(INCLUDE_DIR)/personagem.hpp $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/obstaculo.hpp $(INCLUDE_DIR)/cadastro.hpp
	${CC} ${CFLAGS} -c $(SRC_DIR)/main.cpp ${LIBS} -o $(OBJ_DIR)/main.o

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/personagem.o $(OBJ_DIR)/obstaculo.o
	${CC} ${CFLAGS} $(OBJS) $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_audio-5 allegro_image-5 allegro_primitives-5) -lallegro_main -lallegro_primitives -lallegro_image -lallegro -o bin/main.exe 

clean: 
	rm -f main ${OBJ_DIR}/*.o 