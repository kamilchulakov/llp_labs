#include "page.h"

page* create_empty_page(uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    pg->page_id = page_id;
    pg->is_full = false;
    pg->prevPageId = -1;
    pg->nextPageId = -1;
    pg->used_mem = 42;
    pg->last_modified = time(NULL);
    pg->type = PAGE_EMPTY;
    return pg;
}

bool page_has_type(page* page, page_type type) {
    if (page->type == type)
        return true;
    return false;
}
