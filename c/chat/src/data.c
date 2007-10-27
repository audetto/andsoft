#include <gtk/gtk.h>

#include "data.h"

GtkWidget *status;
int serv_sock = -1;
GtkWidget *lista;
int chat_port;
GtkWidget *all_in_all;
GHashTable *hash = NULL;
char CRLF[] = "\r\n";




