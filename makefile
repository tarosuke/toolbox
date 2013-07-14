all: test


test:
	make -C qon test


clean:
	@for f in *; do if [ -d $$f ]; then make -C $$f clean; fi; done
