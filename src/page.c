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
