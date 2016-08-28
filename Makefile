default: all

all: debug

test:
	$(MAKE) -C tests

debug:
	$(MAKE) -C debug

release:
	$(MAKE) -C release

clean:
	$(MAKE) -C tests clean
	$(MAKE) -C debug clean
	$(MAKE) -C release clean

.PHONY: default all test debug release clean
