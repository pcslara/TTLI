#if 0
This code was found at http://www.linuxquestions.org/questions/programming-9/get-character-without-enter-stdin-buffer-36402/
I do not know who wrote it!
#endif

#ifndef ____CURSOR_TTLI_H
#define ____CURSOR_TTLI_H

#include <stdio.h>

#define cursor_moveyx(y,x) printf("\033[%d;%dH",y,x)	/*Move cursor to position y,x (rows, columns) with (1,1) as origin*/
#define cursor_moveup(y) printf("\033[%dA",y)		/*Move cursor up y*/
#define cursor_movedown(y) printf("\033[%dB",y)		/*Move cursor down y*/
#define cursor_moveright(x) printf("\033[%dC",x)	/*Move cursor right x*/
#define cursor_moveleft(x) printf("\033[%dD",x)		/*Move cursor left x*/
#define cursor_store() printf("\033[s")			/*Store current cursor position and color*/
#define cursor_restore() printf("\033[u")		/*Restore cursor position and color from cursor_store()*/
#define cursor_clear()	printf("\033[2J")		/*Clear screen and leave cursor where is*/
#define cursor_clearline() printf("\033[K")		/*Clear to end of line and leave cursor where is*/
#define cursor_fore_black() printf("\033[30m")		/*Change foreground color to black*/
#define cursor_fore_red() printf("\033[31m")		/*Change foreground color to red*/
#define cursor_fore_green() printf("\033[32m")		/*Change foreground color to green*/
#define cursor_fore_orange() printf("\033[33m")		/*Change foreground color to orange*/
#define cursor_fore_blue() printf("\033[34m")		/*Change foreground color to blue*/
#define cursor_fore_magenta() printf("\033[35m")	/*Change foreground color to magenta*/
#define cursor_fore_cyan() printf("\033[36m")		/*Change foreground color to cyan*/
#define cursor_fore_white() printf("\033[37m")		/*Change foreground color to white*/
#define cursor_back_black() printf("\033[40m")		/*Change background color to black*/
#define cursor_back_red() printf("\033[41m")		/*Change background color to red*/
#define cursor_back_green() printf("\033[42m")		/*Change background color to green*/
#define cursor_back_orange() printf("\033[43m")		/*Change background color to orange*/
#define cursor_back_blue() printf("\033[44m")		/*Change background color to blue*/
#define cursor_back_magenta() printf("\033[45m")	/*Change background color to magenta*/
#define cursor_back_cyan() printf("\033[46m")		/*Change background color to cyan*/
#define cursor_back_white() printf("\033[47m")		/*Change background color to white*/
#define cursor_attr_none() printf("\033[0m")		/*Turn off all cursor attributes*/
#define cursor_attr_bold() printf("\033[1m")		/*Make test bold*/
#define cursor_attr_underline() printf("\033[4m")	/*Underline text*/
#define cursor_attr_blink() printf("\033[5m")		/*Supposed to make text blink, usually bolds it instead*/
#define cursor_attr_reverse() printf("\033[7m")		/*Swap background and foreground colors*/

#endif
