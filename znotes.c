/* znotes.c
 *
 * Copyright 2021 azorfus
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

#include <gtk/gtk.h>
#include <stdbool.h>

enum
{
	LIST_ITEM = 0
};

typedef struct WINDOW_DESCRIPTION
{
	GtkWidget
	
	* window,
	* s_list,
	* s_qtext,
	* s_text,
	* note_list,
	* quick_text,
	* text_area,
	* main_pane,
	* sub_pane;

} Window;

/* sets the widget auto-resizable */
void resize(GtkWidget* widget)
{
	gtk_widget_set_hexpand(widget, TRUE);
	gtk_widget_set_vexpand(widget, TRUE);
}


/* window initialization */
void winInit(GtkApplication* app, Window* window)
{
	window->window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window->window), "ZNotes");
	gtk_window_set_default_size(GTK_WINDOW(window->window), 600, 500);
	gtk_window_set_resizable(GTK_WINDOW(window->window), FALSE);
}

/* inits all the window's widgets */
void widgetInit(Window* window)
{
	window->main_pane = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	window->sub_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	window->s_list = gtk_scrolled_window_new(NULL, NULL);
	window->s_qtext = gtk_scrolled_window_new(NULL, NULL);
	window->s_text = gtk_scrolled_window_new(NULL, NULL);
	window->note_list = gtk_tree_view_new();
	window->text_area = gtk_text_view_new();
	window->quick_text = gtk_text_view_new();
}

/* initializing the main note list */
void listInit(Window* window)
{
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(window->note_list), FALSE);
	GtkCellRenderer* renderer;
	GtkTreeViewColumn* column;
	GtkListStore* store;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Notes", renderer, "text", LIST_ITEM, NULL);
	store = gtk_list_store_new(LIST_ITEM, G_TYPE_STRING);

	gtk_tree_view_append_column(GTK_TREE_VIEW(window->note_list), column);
	gtk_tree_view_set_model(GTK_TREE_VIEW(window->note_list), GTK_TREE_MODEL(store));
	g_object_unref(store);
}

/* adds label to the main list */
void listAdd(GtkWidget* list, const gchar* label)
{
	GtkListStore* store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, LIST_ITEM, label, -1);
}

/* adds border around the widgets */
void addBorder(Window* window)
{
	gtk_container_set_border_width(GTK_CONTAINER(window->note_list), 5);
	gtk_container_set_border_width(GTK_CONTAINER(window->quick_text), 5);
	gtk_container_set_border_width(GTK_CONTAINER(window->text_area), 5);

}

/* assigns every widget to the window */
void fixToWindow(Window* window)
{
	/* adding the widget to scrollable windows */
	gtk_container_add(GTK_CONTAINER(window->s_list), window->note_list);
	gtk_container_add(GTK_CONTAINER(window->s_qtext), window->quick_text);
	gtk_container_add(GTK_CONTAINER(window->s_text), window->text_area);

	/* adding the widgets to the panes */
	gtk_paned_add1(GTK_PANED(window->sub_pane), window->s_list);
	gtk_paned_add2(GTK_PANED(window->sub_pane), window->s_qtext);
	gtk_paned_add1(GTK_PANED(window->main_pane), window->sub_pane);
	gtk_paned_add2(GTK_PANED(window->main_pane), window->s_text);

	/* adding the final main pane to the window */
	addBorder(window);
	gtk_container_add(GTK_CONTAINER(window->window), window->main_pane);
}

void activate(GtkApplication* app, gpointer user_data)
{
	Window window;
	winInit(app, &window);
	widgetInit(&window);
	fixToWindow(&window);	
	listInit(&window);
//	listAdd(window.note_list, "one");
	gtk_widget_show_all(window.window);
}

int main(int argc, char* argv[])
{
	int status;
	GtkApplication* app;
	app = gtk_application_new("azorfus.znotes", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
