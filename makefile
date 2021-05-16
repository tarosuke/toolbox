target := libtoolbox.a

all: $(target)

.PHONY : clean test watch uninstall
#.DELETE_ON_ERROR : $(wildcard .builds/*)


############################################################ FILE RECOGNITIONS

COPTS ?= -Iinclude -I/usr/include/gdbm

COPTS += -Wall -Werror -g -IX11
CCOPTS += $(COPTS) -std=c++11

suffixes := %.c %.cc %.glsl

files := $(subst sources/,, $(shell find sources -type f))
srcs := $(filter $(suffixes), $(files))
mods := $(basename $(srcs))
objs := $(addprefix .builds/, $(addsuffix .o, $(mods)))
deps := $(addprefix .builds/, $(addsuffix .dep, $(mods)))





################################################################# COMMON RULES


-include $(deps)

vpath %.o .builds
vpath % $(dirs)


.builds/%.o : sources/%.cc makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CCOPTS) -c -o $@ $<

.builds/%.o : sources/%.c makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@${CC} $(COPTS) -c -o $@ $<

.builds/%.o : sources/%.glsl makefile
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

.builds/%.dep : sources/%.cc makefile
	@echo " CPP $@"
	@mkdir -p $(dir $@)
	@echo -n .builds/ > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

.builds/%.d : sources/%.c makefile
	@echo " CPP $@"
	@echo -n .builds/ > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@

%.a: makefile $(objs)
	@echo " AR $@"
	@ar rc $@ $(objs)



############################################################### RULES & TARGET

install: libtoolbox.a
	@sudo cp libtoolbox.a /usr/local/lib
	@sudo cp -a include/toolbox /usr/local/include

uninstall:
	@sudo rm -f /usr/local/lib/libtoolbox.a
	@sudo rm -rf  /usr/local/include/toolbox

clean:
	rm -rf .builds/* libtoolbox.a $(shell find . -name "*.orig")

test: libtoolbox.a $(testObjs)
	$(foreach m, $(testMods), $(shell echo "gcc -o $(m) .builds/$(m).o"))
