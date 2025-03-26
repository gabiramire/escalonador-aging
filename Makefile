# Nome do compilador
CXX = g++

# Opções de compilação
CXXFLAGS = -Wall -O2

# Nome do executável
TARGET = escalonador.exe

# Arquivos-fonte
SRCS = main.cpp Escalonador.cpp

# Regra padrão: compilar o programa
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Limpar arquivos gerados
clean:
	del /F /Q $(TARGET)
