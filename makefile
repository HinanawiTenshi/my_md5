CC := g++
INCLUDE := include
SRC := src
BUILD := build
BIN := bin
CFLAG = -I
EXE = my_md5

run: build
	@echo "EXECUTING..."
	@echo ""
	@bin/my_md5

build: $(BUILD)/main.o $(BUILD)/bit_string.o $(BUILD)/my_md5.o
	@echo "BUILDING..."
	@mkdir -p $(BIN)
	@$(CC) -o $(BIN)/$(EXE) $^

$(BUILD)/%.o: $(SRC)/%.cpp
	@echo "COMPILING..."
	@mkdir -p $(BUILD)
	@$(CC) -o $@ -c $^ $(CFLAG) $(INCLUDE)

clean:
	@rm -r -f $(BUILD) $(BIN)

all: clean run
