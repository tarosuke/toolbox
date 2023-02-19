#################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. ターゲット情報を拾って、なければディレクトリ名を使う

.PHONY : clean test RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

MAKECMDGOALS ?= RELEASE
ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS := -O3 -DNDEBUG -Wno-stringop-overflow -DVK_USE_PLATFORM_XLIB_KHR
endif
ifeq ($(MAKECMDGOALS), DEBUG)
TARGETDIR := DEBUG
COPTS := -O0 -g3 -DVK_USE_PLATFORM_XLIB_KHR

endif
ifeq ($(MAKECMDGOALS), COVERAGE)
TARGETDIR := COVERAGE
COPTS := -g -coverage -DVK_USE_PLATFORM_XLIB_KHR
endif

COPTS += -Wall -Werror -D_BUILD_TARGET_=$(TARGETDIR) -Iinclude
CCOPTS += $(COPTS) -std=c++11

EXLIBS := -lstdc++ -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lm -lgcov -lvulkan -lgdbm

-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )


# targetがlibtoolbox.aでない場合の定義
ifneq ($(target),libtoolbox.a)
COPTS += -Itoolbox/include
$(TARGETDIR)/$(target): toolbox/$(TARGETDIR)/libtoolbox.a
toolbox/$(TARGETDIR)/libtoolbox.a :
	make -j -C toolbox $(TARGETDIR)
EXLIBS += -Ltoolbox/$(TARGETDIR) -ltoolbox
endif



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files:= $(subst sources/,, $(shell find sources -type f))
srcs := $(filter $(suffixes), $(files))
ssrcs:= $(filter %.frag %.vert, $(files))
spvs := $(addsuffix .spv, $(ssrcs))
mods := $(basename $(srcs) $(spvs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))

# オブジェクトファイルの分類
testPlaces := $(TARGETDIR)/.tests/%
nobjs := $(filter-out $(testPlaces), $(objs))
tobjs := $(filter $(testPlaces), $(objs))
tmods := $(addprefix $(TARGETDIR)/, $(filter .tests/%, $(mods)))



################################################################# COMMON RULES

ifneq ($(MAKECMDGOALS),clean)
-include $(deps) $(tdeps)
endif

vpath %.o $(TARGETDIR)


$(TARGETDIR)/%.o : sources/%.cc makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C $(CC) $(CCOPTS) -c -o $@ $<

$(TARGETDIR)/%.o : sources/%.c makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C ${CC} $(COPTS) -c -o $@ $<

$(TARGETDIR)/%.o : sources/%.glsl makefile
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

$(TARGETDIR)/%.dep : sources/%.cc makefile
	@echo " CPP $@"
	@mkdir -p $(dir $@)
	@echo -n $(dir $@) > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

$(TARGETDIR)/%.dep : sources/%.c makefile
	@echo " CPP $@"
	@echo -n $(dir $@) > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@

# Vulkan shaders
.PRECIOUS: $(addprefix $(TARGETDIR)/, $(spvs))

$(TARGETDIR)/%.frag.spv : sources/%.frag makefile
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.vert.spv : sources/%.vert makefile
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.o : $(TARGETDIR)/%.spv makefile
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@



############################################################### RULES & TARGET

$(TARGETDIR)/$(target): makefile $(nobjs)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	ar rc $@ $(nobjs)
else
	@echo " LD $@"
	gcc -o $(TARGETDIR)/$(target) $(nobjs) $(EXLIBS)
endif

clean:
	@echo test:$(addsuffix .test, )
	rm -rf RELEASE DEBUG COVERAGE *.gcov
	if [ -d toolbox ]; then make -C toolbox clean; fi

$(tmods) : $(tobjs) $(TARGETDIR)/$(target)
	@echo " LD $@"
	@gcc -o $@ $@.o -L$(TARGETDIR) -ltoolbox $(EXLIBS)

testTargets := $(addsuffix .test, $(tmods))

%.test : %
	@echo $<
	@AUTO_TEST=1 $<

test: $(testTargets)


RELEASE: RELEASE/$(target) test

DEBUG: DEBUG/$(target) test

COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
