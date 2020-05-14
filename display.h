     #ifndef _DISPLAY_H
     #define _DISPLAY_H
     #include <gtk/gtk.h> 
     #include   "global.h" 
     gint gamearea_configure (GtkWidget *widget, GdkEventConfigure *event);
     gint nextbrickarea_configure (GtkWidget *widget, GdkEventConfigure *event);
     gint  gamearea_expose (GtkWidget *widget, GdkEventExpose *event);
     gint  nextbirckarea_expose (GtkWidget *widget, GdkEventExpose *event);
     #endif

