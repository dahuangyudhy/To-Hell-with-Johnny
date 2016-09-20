/*游戏结束*/

#include <graphics.h>
#include <stdlib.h>


Byebye()
{
  int graphdriver = VGA;
  int graphmode = VGAHI;

  Music1(1);

/*前一个版本，未调用位图
  initgraph(&graphdriver,&graphmode,"f:\\tc");
  cleardevice();
  setcolor(BLUE);
  setbkcolor(WHITE);
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,6);
  settextjustify(1,1);
  outtextxy(320,200,"Enjoy Yourself");
  outtextxy(320,260,"and HavING Fun!!");
  delay(30000);
*/

  ShowPicture(12,0,0);
  exit(0);
}
