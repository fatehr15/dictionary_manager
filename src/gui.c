/* improved-main-gtk.c */
#include <gtk/gtk.h>
#include "queue.h"
#include "stacks.h"
#include "binarySearchTrees.h"
#include "recursion.h"



typedef struct {
    // Data structures
    TList *syn, *ant;
    TStack *stk;
    TQueue *que;
    TTree *tree;

    // Widgets
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *txt_output;
    GtkWidget *file_chooser;
    GtkWidget *progress;
    GtkWidget *spinner;
    GtkWidget *statusbar;
    guint context_id;
    GtkWidget *filter_entry;
} AppState;

/* ==== Utilities ==== */

static void append_text(AppState *s, const gchar *msg) {
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(s->txt_output));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buf, &end);
    gtk_text_buffer_insert(buf, &end, msg, -1);
}

static void set_status(AppState *s, const gchar *msg) {
    gtk_statusbar_pop(GTK_STATUSBAR(s->statusbar), s->context_id);
    gtk_statusbar_push(GTK_STATUSBAR(s->statusbar), s->context_id, msg);
}

/* ==== File Loading with Feedback ==== */
static gboolean load_data_async(gpointer user_data) {
    AppState *s = user_data;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(s->file_chooser);
    gchar *path = gtk_file_chooser_get_filename(chooser);
    append_text(s, "🕑 Loading data...\n");
    set_status(s, "Loading...");
    gtk_spinner_start(GTK_SPINNER(s->spinner));
    
    FILE *f = fopen(path, "r");
    if (!f) {
        append_text(s, "❌ Failed to open file!\n");
        set_status(s, "Load error");
    } else {
        // Example: always load synonyms for demo
        s->syn = getSynWords(f);
        fclose(f);
        append_text(s, "✅ Loaded synonyms.\n");
        set_status(s, "Load complete");
    }
    g_free(path);
    gtk_spinner_stop(GTK_SPINNER(s->spinner));
    return FALSE;
}

static void on_open_file(GtkMenuItem *menuitem, gpointer user_data) {
    AppState *s = user_data;
    gtk_widget_show(s->file_chooser);
}

static void on_file_selected(GtkButton *btn, gpointer user_data) {
    AppState *s = user_data;
    gtk_widget_hide(s->file_chooser);
    g_timeout_add(100, load_data_async, s);
}

/* ==== TreeView with Filter ==== */
static GtkTreeModel* create_filter_model(GtkListStore *store, AppState *s) {
    GtkTreeModel *filter = gtk_tree_model_filter_new(GTK_TREE_MODEL(store), NULL);
    gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(filter),
        +[](GtkTreeModel *m, GtkTreeIter *i, gpointer data){
            AppState *s = data;
            gchar *word;
            gtk_tree_model_get(m, i, 0, &word, -1);
            const gchar *filter_text = gtk_entry_get_text(GTK_ENTRY(s->filter_entry));
            gboolean visible = (filter_text[0] == '\0') || (g_strrstr(word, filter_text) != NULL);
            g_free(word);
            return visible;
        }, s, NULL);
    return filter;
}

static GtkWidget* list_view_with_filter(AppState *s) {
    GtkListStore *store = gtk_list_store_new(4,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
    for (TNode *n = s->syn->head; n; n = n->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, n->word, 1, n->syn_or_ant,
            2, n->num_chars, 3, n->num_vowels, -1);
    }
    GtkTreeModel *filter_model = create_filter_model(store, s);
    GtkWidget *view = gtk_tree_view_new_with_model(filter_model);
    g_object_unref(filter_model);
    g_object_unref(store);
    const gchar *labels[] = {"Word","Type","Chars","Vowels"};
    for (int i = 0; i < 4; ++i) {
        GtkCellRenderer *rend = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
            labels[i], rend, "text", i, NULL);
        gtk_tree_view_column_set_sort_column_id(col, i);
        gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    }
    return view;
}

/* ==== Context Menu for Console ==== */
static void on_console_menu(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    if (event->button == GDK_BUTTON_SECONDARY) {
        GtkWidget *menu = gtk_menu_new();
        GtkWidget *copy = gtk_menu_item_new_with_label("Copy All");
        GtkWidget *clear = gtk_menu_item_new_with_label("Clear Console");
        g_signal_connect(copy, "activate", G_CALLBACK(on_clear), user_data);
        g_signal_connect(clear, "activate", G_CALLBACK(on_clear), user_data);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), copy);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), clear);
        gtk_widget_show_all(menu);
        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*)event);
    }
}

/* ==== Menus, Toolbar, About ==== */
static void on_about(GtkMenuItem *item, gpointer user_data) {
    gtk_show_about_dialog(GTK_WINDOW(((AppState*)user_data)->window),
        "program-name", "Data Manager Pro",
        "version", "1.0 Edu UX",
        "authors", (const gchar*[]){"Your Name", NULL},
        "license-type", GTK_LICENSE_MIT,
        NULL);
}

static GtkWidget* build_menu_bar(AppState *s) {
    GtkAccelGroup *accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(s->window), accel);

    GtkWidget *menubar = gtk_menu_bar_new();
    // File menu
    GtkWidget *file = gtk_menu_item_new_with_label("File");
    GtkWidget *fmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), fmenu);
    GtkWidget *open = gtk_menu_item_new_with_mnemonic("_Open...");
    gtk_menu_shell_append(GTK_MENU_SHELL(fmenu), open);
    gtk_widget_add_accelerator(open, "activate", accel, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(open, "activate", G_CALLBACK(on_open_file), s);
    GtkWidget *quit = gtk_menu_item_new_with_mnemonic("_Quit");
    gtk_widget_add_accelerator(quit, "activate", accel, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect_swapped(quit, "activate", G_CALLBACK(gtk_widget_destroy), s->window);
    gtk_menu_shell_append(GTK_MENU_SHELL(fmenu), quit);
    // Help menu
    GtkWidget *help = gtk_menu_item_new_with_label("Help");
    GtkWidget *hmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), hmenu);
    GtkWidget *about = gtk_menu_item_new_with_label("About");
    g_signal_connect(about, "activate", G_CALLBACK(on_about), s);
    gtk_menu_shell_append(GTK_MENU_SHELL(hmenu), about);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
    return menubar;
}

/* ==== UI Setup ==== */
static void activate(GtkApplication *app, gpointer data) {
    AppState *s = data;
    s->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(s->window), "Data Manager Pro: Edu Edition");
    gtk_window_set_default_size(GTK_WINDOW(s->window), 1024, 700);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(s->window), vbox);

    // Menu bar
    GtkWidget *menubar = build_menu_bar(s);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    
    // Toolbar omitted (can be added similarly)

    // File chooser dialog
    s->file_chooser = gtk_file_chooser_dialog_new("Open Data File",
        GTK_WINDOW(s->window), GTK_FILE_CHOOSER_ACTION_OPEN,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    g_signal_connect(s->file_chooser, "response",
        G_CALLBACK(+[](GtkDialog *d, gint r, gpointer u){ if(r==GTK_RESPONSE_ACCEPT) on_file_selected(NULL,u);} ), s);

    // Progress & Spinner
    s->spinner = gtk_spinner_new();
    s->progress = gtk_progress_bar_new();
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(hbox), s->spinner, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(hbox), s->progress, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 4);

    // Filter entry
    s->filter_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(s->filter_entry), "Filter words... (type to search)");
    gtk_box_pack_start(GTK_BOX(vbox), s->filter_entry, FALSE, FALSE, 2);

    // Notebook
    s->notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(vbox), s->notebook, TRUE, TRUE, 0);

    // Console
    s->txt_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(s->txt_output), FALSE);
    gtk_widget_set_hexpand(s->txt_output, TRUE);
    gtk_widget_set_vexpand(s->txt_output, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), gtk_scrolled_window_new(NULL,NULL), TRUE, TRUE, 0);
    GtkWidget *console = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(console), s->txt_output);
    gtk_box_pack_start(GTK_BOX(vbox), console, TRUE, TRUE, 0);
    g_signal_connect(console, "button-press-event", G_CALLBACK(on_console_menu), s);

    // Status bar
    s->statusbar = gtk_statusbar_new();
    s->context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(s->statusbar), "status");
    gtk_box_pack_start(GTK_BOX(vbox), s->statusbar, FALSE, FALSE, 0);

    gtk_widget_show_all(s->window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new(
        "com.example.DataManagerProEduUX", G_APPLICATION_FLAGS_NONE);
    AppState *state = g_malloc0(sizeof(AppState));
    g_signal_connect(app, "activate", G_CALLBACK(activate), state);
    int ret = g_application_run(G_APPLICATION(app), argc, argv);
    g_free(state);
    g_object_unref(app);
    return ret;
}
