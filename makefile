##################################################################### makefile
# * ソースを勝手に探してビルド
# * ソースはsources、ヘッダはinclude、試験コードはtestにあると想定
# * subsystems/*の下にも同様な構造があると想定
# * subsystems/*/includeが-Iオプションに追加される(これはsubsystems以下も)
# * subsystems/*以下はリンク前に*これ*自体が再帰的に適用されてmakeされる
# * target.makeで指定されない限りターゲット名はカレントディレクトリ名
# * ターゲット名の末尾が.aならスタティックライブラリ、でなければ実行形式を生成
# * RELEASE DEBUG COVERAGEターゲットでは同名のビルドディレクトリが掘られる

.PHONY : clean test RELEASE DEBUG COVERAGE FULLTEST



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
CCOPTS += $(COPTS) -std=c++20

EXLIBS += -lstdc++ -lm
EXLIBS += $(addprefix -L, $(wildcard */$(TARGETDIR)))

-include target.make
target ?= $(notdir $(CURDIR))
libtarget:=$(basename $(target)).a


# このmakefileの場所を推定
MAKEFILE ?= $(CURDIR)/$(firstword $(MAKEFILE_LIST))
export MAKEFILE



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

findFiles = $(shell mkdir -p $(1); find $(1) -type f)

files:= $(call findFiles, sources)
srcs := $(filter $(suffixes), $(files))
ssrcs:= $(filter %.frag %.vert, $(files))
spvs := $(addsuffix .spv, $(ssrcs))
mods := $(basename $(srcs) $(spvs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))

# commons & subsystems
commons := $(wildcard commons/*)
COMMONS += $(abspath $(commons))
export COMMONS # commonsは継承される
subsystems := $(wildcard subsystems/*)
subs := $(COMMONS) $(subsystems) # ヘッダ、ライブラリなどは両方
IOPTS := $(addprefix -I, $(addsuffix /include, $(subs)))
LIBS := $(foreach s, $(subs), $(wildcard $(s)/$(TARGETDIR)/*.a))

# 試験用設定
ifeq ($(MAKECMDGOALS), FULLTEST)
tfiles:= $(call findFiles, tests)
else
tfiles:= $(wildcard tests/*)
endif
tsrcs := $(filter $(suffixes), $(tfiles))
tmods := $(basename $(tsrcs))
tobjs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(tmods)))
tdeps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(tmods)))
-include $(wildcard tests/*.make)



################################################################# COMMON RULES

ifneq ($(MAKECMDGOALS),clean)
-include $(deps) $(tdeps) $(ttdeps)
endif

vpath %.o $(TARGETDIR)

absPath := 2>&1 | sed -e "s@^\(sources\|include\|tests\)@$(CURDIR)/\1@"


$(TARGETDIR)/%.o : %.cc $(MAKEFILE)
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C $(CC) $(CCOPTS) $(IOPTS) -c -o $@ $< $(absPath)

$(TARGETDIR)/%.o : %.c $(MAKEFILE)
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C ${CC} $(COPTS) $(IOPTS) -c -o $@ $< $(absPath)

$(TARGETDIR)/%.o : %.glsl $(MAKEFILE)
	@echo " OBJCOPY $@"
	@mkdir -p $(dir $@)
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

$(TARGETDIR)/%.dep : %.cc $(MAKEFILE)
	@echo " CPP $@"
	@mkdir -p $(dir $@)
	@echo -n $(dir $@) > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@ $(absPath)

$(TARGETDIR)/%.dep : %.c $(MAKEFILE)
	@echo " CPP $@"
	@echo -n $(dir $@) > $@
	@mkdir -p $(dir $@)
	@$(CPP) $(COPTS) -MM $< >> $@ $(absPath)

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

.PHONY: SUBTARGETS
SUBTARGETS:
	@for s in $(commons) $(subsystems); do make -rj -f $(MAKEFILE) -C $$s $(MAKECMDGOALS) || exit -1; done


$(TARGETDIR)/$(libtarget): $(objs) $(MAKEFILE)
	@echo " AR $@"
	@ar rc $@ $(objs)

ifneq ($(suffix $(target)),.a)
$(TARGETDIR)/$(target): $(objs) SUBTARGETS $(MAKEFILE)
	@echo " LD $@"
	@mkdir -p $(TARGETDIR)
	@gcc -o $(TARGETDIR)/$(target) $(objs) $(LIBS) $(EXLIBS)
# shellとかやってるのはwildcardがコマンド実行前に評価される挙動故
endif

clean: SUBTARGETS
	rm -rf RELEASE DEBUG COVERAGE *.gcov

$(TARGETDIR)/tests/% : $(TARGETDIR)/tests/%.o $(TARGETDIR)/$(libtarget)
	@echo " LD $@"
	@gcc -o $@ $@.o $(TARGETDIR)/$(libtarget) $(subLibraries) $(EXLIBS)

$(TARGETDIR)/tests/%.test : $(TARGETDIR)/tests/%
	@echo $<
	@$< $(absPath)


.PRECIOUS: $(addprefix $(TARGETDIR)/, $(tmods))

test: SUBTARGETS $(addsuffix .test, $(addprefix $(TARGETDIR)/, $(tmods)))

FULLTEST: SUBTARGETS test

RELEASE: SUBTARGETS RELEASE/$(target)

DEBUG: SUBTARGETS DEBUG/$(target)

COVERAGE: EXLIBS += -lgcov
COVERAGE: SUBTARGETS COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
