-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )


all: $(target)

.PHONY : clean test mtest watch uninstall


############################################################ FILE RECOGNITIONS

COPTS ?= -Iinclude -I/usr/include/gdbm

COPTS += -O0 -Wall -Werror -g -IX11
CCOPTS += $(COPTS) -std=c++11

suffixes := %.c %.cc %.glsl

files := $(subst sources/,, $(shell find sources -type f))
srcs := $(filter $(suffixes), $(files))
mods := $(filter-out tests/%, $(basename $(srcs)))
objs := $(addprefix .builds/, $(addsuffix .o, $(mods)))
deps := $(addprefix .builds/, $(addsuffix .dep, $(mods)))

# テスト
tmods := $(filter .tests/%, $(basename $(srcs)))
tobjs := $(addprefix .builds/, $(addsuffix .o, $(tmods)))
tdeps := $(addprefix .builds/, $(addsuffix .dep, $(tmods)))

# 手動テスト
mtmods := $(filter .mtests/%, $(basename $(srcs)))
mtobjs := $(addprefix .builds/, $(addsuffix .o, $(tmods)))
mtdeps := $(addprefix .builds/, $(addsuffix .dep, $(tmods)))


EXLIBS := -lstdc++ -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lm -lvulkan



################################################################# COMMON RULES

ifneq ($(MAKECMDGOALS),clean)
-include $(deps) $(tdeps)
endif

vpath %.o .builds


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
	@echo -n $(dir $@) > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

.builds/%.dep : sources/%.c makefile
	@echo " CPP $@"
	@echo -n $(dir $@) > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@

# Vulkan shader
.builds/%.spv : sources/%.glsl
	@echo " GLSLC $@"
	@glslc $< -o $@


############################################################### RULES & TARGET

$(target): makefile $(objs)
ifeq ($(suffix $(target)),)
	@echo " LD $@"
	@gcc -o $(executable) $(objs) $(EXLIBS)
endif
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	@ar rc $@ $(objs)
endif

install: libtoolbox.a
	@sudo cp libtoolbox.a /usr/local/lib
	@sudo cp -a include/toolbox /usr/local/include

uninstall:
	@sudo rm -f /usr/local/lib/libtoolbox.a
	@sudo rm -rf  /usr/local/include/toolbox

clean:
	rm -rf .builds/* .builds/.tests .builds/.mtests $(target)* $(shell find . -name "*.orig")

# 自動テストを実行
test: $(target) $(tobjs)
	@echo -n building tests...
	@$(foreach m, $(tmods), gcc -o .builds/$(m) .builds/$(m).o -L. -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(tmods), chmod +x .builds/$(m) &&) true
	@echo OK.
	@echo -n running tests...
	@$(foreach m, $(tmods), .builds/$(m) &&) true
	@echo OK.

# 自動テストを実行、手動テストをビルド
mtest: test $(mtobjs)
	@echo -n building manual tests...
	@$(foreach m, $(mtmods), gcc -o .builds/$(m) .builds/$(m).o -L. -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(mtmods), chmod +x .builds/$(m) &&) true
	@echo OK.

