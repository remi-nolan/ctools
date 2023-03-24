
ifdef ($(OS))
	exe=$(1).exe
	cmd=$(call exe,$(1))
else
	exe=$(1)
	cmd=./$(call exe,$(1))
endif

all: cstream

cstream:
	cd cstream
	clang -Wall -Werror -g -o $(call exe,cstest) cstream_test.c
	$(call cmd,cstest)

.ONESHELL: cstream
.PHONY: all cstream

