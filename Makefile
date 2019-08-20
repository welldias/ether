ifeq (,$(strip $(filter %_config, $(MAKECMDGOALS))))
include ./config.mk
include ./default.mk
endif

all:
	make -C src
	#make -C test
