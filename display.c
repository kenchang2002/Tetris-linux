     #include    "display.h"
   
     extern gboolean bStop;
     extern gint g_Filled[XMAX][YMAX];
   
     GdkPixmap *game_pixmap,*nextbrick_pixmap;   /*��Ϸ��ͼ���Լ���ʾ�����λͼ*/
     GdkColor color;
     GdkColormap *colormap;
     GdkGC *gc,*gc1;
   
     /* �����ʵ���С����Ϸ������ʾ�����λͼ */
     gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event)
     {
             if (game_pixmap)
                 gdk_pixmap_unref(game_pixmap);
             game_pixmap = gdk_pixmap_new(widget->window, GAMEAREAWIDTH,
                                                GAMEAREAHEIGHT, -1);
            /*20191227-����:����һ��λͼ��ͼ��������չʾ��Ϸ������*/    
             
             gc =gdk_gc_new(game_pixmap);
             /*20191227-����:λͼ��ɫ���ԣ��������ͼ����*/
             
              if (gdk_color_parse("DarkBlue", &color)) //20191227-����:Ϊcolor������ɫ����ֵ�Ļ�����Ҫִ��gdk_colormap_alloc_color��
              {
                    colormap = gdk_colormap_get_system ();
                    if (gdk_colormap_alloc_color(colormap, &color, FALSE, TRUE))
                        gdk_gc_set_foreground (gc, &color );
              }
              /*20191227-����:colormap����ɫ��Ӧ��ϵ����һ�žͿ����ˣ���������interface.c���ʼ���ģ����Բ��á�*/
   
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
                                        /* 20191227-���Σ���֪��ɶ�ã�ע�͵���Ӱ��*/

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
                    if(g_Filled[x][y]==1){               /*����ռ������*/
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


