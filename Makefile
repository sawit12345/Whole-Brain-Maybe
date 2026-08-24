CXX      := g++
CXXFLAGS := -O3 -std=c++20 -Wall -Wextra -mpopcnt

BUILD := build
HDRS  := src/common/bitvec.hpp src/common/rng.hpp src/sdr_baseline/sdr.hpp
OBJS  := $(BUILD)/bitvec.o $(BUILD)/sdr.o

.PHONY: all test bench clean

all: $(BUILD)/test_common $(BUILD)/test_sdr $(BUILD)/bench_sdr

test: all
	./$(BUILD)/test_common
	./$(BUILD)/test_sdr

bench: $(BUILD)/bench_sdr
	./$(BUILD)/bench_sdr

clean:
	rm -rf $(BUILD)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/bitvec.o: src/common/bitvec.cpp $(HDRS) | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/sdr.o: src/sdr_baseline/sdr.cpp $(HDRS) | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/test_common: tests/test_common.cpp $(BUILD)/bitvec.o
	$(CXX) $(CXXFLAGS) $< $(BUILD)/bitvec.o -o $@

$(BUILD)/test_sdr: tests/test_sdr.cpp $(BUILD)/sdr.o $(BUILD)/bitvec.o
	$(CXX) $(CXXFLAGS) $< $(BUILD)/sdr.o $(BUILD)/bitvec.o -o $@

$(BUILD)/bench_sdr: bench/bench_sdr.cpp $(BUILD)/sdr.o $(BUILD)/bitvec.o
	$(CXX) $(CXXFLAGS) $< $(BUILD)/sdr.o $(BUILD)/bitvec.o -o $@
