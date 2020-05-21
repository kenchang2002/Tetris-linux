/*******************************************************************************
 20200513 张鑫 
 该文件为程序主入口，同时也是主界面初始化的地方。gtk的图形界面如果是用纯编码
 的方式构建，原则上是先将你的界面区域划分成各个小块，最后才组成一个大的。
 *******************************************************************************/

#include "display.h"  
#include "control.h"
#include "menu.h"

GtkWidget *game_area, *nextbrick_area;     /*游戏绘图区以及提示绘图区*/
GtkWidget *nextbrick_label, *record_label,*level_label,*score_label,*line_label ,*time_label;
/*20200513-01 start: 张鑫 在界面增加一个显示游戏时长的小窗口,先在上面成绩去通过time_lable实现*/
 gboolean deal_time();
/*20200513-01 end:                                                                                        */
KeyArg arg;

extern GdkColormap *colormap;

int main( int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox1,*vbox2,*vbox3,*vbox4,*hbox;

    GtkWidget *menubar;
    GtkWidget *gamemenu,  *helpmenu;
    GtkWidget *game,*newgame, *sep, *quit;
    GtkWidget *help,*content, *about;
    GtkAccelGroup *accel_group = NULL;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_default_size(GTK_WINDOW(window), (GAMEAREAWIDTH+NEXTAREAWIDTH), 360);
    gtk_window_set_title(GTK_WINDOW(window), "Tetris - ken");

    vbox1=gtk_vbox_new(FALSE,0);
    vbox2=gtk_vbox_new(FALSE,0);
    /*20191225-张鑫：这里首参数为TRUE表示这个box里面的控件具有相同高度,因此相互有影响*/
    vbox3=gtk_vbox_new(FALSE,0);
    vbox4=gtk_vbox_new(TRUE,0);
    hbox=gtk_hbox_new(TRUE,0);

    menubar = gtk_menu_bar_new();
    /*建立“游戏”,"帮助"菜单项容器  */
    gamemenu = gtk_menu_new();
    helpmenu= gtk_menu_new();

    /*建立快捷键容器*/
    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

    /*“游戏”菜单各菜单项*/
    game = gtk_menu_item_new_with_mnemonic("Game(_G)");
    newgame = gtk_menu_item_new_with_mnemonic("Newgame(_N)");
    sep = gtk_separator_menu_item_new();//分隔线
    quit = gtk_menu_item_new_with_mnemonic("Quit(_Q)");

    /*创建“游戏”菜单各菜单项快捷键*/
    gtk_widget_add_accelerator(newgame, "activate", accel_group,
      GDK_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(quit, "activate", accel_group,
      GDK_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    /*"游戏"菜单各菜单项与“游戏”菜单关联*/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(game), gamemenu);
    gtk_menu_append(GTK_MENU_SHELL(gamemenu), newgame);
    gtk_menu_append(GTK_MENU_SHELL(gamemenu), sep);
    gtk_menu_append(GTK_MENU_SHELL(gamemenu), quit);

    /*连接“游戏”菜单项处理函数*/
    g_signal_connect (G_OBJECT(newgame),"activate",
                                    G_CALLBACK(NewGame), &arg);
    g_signal_connect(G_OBJECT(quit), "activate",
                                    G_CALLBACK(gtk_main_quit), NULL);

    /*创建“帮助”菜单各菜单项*/
    help= gtk_menu_item_new_with_mnemonic("Help(_H)");
    content= gtk_menu_item_new_with_mnemonic("Content(_C)");
    sep = gtk_separator_menu_item_new();
    about= gtk_menu_item_new_with_mnemonic("About(_A)");

    /*"帮助"菜单各菜单项与“帮助”菜单关联*/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
    gtk_menu_append(GTK_MENU_SHELL(helpmenu), content);
    gtk_menu_append(GTK_MENU_SHELL(helpmenu),sep);
    gtk_menu_append(GTK_MENU_SHELL(helpmenu), about);

    g_signal_connect (G_OBJECT(content),"activate",
                                    G_CALLBACK(HelpContent), NULL);
    g_signal_connect (G_OBJECT(about),"activate",
                                    G_CALLBACK(About), NULL);
    /*把菜单放到菜单栏上*/
    gtk_menu_bar_append(GTK_MENU_SHELL(menubar), game);
    gtk_menu_bar_append(GTK_MENU_SHELL(menubar), help);
    /*将菜单栏组装到vbox1*/
    gtk_box_pack_start(GTK_BOX(vbox1), menubar, FALSE, FALSE, 0);
    /*20191225-张鑫：这里最后一个参数20表示vbox和窗口vbox1上边缘之间的距离是20个像素*/

    /*创建游戏绘图区*/
    game_area=gtk_drawing_area_new();
    gtk_drawing_area_size ((GtkDrawingArea *)game_area,GAMEAREAWIDTH,GAMEAREAHEIGHT);
    /*将绘图区组装到水平盒子*/
    gtk_box_pack_start(GTK_BOX(hbox), game_area, FALSE, FALSE, 10);

    /*下一方块提示区*/
    nextbrick_label=gtk_label_new("next brick");
    gtk_box_pack_start(GTK_BOX(vbox3), nextbrick_label, TRUE, TRUE, 0);

    /*创建显示下一方块绘图区*/
    nextbrick_area=gtk_drawing_area_new();
    gtk_drawing_area_size ((GtkDrawingArea *)nextbrick_area,NEXTAREAWIDTH,NEXTAREAHEIGHT);

    gtk_box_pack_start(GTK_BOX(vbox3), nextbrick_area, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox2), vbox3, FALSE, FALSE, 0);

    /*成绩区*/
    record_label=gtk_label_new("Record:");
    level_label=gtk_label_new("Level:");
    line_label=gtk_label_new("Line:");
    score_label=gtk_label_new("Score:");
   /*20200513 start :张鑫 改变标签颜色*/
    time_label=gtk_label_new("Time:");
    GdkColor color;
    gdk_color_parse ("blue", &color);
    gtk_widget_modify_fg(time_label, GTK_STATE_NORMAL, &color);
   
    

    gtk_box_pack_start(GTK_BOX(vbox4), record_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox4), level_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox4), line_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox4), score_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox4), time_label, FALSE, FALSE, 10);

    gtk_box_pack_start(GTK_BOX(vbox2), vbox4, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox, FALSE, FALSE, 20);
    gtk_container_add(GTK_CONTAINER(window), vbox1);

    //colormap=gtk_widget_get_colormap(game_area); 20191227-张鑫：换到display.c去

    gtk_widget_set_events (game_area, GDK_STRUCTURE_MASK|
                        GDK_EXPOSURE_MASK
                        |GDK_KEY_PRESS_MASK);

    g_signal_connect (G_OBJECT(game_area),"configure_event",
                                        G_CALLBACK(gamearea_configure), NULL);
    g_signal_connect (G_OBJECT (game_area), "expose_event",
                                         G_CALLBACK( gamearea_expose), NULL);
    g_signal_connect (G_OBJECT(nextbrick_area),"configure_event",
                                        G_CALLBACK(nextbrickarea_configure), NULL);
    g_signal_connect (G_OBJECT (nextbrick_area), "expose_event",
                                          G_CALLBACK( nextbirckarea_expose), NULL);
    arg.window=window;
    arg.game_area=game_area;
    arg.nextbrick_area=nextbrick_area;
    g_signal_connect(G_OBJECT(window),"key-press-event",
                                        G_CALLBACK(KeyPress),(void*)&arg);

    g_signal_connect_swapped(G_OBJECT(window), "delete_event",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));
   

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
