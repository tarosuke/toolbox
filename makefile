#################################################################### makefile
# 1. ソースを勝手に探して依存関係ファイルを作成
# 3. ターゲット情報を拾って、なければディレクトリ名を使う



############################################################# TARGET & OPTIONS

MAKECMDGOALS ?= RELEASE
ifeq ($(MAKECMDGOALS), RELEASE)
TARGETDIR := RELEASE
COPTS := -O3
endif
ifeq ($(MAKECMDGOALS), DEBUG)
TARGETDIR := DEBUG
COPTS := -O0
endif




-include target.make
target ?= $(shell echo $$PWD | sed s!.*/!! )
all: $(target)

.PHONY : clean



############################################################ FILE RECOGNITIONS

COPTS := -O0 -Wall -Werror -g -IX11
CCOPTS += $(COPTS) -std=c++11

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


EXLIBS := -lstdc++ -lopenvr_api -lX11 -lGL -lGLX -lGLEW -lcairo -ljpeg -lm



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

$(target): makefile $(objs)
ifeq ($(suffix $(target)),)
	@echo " LD $@"
	@gcc -o $(executable) $(objs) $(EXLIBS)
endif
ifeq ($(suffix $(target)),.a)
	@echo " AR $@"
	@ar rc $@ $(objs)
endif



clean:
	rm -rf RELEASE DEBUG

# 自動テストを実行
test: $(target) $(tobjs)
	@echo -n building tests...
	@$(foreach m, $(tmods), gcc -o $(TARGETDIR)/$(m) $(TARGETDIR)/$(m).o -L. -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(tmods), chmod +x $(TARGETDIR)/$(m) &&) true
	@echo OK.
	@echo running tests...
	@$(foreach m, $(tmods), $(TARGETDIR)/$(m) &&) true
	@echo OK.

# 自動テストを実行、手動テストをビルド
mtest: test $(mtobjs)
	@echo -n building manual tests...
	@$(foreach m, $(mtmods), gcc -o $(TARGETDIR)/$(m) $(TARGETDIR)/$(m).o -L. -ltoolbox $(EXLIBS) &&) true
	@$(foreach m, $(mtmods), chmod +x $(TARGETDIR)/$(m) &&) true
	@echo OK.

