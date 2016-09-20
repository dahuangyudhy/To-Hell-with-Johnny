/*游戏进行时*/

#include <graphics.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "f:\tc\1\define.c"
#include "f:\tc\1\save.c"

int level = EASY;                          /*游戏难度级别参数*/
int thorn[6] = {0,20,0,20,0,35-1};       /*顶端尖刺坐标*/
int px[8], py[8];                        /*用数组标定木板中心位置，p是plank的首字母*/
int pindex[8];                           /*用数组标定木板类型，p是plank的首字母*/
int *topx, *topy, *bottomx, *bottomy;    /*分别为位于最顶端和最底端的木板的位置指针*/
int *bottom_index;   			 /*位于最底端的木板的类型指针*/
int top_num, bottom_num;                 /*用于记录顶指针和底指针分别指向的数组序号*/
int man_x, man_y, man_x0, man_y0;        /*人物当前位置及前一个单位时间的位置*/
int new_plank;                           /*判断人物是否掉到了新木板上*/
int key;                                 /*读键用*/
int plank_speed = 2;                     /*木板移动速度*/
int man_drop_speed = 1;                  /*人物下落速度*/
int man_move_speed = 5;                  /*人物左右移动速度*/
int life;                                /*人物生命值*/
int layer;                               /*已下降的层数*/
int plank_stand;                         /*现在所站立的木板序号*/
int action_flag;                        /*判断某动作是否已执行*/

int drop_control;                        /*control变量，用于控制人物下落速度*/
int timer_control;                       /*control变量，用于控制游戏速度*/

/*游戏难度相关变量*/
int drop_loop;                           /*drop_control不断自加，达到drop_loop时人物下落一次*/     
int timer_loop;                          /*timer_control不断自加，达到timer_loop时游戏循环进行一次*/

/*音乐播放控制相关变量*/
int music_control;                       /*music_control不断自加，控制音乐的播放*/
int music_order;                         /*记录乐曲的播放顺序*/


int GetKey();
int Check_On_Plank();
void CheckLife();
void DrawMan(int x, int y, int color);
void DrawWindow();
void DrawPlank(int x, int y, int color,int index);
void DrawThorn();
void DrawLife();
void DrawLayer();
void DrawPauseGame();
int PauseGame();
int IsConflict();
void LoseGame();

void StartGame(int load)
{
  int graphdriver = VGA, graphmode = VGAHI;       /*图形设置模式参数*/
  int layer_counter = 1;                          /*用于记录人物下降层数的计数器*/
  int index_rand;                                 /*用于产生木板类型的随机数*/
  int life_on_thorn = 0;                          /*当人物在带刺的木板上时，用此变量计时并计算相应生命值损失*/
  int i;                                          /*循环变量*/ 

  /*为载入而设的临时变量，存储木板和人物信息*/
  int *temp1[6], *temp2[8], *temp3[8], *temp4[8];
  int bottom_temp, top_temp;  


  /*界面准备过程*/

  /*1、图形初始化*/
  initgraph(&graphdriver,&graphmode,"f:\\tc");
  setbkcolor(BLACK);
  setcolor(WHITE);
  cleardevice();

  /*2、画游戏边框*/
  rectangle(30,20,430,440);
  setcolor(BROWN);
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(450,20,"Your Life :");
  outtextxy(435,100,"Layers Achieved :");
  setfillstyle(1,YELLOW);

  /*3、画初始血条*/
  for(i=460;i<=460+11*10;i += 10)
  {
    rectangle(i,60,i+10,80);
    bar(i+1,60+1,i+10-1,80-1);
  }
  DrawThorn();
  ShowHiscore();
  
  /*4、提示绘制*/
  setcolor(WHITE);
  setfillstyle(1,BLACK);
  hanzi("空格键为暂停",100-50,462);
  settextjustify(1,2);
  settextstyle(2,0,5);
  outtextxy(235-50,463,", S");
  hanzi("键为保存",260-50,462);
  outtextxy(370-50,463,",\"+\"\"-\"");
  hanzi("更改游戏难度",400-50,462);



  /*以下为游戏进行过程的函数部分*/

  /*初始化木板位置*/
  px[0] = 230;
  py[0] = 450;
  pindex[0] = 1;
  for(i=1;i<8;i++)
  {
    px[i] = 10000;
    py[i] = 10000;
    pindex[i] = 1;
  }

  /*将顶指针和底指针同时指向第一块木板*/
  topx = bottomx = px;
  topy = bottomy = py;
  bottom_index = pindex;
  top_num = bottom_num = 0;

  /*设置人物初始位置*/
  man_x0 = man_x = 230;
  man_y0 = man_y = 430;

  /*设置初始生命值*/
  life = 12;

  /*设置初始层数*/
  layer = 0;
  DrawLayer();

  /*若选择载入游戏，则进行载入*/
  if(load && LoadGame(temp1[0], temp1[1], temp1[2], temp1[3], temp2, temp3, temp4, temp1[4], temp1[5]))
  {
    life  = *temp1[0];
    layer = *temp1[1]; 
    man_x0 = man_x = *temp1[2];
    man_y0 = man_y = *temp1[3];

    for(i=0;i<8;i++){
      px[i] = *temp2[i];
      py[i] = *temp3[i];
      pindex[i] = *temp4[i];
    }

    bottom_num = *temp1[4];
    bottomx = px + bottom_num;
    bottomy = py + bottom_num;
    bottom_index = pindex + bottom_num;

    top_num = *temp1[5];
    topx = px + bottom_num;
    topy = py + bottom_num;

    for(i=0;i<8;i++)
      DrawPlank(px[i],py[i],GREEN,pindex[i]);
    DrawLife();
    setfillstyle(1,BLACK);
    DrawLayer(); 
  }

  /*随机数初始化，以产生源源不断冒出的新木板的横坐标*/
  randomize();

  /*画出人物初始位置*/
  DrawMan(man_x,man_y,BLUE);

  /*设定难度级别*/
  level = EASY;
  drop_loop = 2200;
  timer_loop = 4000;   

  /*音乐播放顺序控制变量重置为0，从乐曲的第一个音符进行播放*/
  music_order = 0;

  /*单个音符播放长度控制变量重置为0*/
  music_control = 0;

  /*游戏过程循环*/
  while(1)
  {
    new_plank = 0;
                                                                                    
    /*判断底指针的位置更改*/
    if(*bottomy <= 430-20-60)  /*不断调整位于bottom位置的木板的指针，以便产生从底部产生新的木板*/
    {
      if(bottomx == px+8-1){
        if(topy != py) {
          bottomx = px;
          bottomy = py;
          bottom_index = pindex;
          bottom_num = 0;
        }
        else {
          bottomx = px+1;
          bottomy = py+1;
          bottom_index = pindex+1;
          bottom_num = 1;
        }
      }/*if*/
      else if(bottomy+1 !=  topy){
             bottomx++;
             bottomy++;
             bottom_index++;
             bottom_num++;
           }
           else {
             bottomx += 2;
             bottomy += 2;
             bottom_index += 2;
             bottom_num +=2;
           }
      *bottomx = (int)rand()%320+70;
      *bottomy = 430;
      index_rand = (int)rand()%10;  
      switch(index_rand)
      {
        case 2: case 12:
        if ( *(bottom_index-1)!=2 ) {*bottom_index = 2;break;}
        case 3: case 13:
        *bottom_index = 3;break;
        case 4: case 14: 
        *bottom_index = 4;break;
        case 5: case 15: 
        *bottom_index = 5;break;
        case 6: case 16:
        if(level==HARD)      /*游戏级别为困难时，增加带刺木板的出现频率*/
        {
          *bottom_index = 5;
          break;        
        }
        default: 
        *bottom_index = 1;break;
      }
    }/*if*/

    /*判断顶指针的位置更改*/
    if(*topy < 50)  /*不断调整位于top位置的木板的指针，防止bottom与top指针重合以致bottom将top所指的木板移到最下面，并使得最上面那块木板消不掉*/
    {
      if(topx == px+8-1) {
        topx = px;
        topy = py;
        top_num = 0;
      }
      else{
        topx++;
        topy++;
        top_num++;
      }
      CheckLife();
    }

    /*判断尚未有操作时人物应当怎样移动*/
    if(Check_On_Plank())         
    {  
      switch(pindex[plank_stand])
      {
        case 3:  /*传送带(向左)*/
        if(man_x - man_move_speed >= 30+10){
          DrawMan(man_x0,man_y0,BLACK);
          man_y -= plank_speed;      /*与木板同速*/
          man_x -= (int)(man_move_speed/3);
          man_x0 = man_x;
          man_y0 = man_y;
          DrawMan(man_x,man_y,BLUE);
        }
        else{
          DrawMan(man_x0,man_y0,BLACK);
          man_y -= plank_speed;      /*与木板同速*/
          man_x0 = man_x;
          man_y0 = man_y;
          DrawMan(man_x,man_y,BLUE);
        }
        break;
        case 4:  /*传送带(向右)*/
        if(man_x + man_move_speed <= 430-10){
          DrawMan(man_x0,man_y0,BLACK);
          man_y -= plank_speed;      /*与木板同速*/
          man_x += (int)(man_move_speed/3);
          man_x0 = man_x;
          man_y0 = man_y;
          DrawMan(man_x,man_y,BLUE);
        }
        else{
          DrawMan(man_x0,man_y0,BLACK);
          man_y -= plank_speed;      /*与木板同速*/
          man_x0 = man_x;
          man_y0 = man_y;
          DrawMan(man_x,man_y,BLUE);
        }
        break;
        case 5:
        if(++life_on_thorn%20 == 0)
          life = life-1;             /*掉到带有尖刺的木板上，损失生命值*/
        CheckLife();
        DrawMan(man_x0,man_y0,BLACK);
        man_y -= plank_speed;        /*与木板同速*/
        man_x0 = man_x;
        man_y0 = man_y;
        DrawMan(man_x,man_y,BLUE);
        break;
        default:
        DrawMan(man_x0,man_y0,BLACK);
        man_y -= plank_speed;       /*与木板同速*/
        man_x0 = man_x;
        man_y0 = man_y;
        DrawMan(man_x,man_y,BLUE);
        break;
      }
    }/*if*/

    /*木板集体上移一次*/
    for(i=0;i<8;i++)
    {
      DrawPlank(px[i],py[i] + plank_speed,BLACK,1);
      if (py[i] < 50)  px[i] = py[i] = 10000;
      DrawPlank(px[i],py[i],GREEN,pindex[i]);
      py[i] = py[i] - plank_speed;
    }

    /*新产生的木板将边框擦掉了一部分，现在补画*/
    DrawWindow(); 

    /*重置游戏速度控制计时器*/   
    timer_control = 0;

    /*重置下落控制计时器*/
    drop_control = 0;

    /*当有键按下时，判断应当执行何种命令，人物应当怎样移动*/
    while(1)          
    {
      if(GetKey())
      {
        if(key == ESC){
          closegraph();
          DrawMainMenu();
          MenuChoice();
          return;
        } 
        DrawMan(man_x0,man_y0,BLACK);
        man_x0 = man_x;
        man_y0 = man_y;
        DrawMan(man_x,man_y,BLUE);
      }
                                                                                         
      if(Check_On_Plank() == 0)        /*不在木板上，就下落*/
      {
        if(++drop_control % drop_loop == 0) {
        DrawMan(man_x0,man_y0,BLACK);
        man_y += man_drop_speed;
        man_x0 = man_x;
        man_y0 = man_y;
        DrawMan(man_x,man_y,BLUE);
        }
        new_plank = 1;   /*现在不在木板上，说明将掉到一个新木板上面*/
      }
      else if(Check_On_Plank() && new_plank == 1)  /*若是掉到了新木板上，就随着新木板上移一次*/
           {
             DrawMan(man_x0,man_y0,BLACK);
             man_y -= plank_speed;
             man_x0 = man_x;
             man_y0 = man_y;
             DrawMan(man_x,man_y,BLUE);
             new_plank = 0;
             if(pindex[plank_stand] == 5 && new_plank == 1)  
               life -= 1;  /*掉到带有尖刺的木板上，损失生命值*/
             CheckLife();
           }
 
                        
                       
/*第一个版本，不带音乐的if((inportb(0x3da)&0x08)==0)  
      {
        if(++timer_control == timer_loop)
          break;
      }
*/

    /*中断控时与背景音乐播放，调用垂直回扫值返回函数，进行计时*/      
    if((inportb(0x3da)&0x08)!=0)
    {
      Music2(music_order,timer_control);  
    }
    else  
      if((inportb(0x3da)&0x08)==0 ) 
      {
        if(++timer_control == timer_loop)
        {
          music_control = (music_control+1)%(10);
          if(music_control == 0)
            music_order = (music_order+1) % (94 + (49*2+47+51+4));
          break;
        }
      }                                         
    } /* while */
    CheckLife();
    if(++layer_counter % 150 == 0)
    {
      if(layer % 10 == 0 && life < 12)    /*每到10层的倍数时，奖2点生命值，若是满血就不奖励了*/
        if(life+2 <= 12) life = life+2;
        else life = 12;
      layer++;
      DrawLayer();
    }
  } ;
}



void NewGame()
{
}

int GetKey()
{
  int conflict;

  if(bioskey(1))
  {
    key = 0;
    key = bioskey(0);
    conflict = IsConflict();
    switch(key)
    {
      case LEFT:
        if(conflict != 1) 
          man_x = man_x - man_move_speed; 
        return key;
        break;
      case RIGHT: 
        if(conflict != 2) 
          man_x = man_x + man_move_speed; 
        return key;
        break;
      case LOWERS: case UPPERS: 
        nosound();
        SaveGame(life, layer, man_x, man_y, px, py, pindex, bottom_num, top_num); 
        setfillstyle(1,BLACK);
        bar(30,460,430,480);
        hanzi("游戏已成功保存",175,462); 
        getch();
        bar(30,460,430,480);
        setcolor(WHITE);
        setfillstyle(1,BLACK);
        hanzi("空格键为暂停",100-50,462);
        settextjustify(1,2);
        settextstyle(2,0,5);
        outtextxy(235-50,463,", S");
        hanzi("键为保存",260-50,462);
        outtextxy(370-50,463,",\"+\"\"-\"");
        hanzi("更改游戏难度",400-50,462);
        break;
      case ESC: 
      nosound();
      return key;
      case SPACE: 
      nosound();
      PauseGame();
      break;
      case PLUS: 
        level = HARD;
        drop_loop = 1500;
        timer_loop = 2500;
        break;
      case MINUS:
        level = EASY;
        drop_loop = 2200;
        timer_loop = 4000; 
        break;    
    }
  }
  return 0;
}

int Check_On_Plank()
{
  int k;
  for(k=0;k<8;k++)
    if(abs(man_x-px[k])<=50 && man_y<=py[k]-19 && man_y>=py[k]-20 && pindex[k]!=2) {
      plank_stand = k;
      return k+1;
    }             /*表示人物在数组编号为k的那块木板上，并返回k+1*/
  plank_stand = -1;
  return 0;     /*表示人物不在木板上，返回0*/
}

void DrawWindow()            /*画游戏窗口*/
{
  setcolor(WHITE);
  rectangle(30,20,430,440);  /*再把框画一遍*/
  setfillstyle(1,BLACK);
  bar(30,440+1,430,440+21);  /*把游戏过程中产生的多余部分用背景色擦掉*/
}

void DrawPlank(int x, int y, int color, int index)   /*画木板*/
{
  int a = y-10, b = y+3;
  int thorns1[6] = {0,0,0,0,0,0};
  int k;

  switch(index)
  {
    case 1:    /*实木板*/
    setfillstyle(1,color);
    bar(x-40,y-10,x+40,y+10);
    break;
    case 2:    /*虚木板*/
    setfillstyle(10,color);
    bar(x-40,y-10,x+40,y+10);
    break;
    case 3:    /*传送带(向左)*/
    setfillstyle(4,color);
    bar(x-40,y-10,x+40,y);
    setfillstyle(5,color);
    bar(x-40,y,x+40,y+10);
    break;
    case 4:    /*传送带(向右)*/
    setfillstyle(5,color);
    bar(x-40,y-10,x+40,y);
    setfillstyle(4,color);
    bar(x-40,y,x+40,y+10);
    break;
    case 5:   /*带刺的木板*/
	setfillstyle(1,RED);
    bar(x-40,y+3,x+40,y+10);
	setcolor(RED);
    thorns1[3] = a;
    thorns1[1] = thorns1[5] = b;
    for(k=0;k<6;k++){
      thorns1[0] = x-40+k*14;
      thorns1[2] = x-35+k*14;
      thorns1[4] = x-30+k*14;
      line(thorns1[0],thorns1[1],thorns1[2],thorns1[3]);
      line(thorns1[2],thorns1[3],thorns1[4],thorns1[5]);
    }/*for*/     
  }
}

void DrawMan(int x, int y, int color)     /*画游戏人物(受硬件条件限制，仅可用木块表示)*/
{
    setfillstyle(1,color);
    bar(x-10,y-10,x+10,y+10);
}

void DrawThorn()         /*画顶端尖刺*/
{
  int i;

  for(i=30+10; i<=430-10; i=i+20)
  {
    thorn[0] = i-10;
    thorn[2] = i+10;
    thorn[4] = i;
    setfillstyle(1,WHITE);
    fillpoly(3,thorn);
  }
}

int IsConflict()   /*判断人物是否移动到了边界，如在边界，则无法继续左右移动*/
{
  switch(key)
  {
	case LEFT:  if(man_x - man_move_speed < 30+10)  return 1;break;
	case RIGHT: if(man_x + man_move_speed > 430-10) return 2;break; 
  }
  return 0;
}

void CheckLife()     /*判断当前生命值，并调用DrawLife()函数显示出人物当前生命值*/
{
  if(man_y <= 45) {
	life -= 3;
	man_y += plank_speed+1;    /*为使人物得以在触到尖刺后下落*/
  }
  if(man_y > 430) life = 0;
  DrawLife();
  if (life <= 0)
  {
    nosound();
    if(GetHiscore(layer) == 0)
      LoseGame();
   /*PauseGame();clrscr();closegraph();*/  /*Temporary Statement调试时用的,要加游戏失败的界面*/
   StartGame(0);
  }
}

void DrawLife()    /*显示人物当前生命值*/
{
  int k;
  int color1;

  if (life < 0)  life = 0;
  if (life >= 6)
	color1 = YELLOW;
  else
	color1 = RED;
  setfillstyle(1,BLACK);
  for(k = 460+life*10; k <= 460+(12-1)*10; k += 10)
	bar(k+1,60+1,k+10-1,80-1);
  setfillstyle(1,color1);
  for(k = 460; k <= 460+(life-1)*10; k += 10)
	bar(k+1,60+1,k+10-1,80-1);
}

void DrawLayer()  /*画出当前下降的层数HORIZ_DIR*/
{
  int buff[100];
  setcolor(BLACK);
  itoa(layer-1,buff,10);
  settextjustify(1,1);
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(580,145,buff);
  setcolor(CYAN);
  itoa(layer,buff,10);
  settextstyle(3,HORIZ_DIR,3);
  outtextxy(580,145,buff);
}

void DrawPauseGame()
{
  int xmin=450,xmax=610,ymin=200,ymax=440;

  settextstyle(2,HORIZ_DIR,5);
  settextjustify(1,1);

  /*画菜单边框*/
  setcolor(WHITE);
  rectangle(xmin,ymin,xmax,ymax);
  

  /*下面画分选项*/
  /*1..回到游戏*/ 
  setcolor(WHITE);
  rectangle(xmin+15,ymin+10-5,xmax-15,ymin+60-5);
  setcolor(BROWN);  
  hanzi("返回游戏",(xmin+xmax)/2-45, ymin+(10-5+60-5)/2-9 );

  /*2..重新开始游戏*/  
  setcolor(WHITE);
  rectangle(xmin+15,ymin+70-5,xmax-15,ymin+120-5);
  setcolor(BROWN);  
  hanzi("重新开始", (xmin+xmax)/2-45, ymin+(70-5+120-5)/2-9);

  /*3..回主界面 或 更改游戏难度并重新开始游戏*/ 
  setcolor(WHITE);
  rectangle(xmin+15,ymin+130-5,xmax-15,ymin+180-5); 
  setcolor(BROWN);
  hanzi("回主菜单",(xmin+xmax)/2-45, ymin+(130-5+180-5)/2-9);  
 
  /*4..退出游戏*/ 
  setcolor(WHITE);
  rectangle(xmin+15,ymin+190-5,xmax-15,ymin+240-5);
  setcolor(BROWN);  
  hanzi("退出游戏",(xmin+xmax)/2-45, ymin+(190-5+240-5)/2-9);
}

int PauseGame()   /*游戏暂停*/
{
  int xmin=450,xmax=610,ymin=200,ymax=440;
  int mouse_key;

  /*擦去排行榜，显示暂停界面*/
  setfillstyle(1,BLACK);
  bar(450,170,640,480);

  /*菜单栏显示的渐变效果*/
  setcolor(WHITE);
  rectangle(xmin+50,ymin+60,xmax-60,ymax-60);
  delay(100);
  setcolor(BLACK);
  rectangle(xmin+50,ymin+60,xmax-60,ymax-60);
  setcolor(WHITE);
  rectangle(xmin+30,ymin+40,xmax-40,ymax-20);
  delay(100);
  setcolor(BLACK);
  rectangle(xmin+30,ymin+40,xmax-40,ymax-20);

  DrawPauseGame();

  MouseMath();
  MouseSetX(xmin+1,xmax-1);
  MouseSetY(ymin+1,ymax-1);
  MouseSetXY(xmin+2,ymin+2);
  while(1) {
    DrawPauseGame();
    mouse_key = MouseStatus();

    /*1..回到游戏*/
    if ((MouseX>=xmin+15)&&(MouseX<=xmax-15)&&(MouseY>=ymin+10-5)&&(MouseY<=ymin+60-5)&&mouse_key==1){
      setfillstyle(1,BLACK);
      bar(xmin-1,ymin-1,xmax+1,ymax+1);
      ShowHiscore();
      setcolor(WHITE);
      setfillstyle(1,BLACK);
      hanzi("空格键为暂停",100-50,462);
      settextjustify(1,2);
      settextstyle(2,0,5);
      outtextxy(235-50,463,", S");
      hanzi("键为保存",260-50,462);
      outtextxy(370-50,463,",\"+\"\"-\"");
      hanzi("更改游戏难度",400-50,462);        
      return 0;
    }

    /*2..重新开始游戏*/
    if ((MouseX>=xmin+15)&&(MouseX<=xmax-15)&&(MouseY>=ymin+70-5)&&(MouseY<=ymin+120-5)&&mouse_key==1){
       closegraph();
       StartGame(0);
    }

    /*3..回主界面 或 更改游戏难度并重新开始游戏*/
    if ((MouseX>=xmin+15)&&(MouseX<=xmax-15)&&(MouseY>=ymin+130-5)&&(MouseY<=ymin+180-5)&&mouse_key==1){
       closegraph();
       DrawMainMenu();
       MenuChoice();
    }

    /*4..退出游戏*/
    if ((MouseX>=xmin+15)&&(MouseX<=xmax-15)&&(MouseY>=ymin+190-5)&&(MouseY<=ymin+240-5)&&mouse_key==1){
       Byebye();
    }
  }
  MouseOff();
  return 0;
}


void LoseGame()
{
  closegraph();
  textmode(C80);
  textattr((0<<7)+LIGHTRED+(BLUE<<4));
  textbackground(BLACK);
  clrscr();
  window(1,1,4,25);
  textbackground(2);
  clrscr();
  window(76,1,79,25);
  textbackground(2);
  clrscr();
  window(5,11,75,13);
  textbackground(1);
  clrscr();
  window(20,6,60,9);
  textbackground(1);
  clrscr() ;

  gotoxy(10,2);
  printf("You've Lost This Game\n");
  gotoxy(6,3);
  printf("Cannot get into the hero rank!");
  gotoxy(5,4);
  printf("Press any key to start a new game");
  getch();
}

