
ifdef ($(OS))
	exe=$(1).exe
	cmd=$(call exe,$(1))
else
	exe=$(1)
	cmd=./$(call exe,$(1))
endif

all: ctalk

ctalk:
	cd ctalk
	clang -Wall -Werror -g -o $(call exe,ct_test) cstream_test.c
	$(call cmd,ct_test)

.ONESHELL: ctalk
.PHONY: all ctalk

