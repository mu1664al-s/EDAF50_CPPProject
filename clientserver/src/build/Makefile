all:
	make -C ../libclientserver
	make -C client
	make -C server

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	make -C client clean
	make -C server clean

distclean:
	make -C client distclean
	make -C server distclean

install:
	make -C server install
	make -C client install
