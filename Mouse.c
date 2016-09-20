#include <dos.h>
#include <graphics.h>

  int   MouseX;
  int   MouseY;


  int   up[16][16],down[16][16],mouse_draw[16][16],pixel_save[16][16];



void   MouseMath()              /*������������*/
{int   i,j,jj,k;
long   UpNum[16]={
              0x3fff,0x1fff,0x0fff,0x07ff,
              0x03ff,0x01ff,0x00ff,0x007f,
              0x003f,0x00ff,0x01ff,0x10ff,
              0x30ff,0xf87f,0xf87f,0xfc3f
              };
long       DownNum[16]={
              0x0000,0x7c00,0x6000,0x7000,
              0x7800,0x7c00,0x7e00,0x7f00,
              0x7f80,0x7e00,0x7c00,0x4600,
              0x0600,0x0300,0x0300,0x0180
            };
      for(i=0;i<16;i++)
      {
      j=jj=15;
        while(UpNum[i]!=0)
        {
        up[i][j]=UpNum[i]%2;
        j--;
        UpNum[i]/=2;
        }
      while(DownNum[i]!=0){
        down[i][jj--]=DownNum[i]%2;
        DownNum[i]/=2;
        }
    for(k=j;k>=0;k--)
        up[i][k]=0;
    for(k=jj;k>=0;k--)
        down[i][k]=0;
    for(k=0;k<16;k++){                     /*������Ϸ�ʽ*/
        if(up[i][k]==0&&down[i][k]==0)
          mouse_draw[i][k]=1;
        else   if(up[i][k]==0&&down[i][k]==1)
          mouse_draw[i][k]=2;
        else   if(up[i][k]==1&&down[i][k]==0)
          mouse_draw[i][k]=3;
        else
          mouse_draw[i][k]=4;
    }
}
   
mouse_draw[1][2]=4;/*�����*/
}


  /*�������ʾ*/
void   MouseOn(int   x,int   y)
{
  int   i,j;
  int   color;
/*�����*/
for(i=0;i<16;i++){
    for(j=0;j<16;j++){
      pixel_save[i][j]=getpixel(x+j,y+i);/*����ԭ������ɫ*/
        if(mouse_draw[i][j]==1)
          putpixel(x+j,y+i,0); 
        else   if(mouse_draw[i][j]==2)
          putpixel(x+j,y+i,15); 
        }   
      }   
    }   



  /*�������*/   
void   MouseOff(){
int   i,j,x,y,color;   
x=MouseX;   
y=MouseY; 
for(i=0;i<16;i++)/*ԭλ�������ȥ*/
    for(j=0;j<16;j++){ 
        if(mouse_draw[i][j]==3||mouse_draw[i][j]==4)
          continue; 
        color=getpixel(x+j,y+i); 
        putpixel(x+j,y+i,color^color); 
        putpixel(x+j,y+i,pixel_save[i][j]); 
        }
}   


/*���״ֵ̬��ʼ��*/
void   MouseReset(){
      _AX=0x00;
      geninterrupt(0x33);
}   

    
/*����������ұ߽� lx:��߽� gx:�ұ߽�*/   
void   MouseSetX(int   lx,int   rx){   
      _CX=lx;   
      _DX=rx;   
      _AX=0x07;   
      geninterrupt(0x33);
}   
    

/*����������±߽� uy:�ϱ߽� dy:�±߽�*/   
void   MouseSetY(int   uy,int   dy){
      _CX=uy;   
      _DX=dy;   
      _AX=0x08;   
      geninterrupt(0x33);   
}

    
/*������굱ǰλ�� x:�������� y:��������*/   
void   MouseSetXY(int   x,int   y){   
      _CX=x;
      _DX=y;
      _AX=0x04;   
      geninterrupt(0x33);   
}   

/*��ȡ��갴�¼�����Ϣ:�Ƿ������  1=����   0=�ͷ�*/   
  
int   LeftPress(){
      _AX=0x03;   
      geninterrupt(0x33);
      return(_BX&1);   
}   



/*��ȡ��굱ǰλ��*/
void   MouseGetXY(){
      _AX=0x03;
      geninterrupt(0x33);
      MouseX=_CX;
      MouseY=_DX;
}


/*��갴�����*/
int   MouseStatus()
{
int   x,y;
int   status;
int   press=0;

int   i,j,color;
status=0;                               /*Ĭ�����û���ƶ�*/


x=MouseX;
y=MouseY;

while(x==MouseX&&y==MouseY&&status==0&&press==0){
if(LeftPress()) press=1;
                                    /*if(LeftPress()&&RightPress())
                                                       press=1;
                                            else   if(LeftPress())
                                                       press=2;
                                            else   if(RightPress())
                                                       press=3;*/
MouseGetXY();
if(MouseX!=x||MouseY!=y)
    status=1;
}
if(status)                             /*�ƶ������������ʾ���*/
{
for(i=0;i<16;i++)                      /*ԭλ�������ȥ*/
    for(j=0;j<16;j++)
        {
        if(mouse_draw[i][j]==3||mouse_draw[i][j]==4)
          continue;
        color=getpixel(x+j,y+i);
        putpixel(x+j,y+i,color^color);
        putpixel(x+j,y+i,pixel_save[i][j]);
        }
MouseOn(MouseX,MouseY);               /*��λ����ʾ*/
}
if(press!=0)                          /*�а��������*/
     return   press;
return   0;                           /*ֻ�ƶ������*/
}



