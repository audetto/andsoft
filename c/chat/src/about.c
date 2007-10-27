#include <gtk/gtk.h>
#include <stdio.h>

#include "config.h"

void about_dialog() {

  GtkWidget *dialog, *label, *okay_button;
  char buf[128];

/* Create the widgets */

  dialog = gtk_dialog_new();
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_title(GTK_WINDOW(dialog), "About ASI Chat Server");

  sprintf(buf, "\nASI Chat Server %s\n(C)opyRight AndSoft Inc., 2001-2003\n\n\nCompiled: %s (%s)\n", VERSION, __DATE__, __TIME__);
  label = gtk_label_new(buf);
  okay_button = gtk_button_new_with_label("OK");

/* Ensure that the dialog box is destroyed when the user clicks ok. */

  gtk_signal_connect_object(GTK_OBJECT(okay_button), "clicked",
    GTK_SIGNAL_FUNC(gtk_widget_destroy), dialog);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->action_area), okay_button);

/* Add the label, and show everything we've added to the dialog. */

  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);
  gtk_widget_show_all(dialog);
}






