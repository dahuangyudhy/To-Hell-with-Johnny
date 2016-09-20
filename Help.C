/*游戏简介*/

#include <conio.h>

Help()
{


  /*Music1(2);*/



/*
  以前的版本，
  textmode(C40);
  textbackground(BLACK);
  clrscr();
  window(5,4,35,20);
  textattr(WHITE);
  cputs("  This game is called \"Hundred Steps\", the player should keep up with the lifting planks, or he will die of those thorns. Use left and right button to decide your way.");
  getch();
*/

 
  ShowPicture(2,0,0);
  ShowPicture(3,0,0);
  ShowPicture(4,0,0);
  ShowPicture(5,0,0);
  ShowPicture(6,0,0);
  ShowPicture(7,0,0);
  ShowPicture(8,0,0);
  ShowPicture(9,0,0);
  ShowPicture(10,0,0);
  ShowPicture(11,0,0);

  DrawMainMenu();
  MenuChoice();
}
