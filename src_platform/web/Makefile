EMCC = emcc

PLATSRCDIR = ../../src_client/platform_web
CORESRCDIR = ../../src_client/core

OUTDIR = obj
JSSRCS = $(shell find $(PLATSRCDIR) -name "*.js")
CSRCS = $(PLATSRCDIR)/main.c $(shell find $(CORESRCDIR) -name "*.c")
OBJS = $(addprefix $(OUTDIR)/,$(notdir $(CSRCS:.c=.o)))
TARGET = $(OUTDIR)/game.js
DEPEND = $(OUTDIR)/MakeDepends.mk

CFLAGS = -I$(PLATSRCDIR) -I$(CORESRCDIR)
EXFLAGS = -s EXPORTED_FUNCTIONS="$(shell sh $(PLATSRCDIR)/exFuncs.sh)"
JSFLAGS = $(addprefix --js-library , $(JSSRCS))

CFLAGS += -Wall -Wextra -Wpedantic -Werror 
CFLAGS += -Wfloat-equal
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -Wno-missing-braces
CFLAGS += -Wno-char-subscripts
CFLAGS += -Wno-unused-parameter

.PHONY : default debug release build clean

default: debug 

# --------------------------------

debug: build

release: EMCC += -O3
release: clean build

build: $(TARGET)
	mkdir -p bin
	rsync -av $(TARGET) game.html frame.html bin/
	[[ ! -f $(TARGET).mem ]] || rsync -av $(TARGET).mem bin/

clean:
	-rm $(TARGET) $(DEPEND) $(OBJS)
	[[ ! -f $(TARGET).mem ]] || rm $(TARGET).mem

# --------------------------------

$(TARGET): $(DEPEND) $(OBJS) $(JSSRCS)
	$(EMCC) $(CFLAGS) $(EXFLAGS) $(JSFLAGS) -o $@ $(OBJS)

$(DEPEND): $(CSRCS)
	$(EMCC) $(CFLAGS) -MM $(CSRCS) |\
		perl -pe 's/\n/NEWLINE/g' | perl -pe 's/\\NEWLINE  //g' | perl -pe 's/NEWLINE/\n/g' |\
		perl -pe 's/^/$(OUTDIR)\//g' |\
		perl -pe 's/\n/\n\t\$$\(EMCC\) \$$\(CFLAGS\) -o \$$@ \$$\<\n/g' > $(DEPEND)

# --------------------------------

-include $(DEPEND)

# --------------------------------

