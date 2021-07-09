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

#define APPLICATION_ID              "azorfus.znotes"
#define DEFAULT_WINDOW_WIDTH        (600)
#define DEFAULT_WINDOW_HEIGHT       (500)
#define DEFAULT_WINDOW_TITLE        "ZNotes"
#define DEFAULT_WINDOW_RESIZABLE    (FALSE)
#define CONTAINER_BORDER_WIDTH      (5)

enum
{
	LIST_ITEM = 0
};

typedef struct TOOLBAR
{
    GtkWidget* toolbar;
    GtkWidget* box;
    GtkToolItem* add_note;
} Toolbar;

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
    *box,
	* sub_pane;
    
    Toolbar toolbar;
    
} Window;

/** sets the widget auto-resizable
 */
void resize (GtkWidget* widget)
{
	gtk_widget_set_hexpand(widget, TRUE);
	gtk_widget_set_vexpand(widget, TRUE);
}

/** window initialization
 */
void winInit (GtkApplication* app, Window* window)
{
	window->window = gtk_application_window_new(app);
    
	/* Configure */
	gtk_window_set_title (GTK_WINDOW(window->window), DEFAULT_WINDOW_TITLE);
	gtk_window_set_default_size (GTK_WINDOW(window->window), DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	//gtk_window_set_resizable (GTK_WINDOW(window->window), FALSE);
}

/** inits all the window's widgets
 */
void widgetInit (Window* window)
{
	window->main_pane = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    window->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	window->sub_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	window->s_list = gtk_scrolled_window_new(NULL, NULL);
	window->s_qtext = gtk_scrolled_window_new(NULL, NULL);
	window->s_text = gtk_scrolled_window_new(NULL, NULL);
	window->note_list = gtk_tree_view_new();
	window->text_area = gtk_text_view_new();
	window->quick_text = gtk_text_view_new();
}


/** initializing the main note list
 */
void listInit (Window* window)
{
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes("Notes", renderer, "text", LIST_ITEM, NULL);
	GtkListStore* store = gtk_list_store_new(1, G_TYPE_STRING); /* nColumns:columnType */
    
	/* Configure list store */
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(window->note_list), FALSE);
    
	/* Add column */
	gtk_tree_view_append_column(GTK_TREE_VIEW(window->note_list), column);
    
	/* Set model */
	gtk_tree_view_set_model(GTK_TREE_VIEW(window->note_list), GTK_TREE_MODEL(store));
}

/** adds label to the main list
 */
void listAdd (GtkWidget* list, const gchar* label)
{
	GtkListStore* store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	GtkTreeIter iter = {0};
    
    /* Add item to list */
	gtk_list_store_insert_with_values(store, &iter, -1, LIST_ITEM, label, -1);
}

/* New note function */
void new_note(GtkWidget* widget, gpointer data)
{
    //GtkWidget* note_dialog, *label, *name;
    //note_dialog = gtk_
    listAdd(data, "000000");
}

/** initializes the tool bar
*/
void toolbarInit(Window* window)
{
    window->toolbar.toolbar = gtk_toolbar_new();
    gtk_box_pack_start(GTK_BOX(window->box), window->toolbar.toolbar, FALSE, FALSE, 0);
    
    /* creating icon for the tool item "new note" */
    GtkWidget* icon_add = gtk_image_new_from_icon_name("document-new", GTK_ICON_SIZE_BUTTON);
    window->toolbar.add_note = gtk_tool_button_new(icon_add, "New");
    
    g_signal_connect(GTK_WIDGET(window->toolbar.add_note), "clicked", G_CALLBACK(new_note), window->note_list);
    gtk_toolbar_insert(GTK_TOOLBAR(window->toolbar.toolbar), window->toolbar.add_note, 0);
}

/** adds border around the widgets
 */
void addBorder (Window* window)
{
	gtk_container_set_border_width(GTK_CONTAINER(window->note_list), CONTAINER_BORDER_WIDTH);
	gtk_container_set_border_width(GTK_CONTAINER(window->quick_text), CONTAINER_BORDER_WIDTH);
	gtk_container_set_border_width(GTK_CONTAINER(window->text_area), CONTAINER_BORDER_WIDTH);
}

/** assigns every widget to the window
 */
void fixToWindow (Window* window)
{
	/* adding the widget to scrollable windows */
	gtk_container_add(GTK_CONTAINER(window->s_list),    window->note_list);
	gtk_container_add(GTK_CONTAINER(window->s_qtext),   window->quick_text);
	gtk_container_add(GTK_CONTAINER(window->s_text),    window->text_area);
    
	/* adding the widgets to the panes */
	gtk_paned_add1(GTK_PANED(window->sub_pane),     window->s_list);
	gtk_paned_add2(GTK_PANED(window->sub_pane),     window->s_qtext);
	gtk_paned_add1(GTK_PANED(window->main_pane),    window->sub_pane);
	gtk_paned_add2(GTK_PANED(window->main_pane),    window->s_text);
    
	/* adding the final main pane to the window */
	addBorder(window);
    
    gtk_box_pack_start(GTK_BOX(window->box), window->main_pane, TRUE, TRUE, 0);
    
	/* Adding main pane to window */
	gtk_container_add(GTK_CONTAINER(window->window), window->box);
}

/** When GTK+ is ought to run
 * Dynamic GTK GUI functions go there
 */
void activate_ (GtkApplication* app, gpointer user_data)
{
	Window window = {NULL};
    
	/* Create window */
	winInit(app, &window);
    
	/* Create widgets */
	widgetInit(&window);
    
    /* Create tool bar */
    toolbarInit(&window);
    
	/* Pack everything */
	fixToWindow(&window);
    
	/* Create list view */
	listInit(&window);
	listAdd(window.note_list, "one");
    
    /* Show all */
	gtk_widget_show_all(window.window);
}

int gtk(int argc, char *argv[])
{
	GtkApplication* app = gtk_application_new(APPLICATION_ID, G_APPLICATION_FLAGS_NONE);
    
	/* Not used */
	(void)argc;
	(void)argv;
    
	/* Connect GtkApplication signals */
	g_signal_connect(app, "activate", G_CALLBACK(activate_), NULL);
    
	/* Run app */
	return g_application_run(G_APPLICATION(app), argc, argv);
}

int main (int argc, char* argv[])
{
	return gtk(argc, argv);
}
