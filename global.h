       #ifndef _GLOBAL_H
       #define _GLOBAL_H
       //#include <gnome.h>  
       #include <gtk/gtk.h>
       #include <gdk/gdkkeysyms.h> 
       #define XMAX    10                  /*��Ϸ����X�������ֵ*/
       #define YMAX    20                   /*��Ϸ����Y�������ֵ*/
       #define BLOCKWIDTH 30       /*������ÿһС����Ŀ��*/
       #define BLOCKHEIGHT 30       /*������ÿһС����ĸ߶�*/
       /*��Ϸ�����Ⱥ͸߶�*/
     #define GAMEAREAWIDTH ( XMAX*BLOCKWIDTH)
      #define GAMEAREAHEIGHT ( YMAX*BLOCKHEIGHT)
     /*��һ������ʾ����Ⱥ͸߶�*/
     #define NEXTAREAWIDTH   (BLOCKWIDTH*6)
     #define NEXTAREAHEIGHT  (BLOCKHEIGHT*6)
    #define NUMBRICK   7                 /*�����ܵ�������*/

     /*�ֱ�����1��2��3��4�����÷���*/
     #define ONEROWSCORE        1
     #define TWOROWSCORE       3
     #define THREEROWSCORE    7
     #define FOURROWSCORE     13
     typedef struct _Position {
         gint x;
         gint y;
      } Position;
      /*������4��С������������Լ��������ھ���������ʼλ��*/
      typedef struct _block {
         Position blockpos[4];
          Position startpos;
          Position endpos;
      } Block;
      /*ÿ�ַ����4����̬*/
      typedef struct _brick{
          Block brick[4];
          gint index;
      }Brick;
     /*��������������Լ���ͼ������һ������ʾ��ͼ������*/
      typedef struct _KeyArg {
         GtkWidget *window;
          GtkWidget *game_area;
          GtkWidget *nextbrick_area;
      } KeyArg;
      #endif
