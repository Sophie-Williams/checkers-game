.PHONY: all clean bot tester

all: bot tester

clean:
	$(MAKE) -f Makefile.bot clean
	$(MAKE) -f Makefile.tester clean

bot: 
	$(MAKE) -f Makefile.bot

tester: 
	$(MAKE) -f Makefile.tester
