#include <dos.h>

#include "f:\tc\1\define.c"
#include "f:\tc\1\chinese.c"
#include "f:\tc\1\mouse.c"
#include "f:\tc\1\hiscore.c"
#include "f:\tc\1\bmp.c"
#include "f:\tc\1\play.c"
#include "f:\tc\1\movie.c"
#include "f:\tc\1\menu.c"
#include "f:\tc\1\music.c"
#include "f:\tc\1\help.c"
#include "f:\tc\1\exit.c"

main()
{
  int gameres;   /*判断游戏输赢，和退出与否*/
  int key;

  LoadingMovie();
  Music1(0);

  DrawMainMenu();
  MenuChoice();
}

