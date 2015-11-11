# GNU Makefile to build&run the tests
.PHONY : default
default :

COMPILERS:=gcc-O3 gcc-O2 gcc-O2-fno-exceptions msvc2015-amd64-O2
SOURCE_DIRS:=Various-Usage
OUTPUT_DIR:=generated

SOURCE_FILES:=$(shell find $(SOURCE_DIRS) -name '*.cpp')

ASM_FILES:=$(foreach source,$(SOURCE_FILES),$(foreach compiler,$(COMPILERS), \
    $(OUTPUT_DIR)/$(compiler)/$(source).asm \
  ))
$(foreach source,$(SOURCE_FILES),$(foreach compiler,$(COMPILERS), $(eval \
    $(OUTPUT_DIR)/$(compiler)/$(source).asm : $(source) compiler/$(compiler) ; \
      mkdir -p '$$(@D)' && \
      'compiler/$(compiler)' '$$<' '$$@' \
  )))

default : $(ASM_FILES)
