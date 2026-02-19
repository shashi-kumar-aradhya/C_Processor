CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -I.

SRC = \
main.cpp \
phase1/Phase1Processor.cpp \
lexer/Lexer.cpp \
preprocessor/MacroTable.cpp \
preprocessor/ConditionalStack.cpp \
preprocessor/ExpressionParser.cpp \
preprocessor/Preprocessor.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = cpp_frontend

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

