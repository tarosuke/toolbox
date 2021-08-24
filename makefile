#################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. ターゲット情報を拾って、なければディレクトリ名を使う

.PHONY : clean test RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

MAKECMDGOALS ?= RELEASE
ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS := -O3 -DNDEBUG -Wno-stringop-overflow
endif
ifeq ($(MAKECMDGOALS), DEBUG)
TARGETDIR := DEBUG
COPTS := -O0 -g
endif
ifeq ($(MAKECMDGOALS), COVERAGE)
TARGETDIR := COVERAGE
COPTS := -g -coverage
endif

COPTS += -Wall -Werror -Iinclude
CCOPTS += $(COPTS) -std=c++11

EXLIBS := -lstdc++ -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lm -lgcov



-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )




############################################################ FILE RECOGNITIONS

suffixes := %.c %.cc %.glsl

files := $(subst sources/,, $(shell find sources -type f))
srcs := $(filter $(suffixes), $(files))
mods := $(basename $(srcs))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))

# オブジェクトファイルの分類
testPlaces := $(TARGETDIR)/.mtests/% $(TARGETDIR)/.tests/%
nobjs := $(filter-out $(testPlaces), $(objs))
tobjs := $(filter $(testPlaces), $(objs))
tmods := $(filter .tests/%, $(mods))



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
else
	@echo " LD $@"
	@gcc -o $(executable) $(nobjs) $(EXLIBS)
endif
	@rm -f $(target)
	@ln -s $(TARGETDIR)/$(target) $(target)


clean:
	rm -rf RELEASE DEBUG COVERAGE .builds *.gcov

test: $(TARGETDIR)/$(target) $(tobjs)
	@echo -n building tests...
	@$(foreach m, $(tmods), gcc -coverage -o $(TARGETDIR)/$(m) $(TARGETDIR)/$(m).o -L$(TARGETDIR) -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(tmods), chmod +x $(TARGETDIR)/$(m) &&) true
	@echo OK.
	@echo running tests...
	@$(shell for m in $(tmods); do AUTO_TEST=1 $(TARGETDIR)/$$m; done)
	@echo OK.

RELEASE: RELEASE/$(target) test

DEBUG: DEBUG/$(target) test

COVERAGE: COVERAGE/$(target) test
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
