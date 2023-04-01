
ifdef ($(OS))
	exe=$(1).exe
	cmd=$(call exe,$(1))
else
	exe=$(1)
	cmd=./$(call exe,$(1))
endif

all: ctalk cbug

ctalk:
	cd ctalk
	clang -Wall -Werror -g -o $(call exe,ct_test) ctalk_test.c
	$(call cmd,ct_test)

cbug:
	cd cbug
	clang -Wall -Werror -g -o $(call exe,cb_test) cbug_test.c
	$(call, cmd,cb_test)

.ONESHELL: ctalk cbug
.PHONY: all ctalk cbug

