#ifndef DATA_H
#define DATA_H

#include <gtk/gtk.h>

typedef struct _Client Client;
struct _Client {
  int fd;
  gint callb, callb_ident;
  GtkWidget *win;
  int traffic;
  char title[128];
  char buffer[256];
  GdkColor back, fore;
};

typedef struct _Send Send;
struct _Send {
  int fd;
  char *buf;
  int buf_len;
  char *tit;
  int tit_len;
};

extern GtkWidget *status;
extern int serv_sock;
extern GtkWidget *lista;
extern int chat_port;
extern GtkWidget *all_in_all;
extern GHashTable *hash;
extern char CRLF[];

#endif
