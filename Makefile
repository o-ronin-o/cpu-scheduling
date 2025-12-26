CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = lab7
SRC = lab7.cpp
HDR = lab7.h
TESTDIR = testcases

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Running test cases..."
	@for infile in $(TESTDIR)/*.in; do \
		name=$$(basename $$infile .in); \
		outfile=$(TESTDIR)/$$name.out; \
		echo "==> $$name"; \
		./$(TARGET) < $$infile > tmp.out; \
		if diff -q tmp.out $$outfile > /dev/null; then \
			echo "PASS"; \
		else \
			echo "FAIL"; \
			diff tmp.out $$outfile; \
		fi; \
		echo ""; \
	done
	@rm -f tmp.out

clean:
	rm -f $(TARGET) tmp.out
