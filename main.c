#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static void dest_and_quit(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	gtk_main_quit();
}

static void do_restart(void)
{
    system("systemctl reboot");
}

static void do_shutdown(void)
{
    system("systemctl poweroff");
}

static void do_logout(void)
{
	system("i3-msg exit");
}
	
static void do_lockscreen(void)
{
    system("i3lock-fancy -g");
}

static void do_lock_and_sleep(void)
{
    system("i3lock-fancy -g && systemctl suspend");
}

static void check_key(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    switch (event->keyval)
    {
	    case GDK_KEY_1:
		    do_lockscreen();
    		    dest_and_quit(widget);
		    break;
	    case GDK_KEY_2:
		    do_lock_and_sleep();
    		    dest_and_quit(widget);
		    break;
	    case GDK_KEY_3:
		    do_logout();
    		    dest_and_quit(widget);
		    break;
	    case GDK_KEY_4:
		    do_restart();
    		    dest_and_quit(widget);
		    break;
	    case GDK_KEY_5:
		    do_shutdown();
    		    dest_and_quit(widget);
		    break;
	    default:
		    dest_and_quit(widget);
    }
}


static void check_focus(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
    if (event->type == GDK_FOCUS_CHANGE) {
	dest_and_quit(widget);
    }
}

int main(int argc, char **argv) 
{
    GtkWidget *window, *box, *lockit, *lock_sleep, *logout, *reboot, *shutdown;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    //  Set title in case WM ignores no decorators
    gtk_window_set_title((GtkWindow *)window, "What do?");
    gtk_window_set_gravity((GtkWindow *)window, GDK_GRAVITY_CENTER);

    //  Sets window to open center screen and have not header bar/decorators
    gtk_window_set_position((GtkWindow *)window, GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_decorated((GtkWindow *)window, FALSE);

    //  Set to close app if button push isn't 1, 2, 3, or 4
    g_signal_connect(window, "key_press_event", 
		     G_CALLBACK (check_key), NULL);
    
    //  Set to close app if focus is lost
    g_signal_connect(window, "focus_out_event", 
		     G_CALLBACK (check_focus), NULL);

    //  Added listener in case WM ignores no decorators/headerbar
    g_signal_connect(window, "destroy", 
		     G_CALLBACK(gtk_main_quit), NULL);

    //  Creates simple window with 4 buttons, in a grid.
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    lockit = gtk_button_new_with_label("lock screen (1)");
    g_signal_connect(lockit, "clicked",
		     G_CALLBACK(do_lockscreen), NULL);
    gtk_box_pack_start((GtkBox *)box, lockit, TRUE, TRUE, 0);

    lock_sleep = gtk_button_new_with_label("lock & sleep (2)");
    g_signal_connect(lock_sleep, "clicked", 
		     G_CALLBACK(do_lock_and_sleep), NULL);
    gtk_box_pack_start((GtkBox *)box, lock_sleep, TRUE, TRUE, 0);

    logout = gtk_button_new_with_label("logout (3)");
    g_signal_connect(logout, "clicked", 
		     G_CALLBACK(do_logout), NULL);
    gtk_box_pack_start((GtkBox *)box, logout, TRUE, TRUE, 0);

    reboot = gtk_button_new_with_label("reboot (4)");
    g_signal_connect(reboot, "clicked", 
		     G_CALLBACK(do_restart), NULL);
    gtk_box_pack_start((GtkBox *)box, reboot, TRUE, TRUE, 0);

    shutdown = gtk_button_new_with_label("shutdown (5)");
    g_signal_connect(shutdown, "clicked", 
		     G_CALLBACK(do_shutdown), NULL);
    gtk_box_pack_start((GtkBox *)box, shutdown, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
