##################################################################### makefile
# * ソースを勝手に探して依存関係ファイルを作成
# * ソースはsources、ヘッダはinclude、試験コードはtestに
# * subsystem/*の下にも同様な構造があると想定
# * subsystem/*/includeが-Iオプションに追加される(これはsubsystem以下も)
# * subsystem/*以下はリンク前に*これ*自体が再帰的に適用されてmakeされる
# * target.makeで指定されない限りターゲット名はカレントディレクトリ名
# * ターゲット名の末尾が.aならスタティックライブラリ、でなければ実行形式を生成
# * RELEASE DEBUG COVERAGEターゲットでは同名のビルドディレクトリが掘られる

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
CCOPTS += $(COPTS) -std=c++20

EXLIBS := -lstdc++ -lm
# -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lvulkan -lgdbm
EXLIBS += $(addprefix -L, $(wildcard */$(TARGETDIR)))

-include target.make
target ?= $(notdir $(PWD))


# このmakefileの場所を推定
MAKEFILE := $(shell if [ -f toolbox/makefile ]; then echo toolbox/makefile; else echo ../toolbox/makefile; fi)



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files:= $(shell mkdir -p sources tests; find sources tests -type f)
srcs := $(filter $(suffixes), $(files))
ssrcs:= $(filter %.frag %.vert, $(files))
spvs := $(addsuffix .spv, $(ssrcs))
mods := $(basename $(srcs) $(spvs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))


# subsystems
subsystems:= $(wildcard subsystems/*)
EXINCS ?= $(addprefix -I$(CURDIR)/, $(addsuffix /include, $(subsystems)))
MAKEFILEPATH?= $(CURDIR)/$(firstword $(MAKEFILE_LIST))
export EXINCS
export MAKEFILEPATH


# 試験用設定
testTarget = $(addprefix $(TARGETDIR)/, $(basename $(foreach s, $(suffix $(suffixes)), $(wildcard $(1)/*$(s)))))
tmods:= $(call testTarget,tests)
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
	@LANG=C $(CC) $(CCOPTS) $(EXINCS) -c -o $@ $< $(absPath)

$(TARGETDIR)/%.o : %.c $(MAKEFILE)
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@LANG=C ${CC} $(COPTS) $(EXINCS) -c -o $@ $< $(absPath)

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

$(TARGETDIR)/$(target): $(objs) $(MAKEFILE)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	@ar rc $@ $(objs)
else
	@for s in $(subsystems); do make -rj -f $(MAKEFILEPATH) -C $$s $(MAKECMDGOALS); done
	@echo " LD $@"
	@mkdir -p $(TARGETDIR)
	gcc -o $(TARGETDIR)/$(target) $(objs) $(wildcard $(addsuffix /$(TARGETDIR)/*.a, $(subsystems))) $(EXLIBS)
endif

clean:
	rm -rf RELEASE DEBUG COVERAGE *.gcov
	@for s in $(subsystems); do make -rj -f $(MAKEFILEPATH) -C $$s $(MAKECMDGOALS); done

$(TARGETDIR)/tests/% : $(TARGETDIR)/tests/%.o $(TARGETDIR)/$(target)
	@echo " LD $@"
	@gcc -o $@ $@.o -L$(TARGETDIR) -ltoolbox $(EXLIBS) $(subLibraries)

$(TARGETDIR)/tests/%.test : $(TARGETDIR)/tests/%
	@echo $<
	@$< $(absPath)


.PRECIOUS: $(tmods)
test: $(addsuffix .test, $(tmods))

RELEASE: RELEASE/$(target)

DEBUG: DEBUG/$(target)

COVERAGE: EXLIBS += -lgcov
COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
