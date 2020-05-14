       #ifndef _GLOBAL_H
       #define _GLOBAL_H
       //#include <gnome.h>  
       #include <gtk/gtk.h>
       #include <gdk/gdkkeysyms.h> 
       #define XMAX    10                  /*游戏区域X坐标最大值*/
       #define YMAX    20                   /*游戏区域Y坐标最大值*/
       #define BLOCKWIDTH 30       /*方块中每一小方块的宽度*/
       #define BLOCKHEIGHT 30       /*方块中每一小方块的高度*/
       /*游戏区域宽度和高度*/
     #define GAMEAREAWIDTH ( XMAX*BLOCKWIDTH)
      #define GAMEAREAHEIGHT ( YMAX*BLOCKHEIGHT)
     /*下一方块提示区宽度和高度*/
     #define NEXTAREAWIDTH   (BLOCKWIDTH*6)
     #define NEXTAREAHEIGHT  (BLOCKHEIGHT*6)
    #define NUMBRICK   7                 /*方块总的类型数*/

     /*分别消掉1、2、3、4行所得分数*/
     #define ONEROWSCORE        1
     #define TWOROWSCORE       3
     #define THREEROWSCORE    7
     #define FOURROWSCORE     13
     typedef struct _Position {
         gint x;
         gint y;
      } Position;
      /*方块中4个小方块各自坐标以及方块所在矩形区域起始位置*/
      typedef struct _block {
         Position blockpos[4];
          Position startpos;
          Position endpos;
      } Block;
      /*每种方块的4种形态*/
      typedef struct _brick{
          Block brick[4];
          gint index;
      }Brick;
     /*保存程序主窗口以及绘图区和下一方块提示绘图区参数*/
      typedef struct _KeyArg {
         GtkWidget *window;
          GtkWidget *game_area;
          GtkWidget *nextbrick_area;
      } KeyArg;
      #endif
