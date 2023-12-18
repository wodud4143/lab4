#include <gtk/gtk.h>

GtkWidget *entry1;
GtkWidget *entry2;
GtkWidget *result_label;


void calculate(GtkWidget *widget, gpointer data) {
    double num1 = atof(gtk_entry_get_text(GTK_ENTRY(entry1)));
    double num2 = atof(gtk_entry_get_text(GTK_ENTRY(entry2)));
    double result = 0.0;
    gchar *operator = (gchar *)data;

    if (strcmp(operator, "+") == 0) {
        result = num1 + num2;
    } else if (strcmp(operator, "-") == 0) {
        result = num1 - num2;
    } else if (strcmp(operator, "*") == 0) {
        result = num1 * num2;
    } else if (strcmp(operator, "/") == 0) {
        if (num2 != 0) {
            result = num1 / num2;
        } else {
            gtk_label_set_text(GTK_LABEL(result_label), "오류: 0으로 나눌 수 없습니다");
            return;
        }
    }

    gchar result_text[64];
    g_snprintf(result_text, sizeof(result_text), "결과: %.2f", result);
    gtk_label_set_text(GTK_LABEL(result_label), result_text);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *add_button;
    GtkWidget *subtract_button;
    GtkWidget *multiply_button;
    GtkWidget *divide_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "간단한 계산기");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();

    add_button = gtk_button_new_with_label("더하기 (+)");
    subtract_button = gtk_button_new_with_label("빼기 (-)");
    multiply_button = gtk_button_new_with_label("곱하기 (*)");
    divide_button = gtk_button_new_with_label("나누기 (/)");

    result_label = gtk_label_new("결과: ");
    gtk_label_set_xalign(GTK_LABEL(result_label), 0.0); 

    g_signal_connect(add_button, "clicked", G_CALLBACK(calculate), "+");
    g_signal_connect(subtract_button, "clicked", G_CALLBACK(calculate), "-");
    g_signal_connect(multiply_button, "clicked", G_CALLBACK(calculate), "*");
    g_signal_connect(divide_button, "clicked", G_CALLBACK(calculate), "/");

    gtk_grid_attach(GTK_GRID(grid), entry1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), subtract_button, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), multiply_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), divide_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, 3, 2, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

