/*定义有效的键值*/

#define UP        0x4800
#define DOWN      0x5000
#define LEFT      0x4b00
#define RIGHT     0x4d00

#define ESC       0x011b
#define SPACE     0x3920
#define ENTER     0x1c0d
#define BACKSPACE 0xe08
#define F1        0x3b00
#define PLUS      0xd3d
#define MINUS     0xc2d

#define LOWERQ 0x1071
#define UPPERQ 0x1051

#define LOWERH 0x2368
#define UPPERH 0x2348

#define LOWERP 0x1970
#define UPPERP 0x1950

#define LOWERN 0x316e
#define UPPERN 0x314e

#define LOWERL 0x266c
#define UPPERL 0x264c

#define LOWERS 0x1f73
#define UPPERS 0x1f53

/*定义一些特殊值*/
#define HARD 10       /*游戏难度（困难）*/
#define EASY 20       /*游戏难度（简单）*/

/*定义时钟中断的中断号*/
#define TIMER 0x1c

/*头文件总汇*/
#include <graphics.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>