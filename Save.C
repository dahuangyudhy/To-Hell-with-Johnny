/*游戏载入及保存*/

typedef struct game_data_typ
{
  int data_life;
  int data_layer;
  int data_man_x,data_man_y;
  int data_px[8],data_py[8],data_pindex[8];
  int data_bottom_num;
  int data_top_num;
} game_data;


int LoadGame(int *life, int *layer, int *man_x, int *man_y, int *px[], int *py[], int *pindex[], int *bottom_num, int *top_num)
{
  FILE *fs;
  int i;
  game_data saved;

  if ((fs = fopen("f:\\tc\\1\\save.txt", "r")) == NULL){
    printf("Can't open save.txt\n");
    getch();
    exit(1);
  }
  fscanf(fs, "%d %d %d %d %d %d ", &saved.data_life, &saved.data_layer, &saved.data_man_x, &saved.data_man_y, &saved.data_bottom_num, &saved.data_top_num);
  for (i = 0; i < 8; i++)
    fscanf(fs, "%d %d %d ", &saved.data_px[i],&saved.data_py[i],&saved.data_pindex[i]);
  fclose(fs);

  if(saved.data_life == 20) return 0;  
  /*说明以前未保存过游戏，则进行新游戏。设定saved.data_life初始值为20（需手工设定save.txt进行游戏初始化）*/

  /*将已读取的数据赋值给游戏当前数据*/
  *life = saved.data_life;
  *layer = saved.data_layer;
  *man_x = saved.data_man_x;
  *man_y = saved.data_man_y;
  *bottom_num = saved.data_bottom_num;
  *top_num = saved.data_top_num;
  for (i=0; i<8; i++)
  {
    *px[i] = saved.data_px[i];
    *py[i] = saved.data_py[i];
    *pindex[i] = saved.data_pindex[i];
  }
  return 1;
}

void SaveGame(int life, int layer, int man_x, int man_y, int px[], int py[], int pindex[], int bottom_num, int top_num)
{
  FILE *fs;
  int i;
  game_data saved1;

  /*保存当前数据到结构体game_data saved中*/
  saved1.data_life = life;
  saved1.data_layer = layer;
  saved1.data_man_x = man_x;
  saved1.data_man_y = man_y;
  saved1.data_bottom_num = bottom_num;
  saved1.data_top_num = top_num;
  for (i=0; i<8; i++)
  {
    saved1.data_px[i] = px[i];
    saved1.data_py[i] = py[i];
    saved1.data_pindex[i] = pindex[i];
  }


  if ((fs = fopen("f:\\tc\\1\\save.txt", "w")) == NULL){
    printf("Can't open save.txt\n");
    getch();
    exit(1);
  }
  fprintf(fs, "%d %d %d %d %d %d ", saved1.data_life,saved1.data_layer,saved1.data_man_x,saved1.data_man_y,saved1.data_bottom_num,saved1.data_top_num);
  for (i = 0; i < 8; i++)
    fprintf(fs, "%d %d %d ", saved1.data_px[i],saved1.data_py[i],saved1.data_pindex[i]);
  fclose(fs);
  return;
}