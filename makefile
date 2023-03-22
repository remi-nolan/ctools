
ifdef OS
	exe := .exe
	fixpath = $(subst /,\,$1)
else
	exe := 
	fixpath = $1
endif

test: cstream_test
	$(call fixpath,./cstest$(exe))

cstream_test:
	clang -Wall -Werror -g -o cstest$(exe) cstream_test.c

