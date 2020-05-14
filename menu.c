#include "menu.h"
void initBrick();/*20200103-张鑫：使用别的c文件中的函数要声明下*/
gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event);
gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event);
extern gboolean time_handler(GtkWidget *widget);
extern gboolean bStop;
extern gboolean bPause;
extern guint nLevel,nLine,nScore;
extern guint g_curbricky;
extern GtkWidget *nextbrick_label,*record_label,*level_label,*score_label,*line_label;
extern gint g_Filled[XMAX][YMAX];
void deal_time();

guint tid = 0;
guint mytimer = 1000;

gint NewGame(GtkWidget *widget,gpointer data){
	 /*2020 张鑫 显示时间信息,但好像受时间变快影响*/
    g_timeout_add(1000, (GSourceFunc)deal_time,NULL);
	
	//20200103-张鑫：解决上次游戏块不清0的bug
	for(gint i=0;i<XMAX;i++) {
		for(gint j=0;j<YMAX;j++) {
			g_Filled[i][j] = 0;
		}
	}
	
	KeyArg *arg = (KeyArg *)data;
    gtk_widget_queue_draw(arg->game_area );
    //////////////////////////////////////////////////////////
	int x,y;
	gchar buffer[20];
	bStop = FALSE;
	mytimer = 1000;
	nLine = 0;
	nScore = 0;
	nLevel = 1;
	g_curbricky = 0;
	for(x = 0;x < XMAX;x++)
		for (y = 0;y < YMAX;y++)
			g_Filled[x][y] = 0;
	initBrick();
	gamearea_configure(arg->game_area,NULL);
	nextbrickarea_configure(arg->nextbrick_area,NULL);

	sprintf(buffer,"Level：1");
	gtk_label_set_text(GTK_LABEL(level_label),buffer);
	sprintf(buffer,"Line：0");
	gtk_label_set_text(GTK_LABEL(line_label),buffer);
	sprintf(buffer,"Score：0");
	gtk_label_set_text(GTK_LABEL(score_label),buffer);

	if(tid)
		g_source_remove (tid);
	tid = g_timeout_add(mytimer,(GSourceFunc)time_handler,(GtkWidget *)arg->window);
	time_handler((GtkWidget *)arg->window);
	return 0;
}

gint CloseContent(GtkWidget *widget,gpointer data) {
	bPause = FALSE;
	gtk_widget_destroy(data);
	return 0;
}

gint HelpContent(GtkWidget *widget,gpointer data) {
}

gint About(GtkWidget *widget,gpointer data) {
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog),"俄罗斯方块");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"0.9");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"张鑫");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"dsfsdfkl");
	gtk_dialog_run(GTK_DIALOG(dialog));
	return 0;
}

