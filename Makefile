
MINGW_CXX = i486-mingw32-g++ -static

.PHONY: all clean all_win clean_win bot tester info bot_win tester_win

all: bot tester info

all_win: bot_win tester_win

clean:
	$(MAKE) -f Makefile.bot clean
	$(MAKE) -f Makefile.tester clean
	$(MAKE) -f Makefile.info clean

clean_win:
	$(MAKE) -f Makefile.bot TARGET=bot.exe clean
	$(MAKE) -f Makefile.tester TARGET=tester.exe clean

bot: 
	$(MAKE) -f Makefile.bot

info: 
	$(MAKE) -f Makefile.info

tester: 
	$(MAKE) -f Makefile.tester

bot_win: 
	$(MAKE) TARGET=bot.exe CXX="$(MINGW_CXX)" -f Makefile.bot

tester_win: 
	$(MAKE) TARGET=tester.exe CXX="$(MINGW_CXX)" -f Makefile.tester

