default: all

all: debug

debug:
	$(MAKE) -C debug

release:
	$(MAKE) -C release

clean:
	$(MAKE) -C debug clean
	$(MAKE) -C release clean

.PHONY: default all debug release clean