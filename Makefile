DIR = $(shell pwd)

default: web

# --------------------------------

clean:
	$(MAKE) -C src_platform/web clean

# --------------------------------

web:
	$(MAKE) -C src_platform/web build

# --------------------------------

