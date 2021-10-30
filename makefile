#################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. ターゲット情報を拾って、なければディレクトリ名を使う

.PHONY : clean test RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

MAKECMDGOALS ?= RELEASE
ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS += -O3 -DNDEBUG -Wno-stringop-overflow
endif
ifeq ($(MAKECMDGOALS), DEBUG)
TARGETDIR := DEBUG
COPTS += -O0 -g
endif
ifeq ($(MAKECMDGOALS), COVERAGE)
TARGETDIR := COVERAGE
COPTS += -g -coverage
endif

COPTS += -Wall -Werror -Iinclude
CCOPTS += $(COPTS) -std=c++11

EXLIBS += -lstdc++ -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lm -lgcov



-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )


# targetがtoolbox.aでないのでtoolbox/includeをincludeパスに追加
ifneq ($(target)),toolbox.a)
COPTS += -Itoolbox/include
endif



############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files := $(subst sources/,, $(shell find sources -type f))
srcs := $(filter $(suffixes), $(files))
mods := $(basename $(srcs))
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
	@$(CC) $(CCOPTS) -c -o $@ $<

$(TARGETDIR)/%.o : sources/%.c makefile
	@echo " CC $@"
	@mkdir -p $(dir $@)
	@${CC} $(COPTS) -c -o $@ $<

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



############################################################### RULES & TARGET

$(TARGETDIR)/$(target): makefile $(nobjs)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	@ar rc $@ $(nobjs)
	@rm -f $(target)
	@ln -s $(TARGETDIR)/$(target) $(target)
else
	@echo " LD $@"
	@gcc -o $(TARGETDIR)/$(target) $(nobjs) $(EXLIBS)
endif

clean:
	rm -rf RELEASE DEBUG COVERAGE .builds *.gcov

$(tmods) : $(tobjs) $(TARGETDIR)/$(target)
	@echo " LD $@"
	@gcc -o $@ $@.o -L$(TARGETDIR) -ltoolbox $(EXLIBS)

test:  $(tmods)
	@$(shell for m in $(tmods); do AUTO_TEST=1 $$m; done)
	@echo "** TEST DONE."

RELEASE: RELEASE/$(target) test

DEBUG: DEBUG/$(target) test

COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
