CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I./include
LDFLAGS = -static
GTEST_LIB = -lgtest -lgtest_main -lpthread

OBJDIR = obj
BINDIR = bin
SRCDIR = src
TESTDIR = tests

SOURCES = $(SRCDIR)/file_queue.cpp
RECEIVER_SRC = $(SRCDIR)/receiver.cpp
SENDER_SRC = $(SRCDIR)/sender.cpp
TEST_SOURCES = $(TESTDIR)/test_queue.cpp $(TESTDIR)/test_validation.cpp

OBJECTS = $(OBJDIR)/file_queue.o
RECEIVER_OBJ = $(OBJDIR)/receiver.o
SENDER_OBJ = $(OBJDIR)/sender.o
TEST_OBJS = $(OBJDIR)/test_queue.o $(OBJDIR)/test_validation.o $(OBJDIR)/file_queue.o

all: $(BINDIR)/receiver.exe $(BINDIR)/sender.exe $(BINDIR)/tests.exe

$(BINDIR)/receiver.exe: $(OBJECTS) $(RECEIVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/sender.exe: $(OBJECTS) $(SENDER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/tests.exe: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(GTEST_LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*.exe

clean-test-files:
	rm -f test_*.bin

.PHONY: all clean clean-test-files
