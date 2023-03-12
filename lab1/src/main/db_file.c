#include "db_file.h"
#include "logger.h"

pager_data* create_header();
mem_info* create_mem_info();
db_handler* create_db_handler(char* filename, FILE* fp, pager_data* header);
WRITE_STATUS write_db_header(FILE* fp, pager_data* header);

size_t db_header_size() {
    return sizeof(pager_data);
}

db_handler* open_db_file(char* db_name) {
    FILE* file = fopen(db_name, "w+");
    if (file == NULL)
        return NULL;
    pager_data* header = create_header();

    db_handler* handler = create_db_handler(db_name, file, header);
    return handler;
}

void utilize_db_file(db_handler* db) {
    remove(db->filename);
    free(db->pagerData->mem);
    free(db->pagerData);
    free(db);
}

void seek_db_header(FILE* fp) {
    fseek(fp, 0, SEEK_SET);
}

db_handler* create_db_handler(char* filename, FILE* fp, pager_data* header) {
    db_handler* handler = malloc(sizeof(db_handler));
    handler->filename = filename;
    handler->fp = fp;
    handler->pagerData = header;
    return handler;
}

pager_data* create_header() {
    pager_data* header = malloc(sizeof(pager_data));
    header->mem = create_mem_info();
    header->pageIdSeq = 1;
    header->lastCollectionPage = -1;
    header->firstFreeCollectionPageId = -1;
    header->lastDocumentPage = -1;
    header->firstFreeDocumentPageId = -1;
    header->firstFreeStringPageId = -1;
    header->lastStringPage = -1;
    return header;
}

WRITE_STATUS write_db_header(FILE* fp, pager_data* header) {
    seek_db_header(fp);
    if (fwrite(header, sizeof(pager_data), 1, fp) == 1)
        return WRITE_OK;
    return WRITE_ERROR;
}

mem_info* create_mem_info() {
    mem_info* mem = malloc(sizeof(mem_info));
    mem->used_mem = 0;
    mem->allocated_mem = 0;
    return mem;
}

int debug_mem_info(db_handler* db_handler) {
    return printf("\nAllocated mem:%u\nUsed mem:%u\n", db_handler->pagerData->mem->allocated_mem, db_handler->pagerData->mem->used_mem);
}

uint32_t get_and_set_page_id(db_handler* db_handler) {
    uint32_t curr_page_id = db_handler->pagerData->pageIdSeq;
    db_handler->pagerData->pageIdSeq = curr_page_id + 1;
    db_handler->pagerData->mem->allocated_mem += PAGE_SIZE;
    write_db_header(db_handler->fp, db_handler->pagerData);
    return curr_page_id;
}