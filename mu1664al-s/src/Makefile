all:
	make -C libclientserver
	make -C test
	make -C build

# Phony targets
.PHONY: all clean distclean install

# Standard clean
clean:
	make -C libclientserver clean
	make -C test clean
	make -C build clean

distclean:
	make -C libclientserver distclean
	make -C test distclean
	make -C build distclean

install:
	make -C build install
