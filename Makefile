# Nome do compilador
CXX = g++

# Opções de compilação (-Wall ativa avisos, -O2 otimiza o código)
CXXFLAGS = -Wall -O2

# Nome do executável
TARGET = processo

# Lista de arquivos-fonte
SRCS = processo.cpp

# Regra padrão: compilar o programa
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Limpar arquivos gerados
clean:
	del $(TARGET).exe
