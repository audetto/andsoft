#include <gtk/gtk.h>
#include <string.h>

#include "data.h"
#include "function.h"
#include "client_win.h"

static gint delete_event(GtkWidget *window, GdkEventAny *e, gpointer data) {
  Client *cl;
  cl = (Client *)gtk_object_get_data(GTK_OBJECT(window), "client");
  cl->win = NULL;
  return FALSE;
}

static void send_msg(GtkButton *button, gpointer user_data) {
  GtkWidget *cw, *entry;
  Client *cl;
  Send s;
  gchar *buf;

  cw = (GtkWidget *)user_data;
  cl = (Client *)gtk_object_get_data(GTK_OBJECT(cw), "client");
  entry = (GtkWidget *)gtk_object_get_data(GTK_OBJECT(cw), "entry");
  s.fd = serv_sock;
  buf = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  s.buf = g_strconcat("*** Private from Chat server: ", buf, CRLF, NULL);
  s.buf_len = strlen(s.buf);
  s.tit = NULL;
  s.tit_len = 0;
  send_data(NULL, cl, &s);
  g_free(s.buf);
  g_free(buf);
  gtk_entry_set_text(GTK_ENTRY(entry), "");
}

GtkWidget *create_client_win(Client *cl) {
  GtkWidget *cw, *hb, *vb, *scroll, *testo, *button, *entry;

  cw = gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_signal_connect(GTK_OBJECT(cw), "delete_event", 
		     GTK_SIGNAL_FUNC(delete_event), NULL);

  gtk_object_set_data(GTK_OBJECT(cw), "client", cl);
    
  vb = gtk_vbox_new(FALSE, 0);

  gtk_container_add(GTK_CONTAINER(cw), vb);

  scroll = gtk_scrolled_window_new(NULL, NULL);
  testo = gtk_text_new(NULL, NULL);
  gtk_object_set_data(GTK_OBJECT(cw), "testo", testo);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), testo);

  gtk_box_pack_start(GTK_BOX(vb), scroll, TRUE, TRUE, 0);

  hb = gtk_hbox_new(FALSE, 0);
  entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hb), entry, TRUE, TRUE, 0);
  gtk_object_set_data(GTK_OBJECT(cw), "entry", entry);

  button = gtk_button_new_with_label("Send!");
  gtk_box_pack_start(GTK_BOX(hb), button, FALSE, TRUE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(send_msg), cw);

  gtk_box_pack_start(GTK_BOX(vb), hb, FALSE, TRUE, 0);

  return cw;
}











