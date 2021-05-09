target=libtoolbox.a

all: $(target)

.PHONY : clean test watch uninstall
.DELETE_ON_ERROR : $(wildcard objs/*)


####################################################################### VALUES

COPTS ?= -Iinclude -I/usr/include/gdbm

COPTS += -Wall -Werror -g -IX11
CCOPTS += $(COPTS) -std=c++11

files := $(shell find . -type d -name test -prune -o -type f -print)
srcs := $(filter %.c %.cc %.glsl, $(files))
hdrs := $(filter %.h, $(files))

dirs = $(sort $(dir $(srcs)))
mods = $(basename $(notdir $(srcs)))
dmds= $(addprefix objs/, $(mods))
objs = $(addsuffix .o, $(dmds)) $(exobjs)
deps = $(addsuffix .d, $(dmds))

exobjs =


testFiles := $(shell find $(shell find . -type d -name test) -type f)



######################################################################## RULES

libtoolbox.a: makefile $(objs)
	@echo " AR $@"
	@ar rc $@ $(objs)

install: libtoolbox.a
	@sudo cp libtoolbox.a /usr/local/lib
	@sudo cp -a include/toolbox /usr/local/include

uninstall:
	@sudo rm -f /usr/local/lib/libtoolbox.a
	@sudo rm -rf  /usr/local/include/toolbox

clean:
	rm -f objs/* libtoolbox.a $(wildcard */*.orig */*/*.orig)

test:
	@echo $(testFiles)


################################################################# COMMON RULES


-include $(deps)

vpath %.o objs
vpath % $(dirs)


objs/%.o : %.cc makefile
	@echo " CC $@"
	@$(CC) $(CCOPTS) -c -o $@ $<

objs/%.o : %.c makefile
	@echo " CC $@"
	@${CC} $(COPTS) -c -o $@ $<

objs/%.o : %.glsl makefile
	@echo " OBJCOPY $@"
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

objs/%.d : %.cc
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

objs/%.d : %.c
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(COPTS) -MM $< >> $@
