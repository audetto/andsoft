#ifndef FUNCTION_H
#define FUNCTION_H

#include <gtk/gtk.h>
#include <sys/socket.h>

void send_data(gpointer key, gpointer value, gpointer user_data);
GdkColor rand_color();
int send_all(int s, char *buf, int len);
gint ask_for_ident_answer(struct sockaddr_in *address, Client *cl);
int create_server_socket();
gboolean cancella_valore(gpointer key, gpointer value, gpointer user_data);
void print_on_status(const char *msg);
void check_buffer(const char *in, Client *cl);

#endif
