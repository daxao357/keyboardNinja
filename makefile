# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Директории
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Исходные файлы
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Объектные файлы
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Исполнительный файл
BIN = $(BIN_DIR)/keyboardNinja

# Правило для компиляции объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для сборки исполнительного файла
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Правило "all" для сборки исполнительного файла
all: $(BIN)

# Правило "run" для запуска исполнительного файла
run: $(BIN)
	$(BIN)

# Правило "clean" для очистки объектных и исполнительных файлов
clean:
	rm -f $(OBJS) $(BIN)
