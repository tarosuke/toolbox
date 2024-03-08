
linuxTestTargets := $(call testTarget,tests/linux)
$(linuxTestTargets): EXLIBS +=
linuxTest: $(addsuffix .test, $(linuxTestTargets))

