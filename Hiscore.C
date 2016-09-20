/*排行榜模块*/


typedef struct hiscore_typ
{
  unsigned char name[8];
  int score;
}hiscore;

hiscore hi[5];             /*记录原排行榜的5个数据*/
hiscore newscore;          /*记录新存入排行榜的数据*/

void LoadHiscore();        /*保存排行榜*/
void SaveHiscore();        /*读取排行榜*/
void ShowHiscore();        /*显示排行榜*/
int  GetHiscore(int x);    /*获取新登榜数据*/
void RankSort();           /*重新排名*/


void LoadHiscore()
{
  FILE *fs;
  int i;

  if ((fs = fopen("f:\\tc\\1\\hiscore.txt", "r+")) == NULL){
    printf("Can't open hiscore.txt\n");
    getch();
    exit(1);
  }
  for (i = 0; i < 5; i++)
    fscanf(fs, "%s %d ", hi[i].name, &hi[i].score);
  fclose(fs);
}

void SaveHiscore()
{
  FILE *fs;
  int i;

  if ((fs = fopen("f:\\tc\\1\\hiscore.txt", "w")) == NULL){
    printf("Can't open hiscore.txt\n");
    getch();
    exit(1);
  }
  for (i = 0; i < 5; i++)
    fprintf(fs, "%s %d ", hi[i].name, hi[i].score);
  fclose(fs);
}

void ShowHiscore()
{
  int i;
  int buff[300];
  int score_temp[5];

  LoadHiscore();
  hanzi("排行榜",450,180);
  hanzi("分数",580,180);
  for(i=0; i<5; i++)
    score_temp[i] = hi[i].score;
  for(i=0; i<5; i++)
  {
    settextjustify(LEFT_TEXT,CENTER_TEXT);   
    settextstyle(3,HORIZ_DIR,3); 
    itoa(i+1,buff,10);
    outtextxy(450,210+55*i,buff);
    outtextxy(470,210+55*i,hi[i].name);
    itoa(hi[i].score,buff,10);
    outtextxy(580,210+55*i,buff);
  }
  for(i=0; i<5; i++)
    hi[i].score = (int)score_temp[i];
}


int GetHiscore(int x)
{
  int i;
  char c;
  int flag=0;
  char buff[100];
  char my_name[3] = {'D','H','y'};

  for(i=0;i<5;i++){
    if(x > hi[i].score){
      flag=1;
    }
  }
  if(flag==0)
    return 0;

  closegraph();
  textattr(128+LIGHTRED+(BLUE<<4));
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
  gotoxy(12,2);
  printf("Congratulations^_^\n");
  gotoxy(4,3);
  printf("you have got into the hero rank!");
  gotoxy(11,4);
  printf("please input your name");

  window(5,11,70,13);
  textattr(LIGHTRED+(BLUE<<4));
  clrscr();
  gotoxy(3,50);
  printf("Please enter no more than 7 alphabets.");
  gotoxy(3,2);

  for(i=0;i<100;i++)
    buff[i] = '\0';
  c = getchar();
  for(i=0; c!='\0';i++)
  {
    if((c<='Z' && c>='A') || (c<='z' && c>='a') ||(c<=9 && c>=0) || c==' ')
      buff[i]=c;
    else if(c==14 && i!=0)
           i -= 2;
         else
           break;
    c = getchar();
  }
  if(i==0) {                            /*假如上榜者不肯留名的话...*/
    strcpy(newscore.name,my_name);      /*我就替他上榜了，嘿嘿^_^*/
    return 1;
  }
  strncpy(newscore.name,buff,7);
  newscore.score=x;
  hi[4]=newscore;
  RankSort();
  return 1;
}



void RankSort()
{
  int i,j;
  hiscore temp;

  for(j=0;j<5;j++)
    for(i=0;i<5-j;i++)
     if(hi[i].score<hi[i+1].score)
     {   
       temp=hi[i];
       hi[i]=hi[i+1];
       hi[i+1]=temp;
    }
  SaveHiscore();
}