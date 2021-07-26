#################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 2. ターゲット情報を拾って、なければディレクトリ名を使う

.PHONY : clean RELEASE DEBUG COVERAGE



############################################################# TARGET & OPTIONS

MAKECMDGOALS ?= RELEASE
ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS := -O3
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
mods := $(filter-out tests/%, $(basename $(srcs)))
objs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(mods)))
deps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(mods)))

# テスト
tmods := $(filter .tests/%, $(basename $(srcs)))
tobjs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(tmods)))
tdeps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(tmods)))

# 手動テスト
mtmods := $(filter .mtests/%, $(basename $(srcs)))
mtobjs := $(addprefix $(TARGETDIR)/, $(addsuffix .o, $(tmods)))
mtdeps := $(addprefix $(TARGETDIR)/, $(addsuffix .dep, $(tmods)))



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

$(TARGETDIR)/$(target): makefile $(objs)
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	@ar rc $@ $(objs)
else
	@echo " LD $@"
	@gcc -o $(executable) $(objs) $(EXLIBS)
endif
	@echo -n building manual tests...
	@$(foreach m, $(mtmods), gcc -o $(TARGETDIR)/$(m) $(TARGETDIR)/$(m).o -L$(TARGETDIR) -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(mtmods), chmod +x $(TARGETDIR)/$(m) &&) true
	@echo OK.
	@echo -n building tests...
	@$(foreach m, $(tmods), gcc -coverage -o $(TARGETDIR)/$(m) $(TARGETDIR)/$(m).o -L$(TARGETDIR) -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(tmods), chmod +x $(TARGETDIR)/$(m) &&) true
	@echo OK.
	@echo running tests...
	@$(foreach m, $(tmods), $(TARGETDIR)/$(m) &&) true
	@echo OK.
	@rm -f $(target)
	@ln -s $(TARGETDIR)/$(target) $(target)


clean:
	rm -rf RELEASE DEBUG COVERAGE .builds *.gcov

RELEASE: RELEASE/$(target)

DEBUG: DEBUG/$(target)

COVERAGE: COVERAGE/$(target)
	@lcov -c -d $(TARGETDIR) -o $(TARGETDIR)/lcov.info
