	# Se for um projeto C++, use g++. Se for C puro, use gcc e renomeie main.cpp para main.c
	CC = g++
	CFLAGS = -Wall -Wextra -std=c++11 -g # Usando std::c++11 para C++
	LDFLAGS =
	
	SRCDIR = src
	OBJDIR = obj
	BINDIR = .
	
	# Usamos vpath para que make possa encontrar os arquivos .c e .h
	# Makefiles podem ter dificuldade em misturar .c e .cpp facilmente na mesma compilação com g++
	# A melhor prática é compilar .c com gcc e .cpp com g++, mas para simplificar, g++ pode compilar ambos.
	vpath %.c $(SRCDIR)
	vpath %.h $(SRCDIR)
	
	# Arquivos fonte (agora incluindo todos os .c e o main.cpp)
	SRCS = $(SRCDIR)/cliente.c \
	       $(SRCDIR)/lista_clientes.c \
	       $(SRCDIR)/io.c \
	       $(SRCDIR)/banco.c \
	       main.cpp # main.cpp está na raiz
	
	# Arquivos objeto no diretório OBJDIR
	# Esta linha gerará os .o para os .c em src e para o main.cpp
	OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter $(SRCDIR)/%.c, $(SRCS))) \
	       $(patsubst %.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SRCS)))
	
	# Nome do executável
	TARGET = $(BINDIR)/banco
	
	.PHONY: all clean
	
	all: $(TARGET)
	
	# Regra para compilar o executável final
	$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	
	# Regra genérica para compilar arquivos .c em .o para os que estão em SRCDIR
	$(OBJDIR)/%.o: $(SRCDIR)/%.c
		@mkdir -p $(OBJDIR)
		$(CC) $(CFLAGS) -c $< -o $@
	
	# Regra específica para main.cpp que está na raiz do projeto
	$(OBJDIR)/main.o: main.cpp
		@mkdir -p $(OBJDIR)
		$(CC) $(CFLAGS) -c $< -o $@
	
	clean:
		rm -rf $(OBJDIR) $(TARGET) data/*.txt
		@echo "Diretórios de objeto e executável limpos, arquivos de dados removidos."
	
	# Dependências (garante que os .o são recompilados se os .h mudarem)
	# Note que os .h são procurados via vpath
	$(OBJDIR)/cliente.o: cliente.h
	$(OBJDIR)/lista_clientes.o: lista_clientes.h cliente.h
	$(OBJDIR)/io.o: io.h
	$(OBJDIR)/banco.o: banco.h lista_clientes.h cliente.h io.h
	$(OBJDIR)/main.o: banco.h io.h cliente.h
