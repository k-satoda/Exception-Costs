# GNU Makefile to build&run the tests
.PHONY : default run
default : run

COMPILERS:=gcc-O3 msvc2015-amd64-O2
SOURCE_DIRS:=Various-Usage
OUTPUT_DIR:=generated

SOURCE_FILES:=$(shell find $(SOURCE_DIRS) -name '*.cpp')

PROGRAMS:=$(foreach source,$(SOURCE_FILES),$(foreach compiler,$(COMPILERS), \
    $(OUTPUT_DIR)/$(compiler)/$(source).exe \
  ))
$(foreach source,$(SOURCE_FILES),$(foreach compiler,$(COMPILERS), $(eval \
    $(OUTPUT_DIR)/$(compiler)/$(source).exe : $(source) compiler/$(compiler) ; \
      mkdir -p '$$(@D)' && \
      'compiler/$(compiler)' '$$<' '$$@' \
  )))

RESULTS:=$(patsubst %.exe,%.txt,$(PROGRAMS))
$(RESULTS) : %.txt : %.exe
	$< >$@

run : $(PROGRAMS)
	for program in $(sort $(PROGRAMS)); do \
	  echo "$$program"; \
	  "$$program" | tee ${program/%.exe/.txt}; \
	  echo; echo; \
	done
