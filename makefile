#################################################################### $(ME)
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. ターゲット情報を拾って、なければディレクトリ名を使う

.PHONY : clean test RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

ME := $(MAKEFILE_LIST)
MAKECMDGOALS ?= RELEASE

ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS += -O3 -DNDEBUG -Wno-stringop-overflow
endif
ifeq ($(MAKECMDGOALS), COVERAGE)
TARGETDIR := COVERAGE
COPTS += -g -coverage
endif
ifeq ($(MAKECMDGOALS), DEBUG)
TARGETDIR := DEBUG
COPTS += -O0 -g3
endif

TARGETDIR ?= DEBUG
COPTS += -O0 -g3


COPTS += -Wall -Werror -D_BUILD_TARGET_=$(TARGETDIR) -Iinclude
CCOPTS += $(COPTS) -std=c++20

EXLIBS := -lstdc++ -lm
# -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lvulkan -lgdbm

-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )


# targetがlibtoolbox.aでない場合の定義
TOOLBOXDIR ?= toolbox
ifneq ($(target),libtoolbox.a)
COPTS += -I$(TOOLBOXDIR)/include
$(TARGETDIR)/$(target): $(TOOLBOXDIR)/$(TARGETDIR)/libtoolbox.a
$(TOOLBOXDIR)/$(TARGETDIR)/libtoolbox.a :
	make -j -C $(TOOLBOXDIR) $(TARGETDIR)
EXLIBS += -L$(TOOLBOXDIR)/$(TARGETDIR) -ltoolbox
endif



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files:= $(shell find sources tests -type f)
srcs := $(filter $(suffixes), $(files))
ssrcs:= $(filter %.frag %.vert, $(files))
spvs := $(addsuffix .spv, $(ssrcs))
mods := $(basename $(srcs) $(spvs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))
subs := $(dir $(wildcard */makefile))

# 試験用設定
testTarget = $(addprefix $(TARGETDIR)/, $(basename $(foreach s, $(suffix $(suffixes)), $(wildcard $(1)/*$(s)))))
tmods:= $(call testTarget,tests)
-include $(wildcard tests/*.make)


################################################################# COMMON RULES

ifneq ($(MAKECMDGOALS),clean)
-include $(deps) $(tdeps) $(ttdeps)
endif

vpath %.o $(TARGETDIR)


$(TARGETDIR)/%.o : %.cc makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C $(CC) $(CCOPTS) -c -o $@ $<

$(TARGETDIR)/%.o : %.c makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C ${CC} $(COPTS) -c -o $@ $<

$(TARGETDIR)/%.o : %.glsl makefile
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

$(TARGETDIR)/%.dep : %.cc makefile
	@echo " CPP $@"
	@mkdir -p $(dir $@)
	@echo -n $(dir $@) > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

$(TARGETDIR)/%.dep : %.c makefile
	@echo " CPP $@"
	@echo -n $(dir $@) > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@

# Vulkan shaders
.PRECIOUS: $(addprefix $(TARGETDIR)/, $(spvs))

$(TARGETDIR)/%.frag.spv : %.frag makefile
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.vert.spv : %.vert makefile
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.o : $(TARGETDIR)/%.spv $(ME)
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@



############################################################### RULES & TARGET

$(TARGETDIR)/$(target): makefile $(objs)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	ar rc $@ $(objs)
else
	@echo " LD $@"
	gcc -o $(TARGETDIR)/$(target) $(objs) $(EXLIBS)
endif

clean:
	rm -rf RELEASE DEBUG COVERAGE *.gcov
	$(if $(subs), $(foreach $(subs), s, make -C $(s) clean))

$(TARGETDIR)/tests/% : $(TARGETDIR)/tests/%.o $(TARGETDIR)/$(target)
	@echo " LD $@"
	@gcc -o $@ $@.o -L$(TARGETDIR) -ltoolbox $(EXLIBS)

$(TARGETDIR)/tests/%.test : $(TARGETDIR)/tests/%
	$<


.PRECIOUS: $(tmods)
test: $(addsuffix .test, $(tmods))

RELEASE: RELEASE/$(target)

DEBUG: DEBUG/$(target)

COVERAGE: EXLIBS += -lgcov
COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
