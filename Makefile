CXXFLAGS += -Wall -g -std=c++14 -Wextra
INCDIR = -I.
LIBS = avformat swscale avcodec avutil SDL2 

# don't add a build-id to the final binary

LINKLIBS = $(addprefix -l,$(LIBS))
LDLIBS := $(LINKLIBS)
PROG := dec
ELF := $(PROG).elf

CPPSRC := $(shell find . -name "*.cpp")
OBJS = $(patsubst %.cpp, %.o, $(CPPSRC))
OBJS_IN_BUILDDIR = $(addprefix $(BUILD_DIR)/, $(OBJS))

DEPDIR := .d

BUILD_DIR = build
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(INCDIR) -c

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS_IN_BUILDDIR)
	@echo "[Linking] "$@
	$(CXX) $^ $(LDLIBS)  -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "[Compiling] "$(notdir $@)
	$(COMPILE.cc) -o $@ $^


.PRECIOUS: $(DEPDIR)/%.d

clean:
	$(RM) $(BUILD_DIR)/* $(PROG)

include $(wildcard $(patsubst %, $(DEPDIR)/%.d, $(basename $(CPPSRC))))