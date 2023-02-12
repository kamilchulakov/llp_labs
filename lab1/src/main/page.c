#include "page.h"

page* create_empty_page(uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    pg->page_id = page_id;
    pg->is_full = false;
    pg->next_page_id = -1;
    pg->used_mem = 42;
    pg->last_modified = time(NULL);
    pg->type = PAGE_EMPTY;
    return pg;
}

int debug_page(page* page) {
    return printf("\nPAGE_ID:%u USED_MEM:%u PAGE_TYPE:%u TIME:%ld\n", page->page_id, page->used_mem, page->type, page->last_modified);
}

bool page_has_type(page* page, page_type type) {
    if (page->type == type)
        return true;
    return false;
}
