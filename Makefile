.PHONY: all clean

all: bot

clean:
	$(MAKE) -f Makefile.bot clean

bot:
	$(MAKE) -f Makefile.bot
