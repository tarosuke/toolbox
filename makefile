##################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. 隣のディレクトリにincludeがあればコンパイル時に-Iオプションに追加
# 3. 隣のディレクトリの$(TARGETDIR)/*.aをリンク対象に追加
# 4. target.makeで指定されない限りターゲット名はカレントディレクトリ名
# 5. ターゲット名の末尾が.aならスタティックライブラリ、でなければ実行形式を生成

.PHONY : clean test RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS += -O3 -DNDEBUG -Wno-stringop-overflow
endif
ifeq ($(MAKECMDGOALS), COVERAGE)
TARGETDIR := COVERAGE
COPTS += -g -coverage
endif
ifndef TARGETDIR
TARGETDIR := DEBUG
COPTS += -O0 -g3
endif


COPTS += -Wall -Werror -D_BUILD_TARGET_=$(TARGETDIR) -Iinclude
CPOTS += $(addprefix -I, $(wildcard ../*/include))
CCOPTS += $(COPTS) -std=c++20

EXLIBS := -lstdc++ -lm
# -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lvulkan -lgdbm

-include target.make
target ?= $(notdir $(PWD))


# このmakefileの場所を推定
MAKEFILE := $(shell if [ -f toolbox/makefile ]; then echo toolbox/makefile; else echo ../toolbox/makefile; fi)



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files:= $(shell find sources tests -type f)
srcs := $(filter $(suffixes), $(files))
ssrcs:= $(filter %.frag %.vert, $(files))
spvs := $(addsuffix .spv, $(ssrcs))
mods := $(basename $(srcs) $(spvs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))

# 試験用設定
testTarget = $(addprefix $(TARGETDIR)/, $(basename $(foreach s, $(suffix $(suffixes)), $(wildcard $(1)/*$(s)))))
tmods:= $(call testTarget,tests)
-include $(wildcard tests/*.make)



################################################################# COMMON RULES

ifneq ($(MAKECMDGOALS),clean)
-include $(deps) $(tdeps) $(ttdeps)
endif

vpath %.o $(TARGETDIR)


$(TARGETDIR)/%.o : %.cc $(MAKEFILE)
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C $(CC) $(CCOPTS) -c -o $@ $<

$(TARGETDIR)/%.o : %.c $(MAKEFILE)
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C ${CC} $(COPTS) -c -o $@ $<

$(TARGETDIR)/%.o : %.glsl $(MAKEFILE)
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

$(TARGETDIR)/%.dep : %.cc $(MAKEFILE)
	@echo " CPP $@"
	@mkdir -p $(dir $@)
	@echo -n $(dir $@) > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

$(TARGETDIR)/%.dep : %.c $(MAKEFILE)
	@echo " CPP $@"
	@echo -n $(dir $@) > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@

# Vulkan shaders
.PRECIOUS: $(addprefix $(TARGETDIR)/, $(spvs))

$(TARGETDIR)/%.frag.spv : %.frag $(MAKEFILE)
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.vert.spv : %.vert $(MAKEFILE)
	@echo " GLSLC $@"
	@glslc $< -o $@

$(TARGETDIR)/%.o : $(TARGETDIR)/%.spv $(MAKEFILE)
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@



############################################################### RULES & TARGET

$(TARGETDIR)/$(target): $(MAKEFILE) $(objs)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	ar rc $@ $(objs)
else
	@echo " LD $@"
	@mkdir -p $(TARGETDIR)
	gcc -o $(TARGETDIR)/$(target) $(objs) $(wildcard ../*/$(TARGETDIR)/*.a) $(EXLIBS)
endif

clean:
	rm -rf RELEASE DEBUG COVERAGE *.gcov

$(TARGETDIR)/tests/% : $(TARGETDIR)/tests/%.o $(TARGETDIR)/$(target)
	@echo " LD $@"
	@gcc -o $@ $@.o -L$(TARGETDIR) -ltoolbox $(EXLIBS) $(subLibraries)

$(TARGETDIR)/tests/%.test : $(TARGETDIR)/tests/%
	$<


.PRECIOUS: $(tmods)
test: $(addsuffix .test, $(tmods))

RELEASE: RELEASE/$(target) test

DEBUG: DEBUG/$(target) test

COVERAGE: EXLIBS += -lgcov
COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
