   #include "control.h"
  gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event);
  gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event);
  extern guint tid;
  extern guint mytimer;
  extern GtkWidget *level_label,*score_label,*line_label,*time_label;

  gint g_Filled[XMAX][YMAX];  /*方块地图，标记游戏区域中哪里有方块，哪里没有*/
  gboolean bStop=TRUE;        /*是否游戏结束*/
  gboolean bPause=FALSE;    /*是否游戏暂停*/
  guint nLevel,nLine,nScore;     /*成绩区的显示的内容*/


  guint g_count =0;

  Brick g_allbrick[ NUMBRICK];    /*???��??��????????*/
  Brick g_curbrick;                       /*??????��?????*/
  Brick g_nextbrick;                      /*??????��?????*/

  gint g_curbrickind;             /*??????????????*/
  gint g_nextbrickind;           /*??????????????*/
  gint g_curbrickx=3;             /* ??????��??x????*/
  gint g_curbricky=0;             /* ??????��??y????*/
  
  /*????????????????*/
 gboolean deal_time() {
    if(bPause == FALSE && bStop == FALSE)
         g_count++;
    gchar buffer[20];
    sprintf (buffer, "time:%d seconds",g_count);
    gtk_label_set_text(GTK_LABEL(time_label),buffer);
    return TRUE;
  }


  gboolean time_handler(GtkWidget *widget)
  {
     GdkEvent t_event;
    if (widget->window == NULL || TRUE==bStop)
        return FALSE;
      t_event.type=GDK_KEY_PRESS;
      ((GdkEventKey*)&t_event)->window=widget->window;
      ((GdkEventKey*)&t_event)->send_event=TRUE;
      ((GdkEventKey*)&t_event)->time=0;
      ((GdkEventKey*)&t_event)->state=0;
      ((GdkEventKey*)&t_event)->keyval=GDK_Down;
      ((GdkEventKey*)&t_event)->length=4;
      ((GdkEventKey*)&t_event)->string="Down";
      ((GdkEventKey*)&t_event)->hardware_keycode=0;
      ((GdkEventKey*)&t_event)->group=0;
      gdk_event_put(&t_event);
      return TRUE;
  }

  ///////==========

  /*检测方块是否到达边界；目标区域是否已有方块*/
  gint PositionCorrect(gint newind,gint srcx, gint srcy)
  {

      gint i, sum=0,ind=g_curbrick.index;
      gint x1,y1,x, y;

      /*把方块当前区域清除*/
      for(i=0;i<4;i++){
          x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
          y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
          g_Filled[x][y]=0;
      }
      /*目标区域标记为当前方块，判断在目标区域内是否已有方块以及是否到达边界*/
      for(i=0;i<4;i++){
          x1=g_curbrick.brick[newind].blockpos[i].x;
          y1=g_curbrick.brick[newind].blockpos[i].y;
          x1+=srcx;
          y1+=srcy;
          if(x1<0||x1>=XMAX||y1<0||y1>=YMAX)
              sum++;
          else
              sum+=g_Filled[x1][y1];
      }

      /* ??????�z??????????*/
      for(i=0;i<4;i++){
          x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
          y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
          g_Filled[x][y]=1;
      }
      /* ?????��??? */
      if(sum>=1)
          return 0;  //???????
      else
          return 1;
  }

  ///---==============

gint KeyPress(GtkWidget *widget, GdkEventKey *event, gpointer arg)
{
    gint ind,oldind,x,y,ret, i;
    Position oldstart,oldend,newstart,newend;
    GdkEvent t_event;
    KeyArg *arg1=arg;
    oldstart.x=g_curbrickx;
    oldstart.y=g_curbricky;
    ind=g_curbrick.index;
    oldind=g_curbrick.index;
    oldend=g_curbrick.brick[ind].endpos;
    oldend.x+=g_curbrickx;
    oldend.y+=g_curbricky;
    ret=0;

    if(bPause==TRUE && event->keyval!=GDK_Return)
    return  0;
    switch(event->keyval)
    {
        case GDK_Return:
              if(bPause==FALSE)
                 bPause=TRUE;
              else
                 bPause=FALSE;
              break;

         case GDK_Up:
         {
             ind=(g_curbrick.index+1)%4;
             ret=PositionCorrect(ind,g_curbrickx, g_curbricky);
             if(ret==1){
                 for(i=0;i<4;i++){
                      x=g_curbrick.brick[oldind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[oldind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=0;
                  }
                  g_curbrick.index=ind;   //??????index
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=1;
                  }
              }
          }
          break;

          case GDK_Down:
          {
              ret=PositionCorrect(ind,g_curbrickx, g_curbricky+1);
              if(ret==1){
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=0;
                  }
                   g_curbricky++;
                 for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=1;
                  }
              }
              else{   /*??????*/
                  ret=1;
                  if(0==g_curbricky)  //?????????????????
                  {
                      bStop=TRUE;
                      GtkWidget *dialog;
                      dialog = gtk_message_dialog_new((GtkWindow *)widget,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_OK,
                                  "GAME OVER!");
                      gtk_window_set_title(GTK_WINDOW(dialog), "GAME OVER");
                      gtk_dialog_run(GTK_DIALOG(dialog));
                      gtk_widget_destroy(dialog);
                      gamearea_configure(arg1->game_area,NULL);
                      nextbrickarea_configure(arg1->nextbrick_area,NULL);
                      return  1;
                  }

                  /*?????????????????��???????????????????*/
                  gint srcy=YMAX-1;
                  gint dsty=YMAX-1;
                  guint fullRow = 0;    //
                  for(y=YMAX-1;y>=0;y--){
                        if(srcy!=dsty){                           /*???????*/
                                  for( x=0;x<XMAX;x++){      /*????????????????*/
                                      g_Filled[x][dsty]=g_Filled[x][srcy];
                                  }
                          }
                        srcy--;
                        /*?????????????????*/
                        gint sumrow=0;
                        for( x=0;x<XMAX;x++){
                              sumrow+=g_Filled[x][dsty];
                        }
                        if(sumrow<XMAX)   /*????��?????*/
                              dsty--;
                        else
                              ++fullRow;
                     }
                  for(y=0;y<=dsty;y++){
                      for(x=0;x<XMAX;x++)
                          g_Filled[x][y]=0;
                  }
                  /*??????????????��????��???*/
                  switch (fullRow)
                  {
                      case 0:
                          break;
                      case 1:
                          nScore += ONEROWSCORE;
                          break;
                      case 2:
                          nScore += TWOROWSCORE;
                      break;
                      case 3:
                          nScore += THREEROWSCORE;
                          break;
                      case 4:
                          nScore += FOURROWSCORE;
                          break;
                      default:
                          g_print("WTF\n");
                          return -1;
                  }
                   nLine+=fullRow;  //????????
                  if(fullRow!=0) {
                  if(nLine<=2)
                  {
                      nLevel=1;
                      mytimer=1000;
                    }
                  else if(nLine<=4)
                  {
                      nLevel=2;
                      mytimer=800;
                  }
                  else if(nLine <=6)
                  {
                      nLevel=3;
                      mytimer=600;
                  }
                   else if(nLine<=8)
                  {
                      nLevel=4;
                      mytimer=400;
                  }
                   else
                   {
                       nLevel=5;
                       mytimer=200;
                   }
                  if(tid)
                              g_source_remove  (tid);
                   tid=g_timeout_add(mytimer, (GSourceFunc) time_handler, (GtkWidget *) arg1->window);
                   time_handler((GtkWidget *)arg1->window);
                  
                  }
                  gchar buffer[20];
                  sprintf (buffer, "等级 :%d",nLevel);
                  gtk_label_set_text(GTK_LABEL(level_label),buffer);
                  sprintf (buffer, "已消除行 :%d",nLine);
                  gtk_label_set_text(GTK_LABEL(line_label),buffer);
                  sprintf (buffer, "分数 :%d",nScore);
                  gtk_label_set_text(GTK_LABEL(score_label),buffer);

                  g_curbrickind=g_nextbrickind;
                  g_curbrick=g_allbrick[g_curbrickind];
                  g_nextbrickind=g_random_int_range(0, NUMBRICK);
                  g_nextbrick=g_allbrick[g_nextbrickind];
                  ind=g_curbrick.index;
                  g_curbrickx=3;
                  g_curbricky=0;
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=1;
                  }
                  oldstart.x=0;oldstart.y=0;
                  oldend.x=XMAX-1;oldend.y=YMAX-1;
              }
           }
           break;

          case GDK_Right:
          {
              ret=PositionCorrect(ind,g_curbrickx+1, g_curbricky);
              if(ret==1){
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=0;
                  }
                  g_curbrickx++;
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=1;
                  }
              }
          }
          break;

          case GDK_Left:
          {
              ret=PositionCorrect(ind,g_curbrickx-1, g_curbricky);
              if(ret==1){
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=0;
                  }
                  g_curbrickx--;
                  for(i=0;i<4;i++){
                      x=g_curbrick.brick[ind].blockpos[i].x+g_curbrickx;
                      y=g_curbrick.brick[ind].blockpos[i].y+g_curbricky;
                      g_Filled[x][y]=1;
                  }
              }
          }
          break;
      }
      if(ret==1){
          newstart.x=g_curbrickx;
          newstart.y=g_curbricky;
          ind=g_curbrick.index;
          newend=g_curbrick.brick[ind].endpos;
          newend.x=newend.x+newstart.x;
          newend.y=newend.y+newstart.y;
          if(newstart.x>oldstart.x)    newstart.x=oldstart.x;
          if(newstart.y>oldstart.y)    newstart.y=oldstart.y;
          if(newend.x<oldend.x)   newend.x=oldend.x;
          if(newend.y<oldend.y)   newend.y=oldend.y;

          /*????????*/
          t_event.type=GDK_EXPOSE;
          ((GdkEventExpose*)&t_event)->window=((GtkWidget* )arg1->game_area)->window;
          ((GdkEventExpose*)&t_event)->send_event=TRUE;
          ((GdkEventExpose*)&t_event)->area.x=newstart.x*BLOCKWIDTH;
          ((GdkEventExpose*)&t_event)->area.y=newstart.y*BLOCKHEIGHT;
          ((GdkEventExpose*)&t_event)->area.width=(newend.x-newstart.x+1)*BLOCKWIDTH;
          ((GdkEventExpose*)&t_event)->area.height=(newend.y-newstart.y+1)*BLOCKHEIGHT;
          ((GdkEventExpose*)&t_event)->region=gdk_region_rectangle(&((GdkEventExpose*)&t_event)->area);
          ((GdkEventExpose*)&t_event)->count=0;
          gdk_event_put(&t_event);

          /* ???????????? */
          t_event.type=GDK_EXPOSE;
          ((GdkEventExpose*)&t_event)->window=((GtkWidget* )arg1->nextbrick_area)->window;
          ((GdkEventExpose*)&t_event)->send_event=TRUE;
          ((GdkEventExpose*)&t_event)->area.x=0;
          ((GdkEventExpose*)&t_event)->area.y=0;
          ((GdkEventExpose*)&t_event)->area.width=NEXTAREAWIDTH;
          ((GdkEventExpose*)&t_event)->area.height=NEXTAREAHEIGHT;
          ((GdkEventExpose*)&t_event)->region=gdk_region_rectangle(&((GdkEventExpose*)&t_event)->area);
          ((GdkEventExpose*)&t_event)->count=0;
          gdk_event_put(&t_event);
      }
      return 0;
  }


  ///=====================
  void initBrick()
  {
      /*??????4???????4??��?????????????????????????????��?��????��??*/
      gint    par[NUMBRICK][48]={{0,1,1,1,2,1,1,0,0,0,2,1,\
                                  0,0,0,1,0,2,1,1,0,0,1,2,\
                                   0,0,1,0,2,0,1,1,0,0,2,1,\
                                   1,0,1,1,1,2,0,1,0,0,1,2},\

                                  {0,0,1,0,2,0,3,0,0,0,3,0,\
                                   0,0,0,1,0,2,0,3,0,0,0,3,\
                                   0,0,1,0,2,0,3,0,0,0,3,0,\
                                   0,0,0,1,0,2,0,3,0,0,0,3},\

                                  {0,1,1,1,2,1,2,0,0,0,2,1,\
                                   0,0,0,1,0,2,1,2,0,0,1,2,\
                                   0,0,1,0,2,0,0,1,0,0,2,1,\
                                   0,0,1,0,1,1,1,2,0,0,1,2},\

                                  {0,0,1,0,0,1,1,1,0,0,1,1,\
                                   0,0,0,1,1,0,1,1,0,0,1,1,\
                                   0,0,1,0,0,1,1,1,0,0,1,1,\
                                   0,0,0,1,1,0,1,1,0,0,1,1},\

                                  {0,0,0,1,1,1,1,2,0,0,1,2,\
                                   1,0,2,0,0,1,1,1,0,0,2,1,\
                                   0,0,0,1,1,1,1,2,0,0,1,2,\
                                   1,0,2,0,0,1,1,1,0,0,2,1},

                                   {0,0,0,1,1,1,2,1,0,0,2,1,\
                                    1,0,0,0,0,1,0,2,0,0,1,2,\
                                    0,0,1,0,2,0,2,1,0,0,2,1,\
                                    1,0,1,1,1,2,0,2,0,0,1,2},\

                                   {1,0,1,1,0,1,0,2,0,0,1,2,\
                                    0,0,1,0,1,1,2,1,0,0,2,1,\
                                    1,0,1,1,0,1,0,2,0,0,1,2,\
                                    0,0,1,0,1,1,2,1,0,0,2,1}} ;
       int i,j,k,l;
       for(i=0;i<NUMBRICK;i++)
       {
           g_allbrick[i].index=0;
           for(j=0;j<4;j++)
           {
                for(k=0;k<4;k++)
              {
                  g_allbrick[i].brick[j].blockpos[k].x= par[i][j*12+2*k];
                  g_allbrick[i].brick[j].blockpos[k].y= par[i][j*12+2*k+1];
              }
              g_allbrick[i].brick[j].startpos.x=par[i][j*12+8];
              g_allbrick[i].brick[j].startpos.y=par[i][j*12+9];
              g_allbrick[i].brick[j].endpos.x=par[i][j*12+10];
              g_allbrick[i].brick[j].endpos.y=par[i][j*12+11];
           }
       }
      /*?څ??????��???????*/
      g_curbrickind=g_random_int_range(0, NUMBRICK);
      g_curbrick=g_allbrick[g_curbrickind];
      g_nextbrickind=g_random_int_range(0, NUMBRICK);
      g_nextbrick=g_allbrick[g_nextbrickind];
   }

