#include <gtk/gtk.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "data.h"
#include "main_win.h"
#include "launch.h"
#include "function.h"

static GtkWidget *port_entry;

static void check_port(GtkButton *button, gpointer user_data) {
  gchar *valore;
  
  valore = gtk_editable_get_chars(GTK_EDITABLE(port_entry), 0, -1);
  chat_port = atoi(valore);
  g_free(valore);

  if (chat_port) {
    serv_sock = create_server_socket();
    if (serv_sock != -1) {
      gtk_widget_destroy(GTK_WIDGET(user_data));
      gdk_input_add(serv_sock, GDK_INPUT_READ, input_func, NULL);  
    }
  }

}

static gint delete_event(GtkWidget *window, GdkEventAny *e, gpointer data) {
  return TRUE;
}

GtkWidget *get_valid_port() {

  GtkWidget *first, *table, *label, *ok;
  
  first = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(first), "Port selection");
  gtk_signal_connect(GTK_OBJECT(first), "delete_event", 
		     GTK_SIGNAL_FUNC(delete_event), NULL);

  table = gtk_hbox_new(FALSE, 0);
  
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(first)->vbox), table);
  
  label = gtk_label_new("Porta:");
  gtk_box_pack_start(GTK_BOX(table), label, FALSE, TRUE, 0);
  
  port_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(port_entry), "9034"); 
  gtk_box_pack_start(GTK_BOX(table), port_entry, TRUE, TRUE, 0);

  ok = gtk_button_new_with_label("OK");
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(first)->action_area), ok);
  gtk_signal_connect(GTK_OBJECT(ok), "clicked", GTK_SIGNAL_FUNC(check_port), first);
  
  return first;
}
