CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I./includes
LDFLAGS = -static

OBJDIR = obj
BINDIR = bin
SRCDIR = src

OBJECTS = $(OBJDIR)/file_queue.o
RECEIVER_OBJ = $(OBJDIR)/receiver.o
SENDER_OBJ = $(OBJDIR)/sender.o

all: $(OBJDIR) $(BINDIR) $(BINDIR)/receiver.exe $(BINDIR)/sender.exe

run: all
	cd $(BINDIR) && receiver.exe

$(OBJDIR):
	if not exist $(OBJDIR) mkdir $(OBJDIR)

$(BINDIR):
	if not exist $(BINDIR) mkdir $(BINDIR)

$(BINDIR)/receiver.exe: $(OBJECTS) $(RECEIVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/sender.exe: $(OBJECTS) $(SENDER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	if exist $(BINDIR) rmdir /s /q $(BINDIR)

.PHONY: all clean run