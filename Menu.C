/*游戏开始*/

#include <graphics.h>
#include <conio.h>


DrawMainMenu()
{
  int graphdriver = VGA;
  int graphmode = VGAHI;

  /*图形初始化*/
  initgraph(&graphdriver,&graphmode,"f:\\tc"); 
  cleardevice();
  setbkcolor(BLACK);

  /*绘制游戏标题*/
  setcolor(BROWN);      
  settextstyle(4,HORIZ_DIR,8);
  outtextxy(55,19,"Hundred Layers");


  /*游戏制作人说明*/
  setcolor(WHITE);      
  settextstyle(2,HORIZ_DIR,6);
  outtextxy(490,455,"No.3080030167");
  hanzi("制作人",480,430);
  hanzi("丁寰宇",550,430);


  /*绘制游戏选项*/
  setfillstyle(1,BLUE); 
  setcolor(YELLOW);
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
  settextjustify(1,1);
  outtextxy(300,155,"New  Game (N)");
  outtextxy(300,155+(374-155)/3,"Load Game (L)");
  outtextxy(300,155+(374-155)*2/3,"Help (H)");
  outtextxy(300,374,"Exit (Esc or Q)");
}

MenuChoice()
{
  while(1)
  {
    key = 0;
    key = bioskey(0);
    switch(key)
    {
      case ENTER: case LOWERN: case UPPERN: StartGame(0);break;
      case LOWERL: case UPPERL: StartGame(1);break;
      case UPPERH: case LOWERH: case F1: Help(); break;
      case ESC: case UPPERQ: case LOWERQ: Byebye();break;
      default: break;
    }  
  } 
}