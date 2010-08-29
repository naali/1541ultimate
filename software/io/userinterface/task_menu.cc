#include "task_menu.h"
#include <string.h>

IndexedList<PathObject*> main_menu_static_items(16, NULL);
static TaskMenuManager main_menu_clear_on_exit;

TaskMenu :: TaskMenu(PathObject *n, PathObject *obj) : ContextMenu(n, obj, 0, 0)
{
    parent_win = NULL;
    context_state = e_new;
    keyb = NULL;
    window = NULL;
}

// y_offs = line above selected, relative to window
// corner = line of selected, relative to window

TaskMenu :: ~TaskMenu(void)
{
}

void TaskMenu :: init(Screen *scr, Keyboard *key)
{
    parent_win = scr;
    keyb = key;

    if(context_state == e_new) {
        items = object->fetch_task_items(state->node->children);
        for(int i=0;i<main_menu_static_items.get_elements();i++) {
        	state->node->children.append(main_menu_static_items[i]);
        	items++;
        }
        if(!items) {
            printf("No items.. exit.\n");
            context_state = e_finished;
            return;
        }
        rows = items + 2;
        size_y = parent_win->get_size_y();
        if(rows > size_y) {
            rows = size_y;
        }
		y_offs = (size_y - rows) >> 1;
        
        max_len = 0;
        for(int i=0;i<state->node->children.get_elements();i++) {
        	PathObject *it = state->node->children[i];
            len = strlen(it->get_name());
            if(len > max_len)
                max_len = len;
        }
        if(max_len > 30)
            max_len = 30;
    }        

    window = new Screen(parent_win, 19-(max_len>>1), y_offs+2, max_len+2, rows);
    window->draw_border();
    context_state = e_active;
}
