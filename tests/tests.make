
linuxTest: $(addsuffix .test, $(addprefix $(TARGETDIR)/, $(basename $(foreach s, $(suffix $(suffixes)), $(wildcard tests/linux/*$(s))))))
