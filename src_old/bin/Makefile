include ../../config.mk
include ../../default.mk

LDFLAGS+=-L$(LIBETHER_DIR)/src/lib -lether -lm -ldl -lallegro -lallegro_font -lallegro_primitives
EXAMPLES?=$(OBJECTS:.o=)

all: $(EXAMPLES)

$(EXAMPLES): $(OBJECTS)
	@$(CC) $@.o -o $@ $(CPPFLAGS) $(LDFLAGS)
