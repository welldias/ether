ALL_C=$(shell find . -name "*.c")
SOURCES?=$(filter-out %_test.c,$(ALL_C))
OBJECTS?=$(SOURCES:.c=.o)

CPPFLAGS=-Wall -Werror -Wmissing-declarations -Wmissing-prototypes -Wno-format -fno-strict-aliasing -g3
CPPFLAGS+=-DLINUX -D__EXPORTED_HEADERS__  -DALLEGRO_NO_MAGIC_MAIN

ifeq ($(ENABLE_TESTS_COVERAGE),true)
CPPFLAGS+=-DTESTS_COVERAGE_ENABLED -O0
CPPFLAGS+=-fprofile-arcs -ftest-coverage
CPPFLAGS+=-lgcov
else
CPPFLAGS+=-O2
endif

CPPFLAGS+=-DPC
CPPFLAGS+=-Wno-unused-result -Wdeclaration-after-statement

INCLUDE_PATHS=-I$(LIBETHER_DIR)/src/lib
CPPFLAGS+= $(INCLUDE_PATHS) $(FLAGS)

export CC CPPFLAGS LIBETHER

all:

%.o: %.c
	# CC $(notdir $(filter %.c,$^)) -o $(notdir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $(filter %.c,$^) -o $@
ifneq ($(DISABLE_CPPCHECK), true)
	@cppcheck --enable=performance,portability,information,missingInclude -f -q $(INCLUDE_PATHS) $(FLAGS) --inline-suppr $(filter %.c,$^)
endif

clean:
	@rm -f $(shell find . -name "*.[oa]") $(shell find . -name "*~") $(shell find . -name "*.gc*") $(shell find src/ -executable -type f)

clobber: clean
	@rm -f $(shell find . -name "*.d")
