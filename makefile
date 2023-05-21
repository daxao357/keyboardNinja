# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Директории
SRCDIR = src
OBJDIR = obj/main
BINDIR = bin
TESTDIR = test
TESTOBJDIR = obj/test

# Исходные файлы
SRCS = $(wildcard $(SRCDIR)/*.cpp)
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)

# Объектные файлы
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TESTOBJS = $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TESTSRCS))

# Исполняемые файлы
EXEC = $(BINDIR)/program
TESTEXEC = $(BINDIR)/test

# Правило по умолчанию
all: $(EXEC)

# Правило для сборки исполняемого файла
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Правило для компиляции исходных файлов программы
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для сборки тестового исполняемого файла
$(TESTEXEC): $(TESTOBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Правило для компиляции исходных файлов тестов
$(TESTOBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для запуска программы
run: $(EXEC)
	$(EXEC)

# Правило для запуска тестов
test: $(TESTEXEC)
	$(TESTEXEC)

# Правило для очистки сгенерированных файлов
clean:
	rm -rf $(OBJDIR)/*.o $(TESTOBJDIR)/*.o $(EXEC) $(TESTEXEC)

.PHONY: all clean run test
