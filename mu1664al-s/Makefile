all:
	make -C src
	mkdir -p ./database

# Phony targets
.PHONY: all clean distclean install

# Standard clean
clean:
	make -C src clean
	rm -rf ./database/*

distclean:
	make -C src distclean
	rm -rf ./bin/*
	rm -rf ./database

install:
	make -C src install
	rm -rf ./database
	mkdir ./database
