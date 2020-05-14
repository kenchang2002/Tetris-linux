 #ifndef     _CONTROL_H
  #define     _CONTROL_H
  #include <gtk/gtk.h> 
  #include "global.h"  
  gboolean time_handler(GtkWidget *widget);
  gint PositionCorrect(gint newind,gint srcx, gint srcy);
  gint KeyPress(GtkWidget *widget, GdkEventKey *event, gpointer arg);
  void initBrick();
  #endif



