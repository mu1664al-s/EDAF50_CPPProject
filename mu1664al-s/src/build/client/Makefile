# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS  = -std=c++11 -I ../../libclientserver
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS   = -g -L ../../libclientserver
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Libraries
LDLIBS = -lclientserver

# Targets
PROGS = deps client

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
# The dependency on libclientserver.a is not defined.
deps:
	make -C ../../libclientserver
client: client.o

SRC = $(wildcard *.cc)

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f $(SRC:.cc=.o) $(PROGS)


distclean: clean
	-rm $(SRC:.cc=.d)
	-rm -r $(addsuffix .dSym,$(PROGS)) # for cleaning on Macs

install:
	cp client ../../../bin

# Include the *.d files
-include $(SRC:.cc=.d)
