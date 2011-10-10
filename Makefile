
MINGW_CXX = i486-mingw32-g++ -static

.PHONY: all clean bot tester bot_win tester_win

all: bot tester

all_win: bot_win tester_win

clean:
	$(MAKE) -f Makefile.bot clean
	$(MAKE) -f Makefile.tester clean

bot: 
	$(MAKE) -f Makefile.bot

tester: 
	$(MAKE) -f Makefile.tester

bot_win: 
	$(MAKE) TARGET=bot.exe CXX="$(MINGW_CXX)" -f Makefile.bot

tester_win: 
	$(MAKE) TARGET=tester.exe CXX="$(MINGW_CXX)" -f Makefile.tester

