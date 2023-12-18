#include <gtk/gtk.h>

// 버튼 클릭 이벤트 핸들러
void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Hello, GTK!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *grid;

    gtk_init(&argc, &argv);

   
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "간단한 GTK 앱");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

 
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    
    button = gtk_button_new_with_label("클릭하세요");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

  
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_widget_show_all(window);

    
    gtk_main();

    return 0;
}

