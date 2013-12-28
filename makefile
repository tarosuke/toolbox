all:
	make -C complex
	make -C input


test:
	make -C complex test
	make -C input test


clean:
	@for f in *; do if [ -f $$f/makefile ]; then make -C $$f clean; fi; done
