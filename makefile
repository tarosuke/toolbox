all: libtoolbox.a

.PHONY : clean test watch uninstall
.DELETE_ON_ERROR : $(wildcard objs/*)


####################################################################### VALUES

COPTS += -Wall -Werror -IX11
CCOPTS += $(COPTS) -std=c++11


ssfc = .c .cc .glsl
spth = */* */*/*
srcs = $(foreach p, $(spth), $(foreach s, $(ssfc), $(wildcard $(p)$(s))))
hdrs = $(foreach p, * $(spth), $(foreach s, .h, $(wildcard $(p)$(s))))

dirs = $(sort $(dir $(srcs)))
mods = $(basename $(notdir $(srcs)))
dmds= $(addprefix objs/, $(mods))
objs = $(addsuffix .o, $(dmds)) $(exobjs)
deps = $(addsuffix .d, $(dmds))

exobjs =



######################################################################## RULES

libtoolbox.a: makefile $(objs)
	@echo " AR $@"
	@ar rc $@ $(objs)

install: libtoolbox.a
	@cp libtoolbox.a /usr/local/lib
	@cp -a include/toolbox /usr/local/include

uninstall:
	@rm -f /usr/local/lib/libtoolbox.a
	@rm -rf  /usr/local/include/toolbox

clean:
	rm -f objs/* toolbox*



################################################################# COMMON RULES


-include $(deps)

vpath %.o objs
vpath % $(dirs)


objs/%.o : %.cc makefile
	@echo " CC $@"
	@$(CC) $(CCOPTS) -c -o $@ $<

objs/%.o : %.c makefile
	@echo " CC $@"
	@${CC} $(COPTS) -c -o $@ $<

objs/%.o : %.glsl makefile
	@echo " OBJCOPY $@"
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

objs/%.d : %.cc
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

objs/%.d : %.c
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(COPTS) -MM $< >> $@


