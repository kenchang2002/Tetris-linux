     #include    "display.h"
   
     extern gboolean bStop;
     extern gint g_Filled[XMAX][YMAX];
   
     GdkPixmap *game_pixmap,*nextbrick_pixmap;   /*游戏绘图区以及提示区后端位图*/
     GdkColor color;
     GdkColormap *colormap;
     GdkGC *gc,*gc1;
   
     /* 创建适当大小的游戏区及提示区后端位图 */
     gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event)
     {
             if (game_pixmap)
                 gdk_pixmap_unref(game_pixmap);
             game_pixmap = gdk_pixmap_new(widget->window, GAMEAREAWIDTH,
                                                GAMEAREAHEIGHT, -1);
            /*20191227-张鑫:创建一个位图绘图区，用来展示游戏主区域*/    
             
             gc =gdk_gc_new(game_pixmap);
             /*20191227-张鑫:位图颜色属性，和上面绘图区绑定*/
             
              if (gdk_color_parse("DarkBlue", &color)) //20191227-张鑫:为color分析颜色，赋值的话必须要执行gdk_colormap_alloc_color。
              {
                    colormap = gdk_colormap_get_system ();
                    if (gdk_colormap_alloc_color(colormap, &color, FALSE, TRUE))
                        gdk_gc_set_foreground (gc, &color );
              }
              /*20191227-张鑫:colormap是颜色对应关系表。有一张就可以了，这里是在interface.c里初始化的，可以不用。*/
   
              gc1 =gdk_gc_new(game_pixmap);
               if (gdk_color_parse("white", &color))
                 {
                     if (gdk_colormap_alloc_color(colormap, &color, FALSE, TRUE))
                         gdk_gc_set_foreground (gc1, &color );
                 }
              gdk_draw_rectangle (game_pixmap,  widget->style->white_gc, FALSE,  0, 0,
                                  GAMEAREAWIDTH, GAMEAREAHEIGHT);
             gdk_draw_pixmap(widget->window, widget->style->white_gc, game_pixmap,0,0,0,0,
                                         GAMEAREAWIDTH, GAMEAREAHEIGHT);
                                        /* 20191227-张鑫：不知有啥用，注释掉不影响*/

             return TRUE;
     }

     //======================

     gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event)
     {
          if (nextbrick_pixmap)
                 gdk_pixmap_unref(nextbrick_pixmap);
          nextbrick_pixmap = gdk_pixmap_new(widget->window, GAMEAREAWIDTH,
                                      GAMEAREAHEIGHT, -1);
          gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
                                  NEXTAREAWIDTH, NEXTAREAHEIGHT);                                  
          gdk_draw_pixmap(widget->window, widget->style->white_gc, nextbrick_pixmap, 0 ,0, 0, 0,
                           NEXTAREAWIDTH, NEXTAREAHEIGHT);
          return TRUE;
     }


     //=====================
     gint  gamearea_expose (GtkWidget *widget, GdkEventExpose *event)
     {
   
         gint x,y;
         gint srcx, srcy, destx, desty;
         if(FALSE==bStop)
         {
            srcx=event->area.x/BLOCKWIDTH;
            srcy=event->area.y/BLOCKHEIGHT;
            destx=(event->area.x+event->area.width-1)/BLOCKWIDTH;
            desty=(event->area.y+event->area.height-1)/BLOCKHEIGHT;
            for(y=srcy;y<=desty;y++){
                for(x=srcx;x<=destx;x++){
                    if(g_Filled[x][y]==1){               /*方块占据区域*/
                        gdk_draw_rectangle (game_pixmap,gc, TRUE,
                                          x*BLOCKWIDTH,y*BLOCKHEIGHT,
                                          BLOCKWIDTH, BLOCKHEIGHT);
                        gdk_draw_rectangle (game_pixmap,gc1, FALSE,
                                          x*BLOCKWIDTH,y*BLOCKHEIGHT,
                                          BLOCKWIDTH, BLOCKHEIGHT);
                     }
                    else{
                          gdk_draw_rectangle (game_pixmap,widget->style->white_gc, TRUE,
                                                x*BLOCKWIDTH,y*BLOCKHEIGHT,
                                                BLOCKWIDTH, BLOCKHEIGHT);
                   }
               }
            }
         }
         else
             gdk_draw_rectangle (game_pixmap,  widget->style->white_gc, TRUE,  0, 0,
                               GAMEAREAWIDTH, GAMEAREAHEIGHT);
        gdk_draw_pixmap(widget->window, widget->style->white_gc, game_pixmap,
                            event->area.x, event->area.y, event->area.x, event->area.y,
                            event->area.width, event->area.height);
   
        return FALSE;
     }

     //==============
     gint  nextbirckarea_expose (GtkWidget *widget, GdkEventExpose *event)
     {
           gint g_x=2,g_y=2,x1,y1,index;
           extern Brick g_nextbrick;
   
             if(FALSE==bStop) {
                  gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
                                      NEXTAREAWIDTH, NEXTAREAHEIGHT);
                 for(index=0;index<4;index++)
                 {
                     x1=(g_nextbrick.brick[0].blockpos[index].x+g_x)*BLOCKWIDTH;
                     y1=(g_nextbrick.brick[0].blockpos[index].y+g_y)*BLOCKHEIGHT;
                     gdk_draw_rectangle (nextbrick_pixmap,gc, TRUE, x1,y1,
                                             BLOCKWIDTH, BLOCKHEIGHT);
                     gdk_draw_rectangle (nextbrick_pixmap,gc1, FALSE, x1,y1,
                                             BLOCKWIDTH, BLOCKHEIGHT);
                 }
             }
             else
                gdk_draw_rectangle (nextbrick_pixmap,  widget->style->white_gc, TRUE,  0, 0,
                                           NEXTAREAWIDTH, NEXTAREAHEIGHT);
            gdk_draw_pixmap(widget->window, widget->style->white_gc, nextbrick_pixmap,
                                   event->area.x, event->area.y, event->area.x, event->area.y,
                                   event->area.width, event->area.height);
          return FALSE;
     }


