/*游戏装载*/

#include <conio.h>
#include <math.h>



/*一个loading动画*/
LoadingMovie()
{
   int graphdriver = VGA, graphmode = VGAHI;   
   int color;
   int i,j,interval=1000*1000*1000;

   /*图形初始化*/
   initgraph(&graphdriver,&graphmode,"f:\\tc");

   settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
   settextjustify(1,1);
   rectangle(199,340,441,382);
   setcolor(2);
   for(i=1;i<241;i++) {
     color=random(15);
     setcolor(color);
     outtextxy(315,230,"Loading");
     line(201+i,342,201+i,380);
     interval = 30-25*sin(i*3.1415/(2*240))*sin(i*3.1415/(2*240));
     delay(interval);          /*延时函数*/
     if(i==70||i==90) delay(5*1000*1000);
   }
}

/*备用的loading动画（当前者运行过慢时）*/
LoadingMovie1()
{
  textmode(C80);          /*一个loading过程*/
  textcolor(WHITE);
  textbackground(0);
  textattr(128+RED+(BLUE<<4));
  clrscr();
  gotoxy(33,12.5);
  puts("Loading......");
  gotoxy(46,12.5);
  delay(200);
}
