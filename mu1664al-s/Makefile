all:
	make -C src

# Phony targets
.PHONY: all clean distclean install

# Standard clean
clean:
	make -C src clean

distclean:
	rm bin/* bin/.keep
	make -C src distclean

install:
	make -C src install
