#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "queue.h"
#include "stacks.h"
#include "binarySearchTrees.h"
#include "recursion.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    // Data structures
    TList    *syn;           /**< synonym list */
    TList    *ant;           /**< antonym list */
    TStack   *stk;           /**< stack example */
    TQueue   *que;           /**< queue example */
    TTree    *tree;          /**< binary search tree example */

    // GTK widgets
    GtkWidget *window;       /**< main application window */
    GtkWidget *notebook;     /**< tabbed view container */
    GtkWidget *txt_output;   /**< text console output */
    GtkWidget *file_chooser; /**< file chooser dialog */
    GtkWidget *progress;     /**< progress bar widget */
    GtkWidget *spinner;      /**< spinner for loading */
    GtkWidget *statusbar;    /**< status bar for feedback */
    guint      context_id;   /**< status bar context ID */
    GtkWidget *filter_entry; /**< text entry for filtering data */
} AppState;

/* Utility functions */
void append_text(AppState *s, const gchar *msg);
void set_status (AppState *s, const gchar *msg);

/* File loading callbacks */
gboolean load_data_async    (gpointer user_data);
void     on_open_file        (GtkMenuItem *menuitem, gpointer user_data);
void     on_file_selected    (GtkButton   *btn, gpointer user_data);

/* TreeView filtering */
GtkTreeModel* create_filter_model(GtkListStore *store, AppState *s);
GtkWidget*    list_view_with_filter(AppState *s);

/* Console context menu */
void on_console_menu(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

/* About dialog */
void on_about(GtkMenuItem *item, gpointer user_data);

/* Menu bar construction */
GtkWidget* build_menu_bar(AppState *s);

/* GTK application activation */
void activate(GtkApplication *app, gpointer data);

#ifdef __cplusplus
}
#endif

#endif /* GUI_H */
