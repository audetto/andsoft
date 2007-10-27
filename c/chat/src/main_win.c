#include <gtk/gtk.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "data.h"
#include "client_win.h"
#include "function.h"
#include "main_win.h"
#include "about.h"

static char benvenuto[] = "\nWelcome to ASI Chat Server.\n(C)opyRight AndSoft Inc., 2001-2003\n\n";
static int riga = -1;

static gint delete_event(GtkWidget *window, GdkEventAny *e, gpointer data);

static void quit_app(gpointer callback_data, guint callback_action, GtkWidget *widget) {
  delete_event(NULL, NULL, NULL);
}

static void disconnect_client(gpointer callback_data, guint callback_action, GtkWidget *widget) {
  Client *cl;
  char buf[128];

  if (riga != -1) {
    cl = (Client *)gtk_clist_get_row_data(GTK_CLIST(lista), riga);    
    sprintf(buf, "Disconnessione forzata: %s", cl->title);
    print_on_status(buf);
    g_hash_table_remove(hash, GINT_TO_POINTER(cl->fd));
    cancella_valore(NULL, cl, NULL);
  }
}

static GtkItemFactoryEntry menu_items[] = {
  {"/_System",         NULL,         NULL,    0, "<Branch>"},
  {"/_System/_Quit",   "<control>Q", quit_app, 0, NULL},
  {"/_Client",         NULL,         NULL,    0, "<Branch>"},
  {"/_Client/_Disconnect", "<control>D", disconnect_client, 0, NULL},
  {"/_Help",           NULL,         NULL,    0, "<LastBranch>"},
  {"/_Help/_About...", "<control>A", about_dialog,    0, NULL}
};

static int nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);

static void send_msg_all(GtkButton *button, gpointer user_data) {
  GtkWidget *mw, *entry;
  Send s;
  char *buf;

  mw = (GtkWidget *)user_data;
  entry = (GtkWidget *)gtk_object_get_data(GTK_OBJECT(mw), "entry");
  s.fd = serv_sock;
  buf = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
  s.buf = g_strconcat(buf, CRLF, NULL);
  s.buf_len = strlen(s.buf);
  s.tit = "chat server: ";
  s.tit_len = strlen(s.tit);
  g_hash_table_foreach(hash, send_data, &s);
  s.buf[s.buf_len - 2] = '\n';
  gtk_text_insert(GTK_TEXT(all_in_all), NULL, NULL, NULL, s.buf, s.buf_len - 1);
  g_free(s.buf);
  g_free(buf);
  gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void input_func(gpointer data, gint source, GdkInputCondition condition) {
  int newfd, addrlen, nbytes;
  gint riga;
  struct sockaddr_in remoteaddr;
  Client *cl;
  Send s;
  char buf[128];
  char adr[32], po[32], fd[32];
  char *val[6] = {adr, po, fd, "0"};
  time_t t;

  static char no_id[] = "no server";
  static char wa_id[] = "waiting...";

/* ********************************* NUOVO CLIENT ************************** */

  if (source == serv_sock) {

    addrlen = sizeof(remoteaddr);
    newfd = accept(serv_sock, &remoteaddr, &addrlen);
    sprintf(adr, "%s", inet_ntoa(remoteaddr.sin_addr));
    sprintf(po, "%d", ntohs(remoteaddr.sin_port));
    sprintf(fd, "%d", newfd);
    t = time(NULL);
    val[4] = ctime(&t);
    val[4][strlen(val[4]) - 1] = '\0';

    cl = malloc(sizeof(Client));
    cl->fd = newfd;
    cl->callb = gdk_input_add(newfd, GDK_INPUT_READ, input_func, cl);
    cl->callb_ident = ask_for_ident_answer(&remoteaddr, cl);
    if (cl->callb_ident) {
      val[5] = wa_id;
    } else {
      val[5] = no_id;
    }
    cl->win = create_client_win(cl);
    cl->traffic = 0;
    cl->buffer[0] = '\0';
    cl->fore = rand_color();
    cl->back = rand_color();

    g_hash_table_insert(hash, GINT_TO_POINTER(newfd), cl);

    riga = gtk_clist_append(GTK_CLIST(lista), val);
    gtk_clist_set_row_data(GTK_CLIST(lista), riga, cl);    

    sprintf(cl->title, "%s:%d", inet_ntoa(remoteaddr.sin_addr), ntohs(remoteaddr.sin_port)); 
    print_on_status(cl->title);

    gtk_window_set_title(GTK_WINDOW(cl->win), cl->title);
    gtk_widget_show_all(cl->win);

    s.fd = serv_sock;
    s.buf = benvenuto;
    s.buf_len = strlen(s.buf);
    s.tit = NULL;
    s.tit_len = 0;
    send_data(NULL, cl, &s);
/* ************************************************************************* */
  } else {
/* ********************************* NUOVI DATI **************************** */
    //    cl = (Client *)g_hash_table_lookup(hash, GINT_TO_POINTER(source));
    cl = (Client *)data;
    if ((nbytes = recv(source, buf, sizeof(buf) - 1, 0)) <= 0) {
      sprintf(buf, "%s: Errore ricezione (%d) [%s]", cl->title, errno, strerror(errno));
      print_on_status(buf);
      cancella_valore(NULL, cl, NULL);
      g_hash_table_remove(hash, GINT_TO_POINTER(source));
    } else {
      if (nbytes > 0) {
        buf[nbytes] = '\0';
        check_buffer(buf, cl);
      }
    } 
/* ************************************************************************* */
  }
}

static void seleziona_client(GtkCList *clist, gint row, gint column, 
  GdkEventButton *event, gpointer user_data) {
  riga = row;
}

static void deseleziona_client(GtkCList *clist, gint row, gint column, 
  GdkEventButton *event, gpointer user_data) {
  riga = -1;
}

static gint delete_event(GtkWidget *window, GdkEventAny *e, gpointer data) {
  close(serv_sock);
  g_hash_table_foreach_remove(hash, cancella_valore, NULL);
  g_hash_table_destroy(hash);
  gtk_main_quit();
  return FALSE;
}

GtkWidget *create_main_win() {
  GtkWidget *main_win, *notebook, *scroll, *label, *vb, *hb, *button, *entry, *box;
  GtkAccelGroup *accel_group;
  GtkItemFactory *item_factory;
  gchar *titoli[] = {"host", "port", "fd", "traffic", "logged", "identd"};

  main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(main_win), "ASI Chat Server");

  gtk_signal_connect(GTK_OBJECT(main_win), "delete_event", 
		     GTK_SIGNAL_FUNC(delete_event), NULL);

  box = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(main_win), box);

  accel_group = gtk_accel_group_new();
  item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<blah>", accel_group);
  gtk_item_factory_create_items(item_factory, nmenu_items, menu_items, NULL);
  gtk_accel_group_attach(accel_group, GTK_OBJECT(main_win));

  gtk_box_pack_start(GTK_BOX(box), gtk_item_factory_get_widget(item_factory, "<blah>"), FALSE, FALSE, 0);

  notebook = gtk_notebook_new();

  scroll = gtk_scrolled_window_new(NULL, NULL);
  label = gtk_label_new("Clients");
  lista = gtk_clist_new_with_titles(6, titoli);
  gtk_signal_connect(GTK_OBJECT(lista), "select-row", GTK_SIGNAL_FUNC(seleziona_client), NULL);
  gtk_signal_connect(GTK_OBJECT(lista), "unselect-row", GTK_SIGNAL_FUNC(deseleziona_client), NULL);
  
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), lista);
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll, label);

  scroll = gtk_scrolled_window_new(NULL, NULL);
  label = gtk_label_new("Status");
  status = gtk_text_new(NULL, NULL);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), status);
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll, label);

  vb = gtk_vbox_new(FALSE, 0);

  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_box_pack_start(GTK_BOX(vb), scroll, TRUE, TRUE, 0);

  label = gtk_label_new("All");
  all_in_all = gtk_text_new(NULL, NULL);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll), all_in_all);

  hb = gtk_hbox_new(FALSE, 0);
  entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hb), entry, TRUE, TRUE, 0);
  gtk_object_set_data(GTK_OBJECT(main_win), "entry", entry);

  button = gtk_button_new_with_label("Send all!");
  gtk_box_pack_start(GTK_BOX(hb), button, FALSE, TRUE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(send_msg_all), main_win);

  gtk_box_pack_start(GTK_BOX(vb), hb, FALSE, TRUE, 0);
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vb, label);

  gtk_box_pack_start(GTK_BOX(box), notebook, TRUE, TRUE, 0);

  hash = g_hash_table_new(NULL, NULL);

  return main_win;
}
