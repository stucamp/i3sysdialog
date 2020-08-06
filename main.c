#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void do_restart(GtkWidget *widget, gpointer data)
{
    system("systemctl reboot");
}

void do_shutdown(GtkWidget *widget, gpointer data)
{
    system("systemctl poweroff");
}

void do_lockscreen(GtkWidget *widget, gpointer data)
{
    system("i3lock-fancy -g");
}

void do_lock_and_sleep(GtkWidget *widget, gpointer data)
{
    system("i3lock-fancy -g && systemctl suspend");
}

static gboolean check_key(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch (event->keyval)
    {
	    case GDK_KEY_1:
		    do_lockscreen(widget, data);
		    gtk_main_quit();
		    return TRUE;
	    case GDK_KEY_2:
		    do_lock_and_sleep(widget, data);
		    gtk_main_quit();
		    return TRUE;
	    case GDK_KEY_3:
		    do_restart(widget, data);
		    gtk_main_quit();
		    return TRUE;
	    case GDK_KEY_4:
		    do_shutdown(widget, data);
		    gtk_main_quit();
		    return TRUE;
    }
    gtk_main_quit();
    return FALSE;
}

static gboolean check_focus(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
    if (event->type == GDK_FOCUS_CHANGE) {
        gtk_main_quit();
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char **argv) {
    GtkWidget *window, *grid, *lockit, *lock_sleep, *reboot, *shutdown;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//  Set title in case WM ignores no decorators
    gtk_window_set_title((GtkWindow *)window, "What do?");

//  Sets window to open center screen and have not header bar/decorators
    gtk_window_set_position((GtkWindow *)window, GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_decorated((GtkWindow *)window, FALSE);
//  Set to close app if button push isn't 1, 2, 3, or 4
    g_signal_connect(window, "key_press_event", G_CALLBACK(check_key), NULL);
//  Set to close app if focus is lost
    g_signal_connect(window, "focus_out_event", G_CALLBACK(check_focus), NULL);
//  Added listener in case WM ignores no decorators
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


//  Creates simple window with 4 buttons, in a grid.
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    lockit = gtk_button_new_with_label("lock screen (1)");
    g_signal_connect(lockit, "clicked", G_CALLBACK(do_lockscreen), NULL);
    gtk_grid_attach(GTK_GRID(grid), lockit, 0, 0, 1, 1);

    lock_sleep = gtk_button_new_with_label("lock & sleep (2)");
    g_signal_connect(lock_sleep, "clicked", G_CALLBACK(do_lock_and_sleep), NULL);
    gtk_grid_attach(GTK_GRID(grid), lock_sleep, 1, 0, 1, 1);

    reboot = gtk_button_new_with_label("reboot (3)");
    g_signal_connect(reboot, "clicked", G_CALLBACK(do_restart), NULL);
    gtk_grid_attach(GTK_GRID(grid), reboot, 2, 0, 1, 1);

    shutdown = gtk_button_new_with_label("shutdown (4)");
    g_signal_connect(shutdown, "clicked", G_CALLBACK(do_shutdown), NULL);
    gtk_grid_attach(GTK_GRID(grid), shutdown, 3, 0, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}