#include "page.h"

page* create_empty_page(uint32_t page_id) {
    page* pg = malloc(sizeof(page));
    pg->page_header.page_id = page_id;
    pg->page_header.next_page_id = 0;
    pg->page_header.used_mem = 0;
    pg->page_header.last_modified = time(NULL);
    pg->page_header.type = PAGE_EMPTY;
    return pg;
}

int debug_page(page* page) {
    return printf("\nPAGE_ID:%u USED_MEM:%u PAGE_TYPE:%u TIME:%ld\n", page->page_header.page_id, page->page_header.used_mem, page->page_header.type, page->page_header.last_modified);
}
