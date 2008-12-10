/*
 * ToscoTerm - A very simple and stupid terminal emulator
 * Copyright (C) 2008 O.S. Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <vte/vte.h>

static void set_window_geometry_hints(GtkWidget *window, VteTerminal *term)
{
	GdkGeometry hints;
	gint xpad;
	gint ypad;

	vte_terminal_get_padding(VTE_TERMINAL(term), &xpad, &ypad);

	hints.base_width = xpad;
	hints.base_height = ypad;
	hints.width_inc = VTE_TERMINAL(term)->char_width;
	hints.height_inc = VTE_TERMINAL(term)->char_height;
	hints.min_width = hints.base_width + hints.width_inc * 4;
	hints.min_height = hints.base_height + hints.height_inc * 2;

	gtk_window_set_geometry_hints(GTK_WINDOW(window), GTK_WIDGET(term), &hints,
		GDK_HINT_RESIZE_INC | GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE);
}

static void childexited_cb(VteTerminal *term)
{
	gtk_exit(0);
}

static int keypressed_cb(GtkWidget *widget, GdkEventKey *event)
{
	if (event->keyval == GDK_F11 && event->state & GDK_SHIFT_MASK) {
		GdkWindowState state = gdk_window_get_state(GTK_WIDGET(widget)->window);
		if (state & GDK_WINDOW_STATE_FULLSCREEN)
			gtk_window_unfullscreen(GTK_WINDOW(widget));
		else
			gtk_window_fullscreen(GTK_WINDOW(widget));

		return 1;
	}

	return 0;
}

static gboolean delete_event(GtkWidget *widget)
{
	gtk_main_quit();
	return FALSE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	guint32 xid = 0;
	int fullscreen = 0;
	char *exec = NULL;

	int i;
	for (i = 0; i < argc; i++) {
		if (!strncmp(argv[i], "-x", 2))
			xid = strtol(argv[++i], NULL, 0);
		else if (!strncmp(argv[i], "-f", 2))
			fullscreen = 1;
		else if (!strncmp(argv[i], "-e", 2))
			exec = argv[++i];
	}

	GtkWidget *main_window;
	if (xid)
		main_window = gtk_plug_new(xid);
	else
		main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GtkWidget *term = vte_terminal_new();
	gtk_container_add(GTK_CONTAINER(main_window), term);

	set_window_geometry_hints(main_window, VTE_TERMINAL(term));

	vte_terminal_set_emulation(VTE_TERMINAL(term), "xterm");
	vte_terminal_set_size(VTE_TERMINAL(term), 80, 24);

	if (!exec)
		vte_terminal_fork_command(VTE_TERMINAL(term), g_getenv("SHELL"), NULL, NULL, ".", FALSE, FALSE, FALSE);
	else {
		gchar **command;
		g_shell_parse_argv(exec, NULL, &command, NULL);
		vte_terminal_fork_command(VTE_TERMINAL(term), (const char *)*(command), command, NULL, ".", FALSE, FALSE, FALSE);
	}

	g_signal_connect(term, "child-exited", G_CALLBACK(childexited_cb), NULL);
	g_signal_connect(main_window, "key-press-event", G_CALLBACK(keypressed_cb), NULL);
	g_signal_connect(main_window, "delete_event", G_CALLBACK(delete_event), NULL);

	if (fullscreen == 1)
		gtk_window_fullscreen(GTK_WINDOW(main_window));

	gtk_widget_show_all(GTK_WIDGET(main_window));

	gtk_main();
	return 0;
}

