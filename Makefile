CXX			:= g++
CXX_FLAGS		:= -Wall
OPENSSL_FLAGS		:= -lssl -lcrypto
LIBPQXX_FLAGS		:= -lpqxx -lpq

SERVER_EXE		:= rpiserver
CLIENT_EXE		:= rpiclient
SERVERO			:= server.o
CLIENTO			:= client.o

SRCDIR		 	:= ./src
OBJDIR 			:= ./obj
BINDIR			:= ./bin
SUBDIRS			:= $(wildcard $(SRCDIR)/*)
SRC_FILES		:= $(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS			:= $(foreach dir,$(SUBDIRS),$(patsubst $(dir)/%.cpp,$(OBJDIR)/%.o,$(filter $(dir)/%.cpp, $(SRC_FILES))))

vpath %.cpp $(SUBDIRS)

all: server client

server: $(filter-out $(OBJDIR)/$(CLIENTO), $(OBJS))
	@echo -SERVER-
	$(CXX) $(CXX_FLAGS) $^ -o $(BINDIR)/$(SERVER_EXE) $(OPENSSL_FLAGS) $(LIBPQXX_FLAGS)

client: $(filter-out $(OBJDIR)/$(SERVERO), $(OBJS))
	@echo -CLIENT-
	$(CXX) $(CXX_FLAGS) $^ -o $(BINDIR)/$(CLIENT_EXE) $(OPENSSL_FLAGS) $(LIBPQXX_FLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/*
