### PROJECT

TARGET := $(shell basename $(shell pwd))
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)

### CONFIG

BUILD_MODE ?= RELEASE

ifeq ($(BUILD_MODE), RELEASE) 
CXX = g++
CXXFLAGS = -pipe -std=c++0x -pedantic -Wall -Wextra -Wdisabled-optimization -O2 -g -I$(SRCDIR)
else
CXX = clang++
CXXFLAGS = -Qunused-arguments -std=c++0x -pipe -pedantic -Wall -Wextra  -g -I$(SRCDIR)
endif

LDFLAGS = -lm -Wl,--as-needed

VERBOSE_MAKE ?= 0

ifeq ($(VERBOSE_MAKE),1)
	A := 
else
	A := @
endif

OBJDIR := .obj
DEPDIR := .dep

CFLAGS += $(addprefix -I,$(INCLUDEPATH))
CFLAGS += $(addprefix -D,$(DEFINES))
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SOURCES))

GENERATED := $(TARGET) 

### RULES

.PHONY: all clean

all: $(TARGET)

clean:
	@echo -e "\tRM\t$(GENERATED)"
	$(A)rm -f $(GENERATED) 
	@echo -e "\tRM\t$(OBJDIR) $(DEPDIR)"
	$(A)rm -rf $(OBJDIR) $(DEPDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo -e "\tCXX\t$<"
	$(A)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@echo -e "\tDEP\t$<"
	$(A)$(CXX) $(CXXFLAGS) -o $@ -MT "$(OBJDIR)/$*.o" -MM $< 

$(TARGET): $(OBJECTS)
	@echo -e "\tLD\t$^"
	$(A)$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(OBJECTS): Makefile | $(OBJDIR)

$(DEPENDS): Makefile | $(DEPDIR)

$(OBJDIR):
	@mkdir -p $@

$(DEPDIR):
	@mkdir -p $@

-include $(DEPENDS)
