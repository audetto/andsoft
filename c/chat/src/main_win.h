#ifndef MAIN_WIN_H
#define MAIN_WIN_H

#include <gtk/gtk.h>

GtkWidget *create_main_win();
void input_func(gpointer data, gint source, GdkInputCondition condition);

#endif
