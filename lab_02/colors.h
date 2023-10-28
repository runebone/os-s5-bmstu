#ifndef __COLORS__H__
#define __COLORS__H__

/*
Colors:              
   R - red              
   G - green            
   B - blue             

   C - cyan
   M - magenta          
   Y - yellow           
   K - blacK            
   W - white

Styles:
   N - normal
   B - bold
   I - italic

   U - underline
   S - strikethrough
   T - change tone
*/

// Text colors (TC)
#define TCK "\033[30m"
#define TCR "\033[31m"
#define TCG "\033[32m"
#define TCY "\033[33m"
#define TCB "\033[34m"
#define TCM "\033[35m"
#define TCC "\033[36m"
#define TCW "\033[37m"

// Background colors (BC)
#define BCK "\033[40m"
#define BCR "\033[41m"
#define BCG "\033[42m"
#define BCY "\033[43m"
#define BCB "\033[44m"
#define BCM "\033[45m"
#define BCC "\033[46m"
#define BCW "\033[47m"

// Text styles (TS)
#define TSN "\033[0m"
#define TSB "\033[1m"
#define TST "\033[2m"
#define TSI "\033[3m"
#define TSU "\033[4m"
#define TSS "\033[9m"

// Reset color (RC) "\033[TS;TC;BCm"
#define RC "\033[0;0;0m"

// Usage: printf(TSB TCG BCR "Hello World!\n" RC);

#endif
