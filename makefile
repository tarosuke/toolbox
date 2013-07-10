all: rwm

COPTS += -IX11 -I.

include make.in


libs= -lGL -lGLU -lm -lX11 -lXmu -lXi -lXext -lXcomposite -lstdc++ -lglut

test: rwm.test
	./rwm.test

run: rwm
	./rwm

rwm: makefile $(objs) extralibs
	gcc -o $@ $(objs) $(libs)

rwm.test: COPTS+=-DTEST
rwm.test: makefile $(objs) extralibs
	gcc -o $@ $(objs) $(libs)


extralibs:
	make -C room
	make -C rift
	make -C image
	make -C avatar
