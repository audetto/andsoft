#include <gtk/gtk.h>

#include "launch.h"
#include "main_win.h"

int main(int argc, char* argv[]) {
  GtkWidget *launcher, *main_win;
  
  gtk_init(&argc, &argv);
  main_win = create_main_win();
  launcher = get_valid_port();
  
  gtk_widget_show_all(launcher);
  gtk_widget_show_all(main_win);
  
  gtk_main();
  
  return 0;
}
