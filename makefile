all:
	make -C qon
	make -C input


test:
	make -C qon test
	make -C input test


clean:
	@for f in *; do if [ -d $$f ]; then make -C $$f clean; fi; done
