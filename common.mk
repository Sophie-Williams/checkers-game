### PROJECT

# TARGET external
# MODULES external

### CONFIG

CXX ?= g++
CXXFLAGS = -pipe -std=c++0x -pedantic -Wall -Wextra -Wdisabled-optimization -O2 -g

LDFLAGS = -lm -Wl,--as-needed

VERBOSE_MAKE ?= 0

ifeq ($(VERBOSE_MAKE),1)
	A := 
else
	A := @
endif

SRCDIR := src

SRCDIRS := $(foreach module,$(MODULES),$(SRCDIR)/$(module))
vpath %.h $(SRCDIRS)
vpath %.cpp $(SRCDIRS)
INCLUDEPATH += $(SRCDIRS)

SOURCES := $(notdir $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp)))

OBJDIR := .obj-$(TARGET)
DEPDIR := .dep-$(TARGET)

CXXFLAGS += $(addprefix -I,$(INCLUDEPATH))
CXXFLAGS += $(addprefix -D,$(DEFINES))
OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,$(DEPDIR)/%.d,$(SOURCES))

GENERATED := $(TARGET) 

### RULES

.PHONY: all clean

all: $(TARGET)

clean:
	@echo -e "\tRM\t$(GENERATED)"
	$(A)rm -f $(GENERATED) 
	@echo -e "\tRM\t$(OBJDIR) $(DEPDIR)"
	$(A)rm -rf $(OBJDIR) $(DEPDIR)

$(OBJDIR)/%.o: %.cpp
	@echo -e "\tCXX\t$<"
	$(A)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(DEPDIR)/%.d: %.cpp
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
