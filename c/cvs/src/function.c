#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "data.h"
#include "function.h"

GdkColor rand_color() {
  GdkColor result;

  result.pixel = 0;
  result.red = rand();
  result.green = rand();
  result.blue = rand();

  return result;
}

void send_data(gpointer key, gpointer value, gpointer user_data) {
  Send *s;
  Client *cl;
  GtkWidget *t;
  char buf[128];

  s = (Send *)user_data;
  cl = (Client *)value;

  if (cl->fd != s->fd) {
    send_all(cl->fd, s->tit, s->tit_len);
    if (send_all(cl->fd, s->buf, s->buf_len) == -1) {
      sprintf(buf, "%s: Errore invio (%d) [%s]", cl->title, errno, strerror(errno));
      print_on_status(buf);
    }
  } else {
    if (cl->win) {
      t = gtk_object_get_data(GTK_OBJECT(cl->win), "testo");
      
      // Cosi' evitiamo il carattere '\r' in Linux
      s->buf[s->buf_len - 2] = '\n';
      gtk_text_insert(GTK_TEXT(t), NULL, NULL, NULL, s->buf, s->buf_len - 1);
      s->buf[s->buf_len - 1] = '\r';
    }
    cl->traffic += s->buf_len;
    sprintf(buf, "%d", cl->traffic);
    gtk_clist_set_text(GTK_CLIST(lista),
      gtk_clist_find_row_from_data(GTK_CLIST(lista), cl), 3, buf);
  }
}

/* formerly -- int *len */

int send_all(int s, char *buf, int len) {
  int total = 0;
  int n = 0;

  while (total < len) {
    n = send(s, buf + total, len - total, 0);
    if (n == -1) {
      break;
    }
    total += n;
  }

  return n == -1 ? -1 : 0;
}

static void ident_input_func(gpointer data, gint source, GdkInputCondition condition) {
  int nbytes;
  Client *cl;
  char buf[128];
  gint riga;

  cl = (Client *)data;
  riga = gtk_clist_find_row_from_data(GTK_CLIST(lista), cl);
  if (riga != -1) {
    if ((nbytes = recv(source, buf, sizeof(buf), 0)) <= 0) {
      sprintf(buf, "%s: Errore ricezione ident (%d) [%s]", cl->title, errno, strerror(errno));
      print_on_status(buf);
      gtk_clist_set_text(GTK_CLIST(lista), riga, 5, "error...");
    } else {
      buf[nbytes] = '\0';
      gtk_clist_set_text(GTK_CLIST(lista), riga, 5, buf);
    }
  }
  close(source);
  gdk_input_remove(cl->callb_ident);
  cl->callb_ident = 0;
} 

gint ask_for_ident_answer(struct sockaddr_in *address, Client *cl) {
  gint result = 0;
  /* NO IDENT
  int sockfd;
  struct sockaddr_in dest_addr;
  char buf[128];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(113);
  dest_addr.sin_addr.s_addr = address->sin_addr.s_addr;
  memset(&(dest_addr.sin_zero), '\0', 8);

  if (connect(sockfd, 
      (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == 0) {
    sprintf(buf, "%d, %d\n", ntohs(address->sin_port), chat_port);
    if (send(sockfd, buf, strlen(buf), 0) == strlen(buf)) {
      result = gdk_input_add(sockfd, GDK_INPUT_READ, ident_input_func, cl);
    }
  }
  if (!result && sockfd) {
    close(sockfd);
  }
  */
  return result;
}

int create_server_socket() {
  int ss, t;
  struct sockaddr_in myaddr;
  char buf[128];

  ss = socket(AF_INET, SOCK_STREAM, 0);
  if (ss == -1) {
    sprintf(buf, "Errore socket (%d) [%s]", errno, strerror(errno));
    print_on_status(buf);
  } else {
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(chat_port);
    memset(&(myaddr.sin_zero), '\0', 8);
    t = bind(ss, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (t == -1) {
      sprintf(buf, "Errore bind (%d) [%s]", errno, strerror(errno));
      print_on_status(buf);
      ss = -1;
    } else {
      t = listen(ss, 10);
      if (t == -1) {
        sprintf(buf, "Errore listen (%d) [%s]", errno, strerror(errno));
        print_on_status(buf);
        ss = -1;
      } else {
        sprintf(buf, "Server listening on port %d", chat_port);
        print_on_status(buf);
      }	
    }
  }
  return ss;
}

gboolean cancella_valore(gpointer key, gpointer value, gpointer user_data) {
  Client *cl;
  cl = (Client *)value;

  close(cl->fd);
  gdk_input_remove(cl->callb);
  if (cl->win) {
    gtk_widget_destroy(cl->win);
  }
  gtk_clist_remove(GTK_CLIST(lista),
    gtk_clist_find_row_from_data(GTK_CLIST(lista), cl));

  free(value);
  return TRUE;
}

void print_on_status(const char *msg) {
  char *buf, *tmp;
  time_t t;

  t = time(NULL);
  tmp = ctime(&t);
  tmp[strlen(tmp) - 1] = '\0';
  buf = g_strconcat(tmp, ": ", msg, CRLF + 1, NULL);
  gtk_text_insert(GTK_TEXT(status), NULL, NULL, NULL, buf, -1);
  g_free(buf);
}

void check_buffer(const char *data, Client *cl) {
  int ch, i, n;
  Send s;

  // solo fino al primo \r o \n e comunque non oltre il 255 carattere!
  n = sizeof(cl->buffer) - strlen(cl->buffer) - 1;
  strncat(cl->buffer, data, n);
  cl->buffer[sizeof(cl->buffer) - 2] = '\n';
  cl->buffer[sizeof(cl->buffer) - 1] = '\0';

  ch = strcspn(cl->buffer, "\r\n");
  if (ch != strlen(cl->buffer)) {

    for (i = 0; i < ch; i++)
      if ((cl->buffer[i] < ' ') || (cl->buffer[i] > '~')) // only from ' ' = 32 to '~' = 126
        cl->buffer[i] = '.';

    cl->buffer[ch++] = CRLF[0];
    cl->buffer[ch++] = CRLF[1];
    s.fd = cl->fd;
    s.buf = cl->buffer;
    s.buf_len = ch;
    s.tit = g_strconcat(cl->title, ": ", NULL);
    s.tit_len = strlen(s.tit);

    g_hash_table_foreach(hash, send_data, &s);
    g_free(s.tit);

    gtk_text_insert(GTK_TEXT(all_in_all), NULL, &cl->fore, &cl->back, cl->title, -1);
    s.buf[s.buf_len - 2] = '\n';
    gtk_text_insert(GTK_TEXT(all_in_all), NULL, NULL, NULL, s.buf, s.buf_len - 1);

    cl->buffer[0] = '\0';
  }
}
