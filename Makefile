CC = g++
CXXFLAGS = -Wall -Wextra -pg -ggdb
LDFLAGS = -ggdb
LDLIBS = -lm -lgsl -lgslcblas -lstdc++ 

sources = binotree.C bs.C 

all: binotree 
clean:
	rm -f *.o *.d

binotree: binotree.o bs.o

include $(sources:.C=.d)

%.d: %.C
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

