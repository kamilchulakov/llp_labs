#ifndef ENORMEDB_ELEMENT_H
#define ENORMEDB_ELEMENT_H

#include "io.h"
#include "bson.h"

typedef struct {
    field* e_field;
    union {
        int32_t int_data;
        double double_data;
        bool bool_data;
        string* string_data;
        string_part* string_split;
    };
} element;

element* create_element(ELEMENT_TYPE e_type, char* e_name);
element* create_element_int32(char* e_name, int32_t value);
READ_STATUS read_element(FILE* fp, element* el);
WRITE_STATUS write_element(FILE* fp, element* el);

typedef enum {
    CMP_EQ = 0,
    CMP_NEQ = 1,
    CMP_GT = 2,
    CMP_GTE = 3,
    CMP_LT = 4,
    CMP_LTE = 5,
    CMP_REGEX = 6
} CMP_TYPE;
typedef struct element_filter element_filter;
struct element_filter {
    CMP_TYPE type;
    element* el;
    element_filter* nxt;
};

typedef enum {
    OR_OPERATOR = 0,
    AND_OPERATOR
} OP_TYPE;
typedef struct operator_filter operator_filter;
typedef enum { ELEMENT_FILTER, OPERATOR_FILTER } FILTER_TYPE;
typedef struct complex_filter complex_filter;
struct operator_filter {
    complex_filter* flt1;
    OP_TYPE type;
    complex_filter* flt2;
};
struct complex_filter {
  union {
      operator_filter* op_filter;
      element_filter* el_filter;
  };
  FILTER_TYPE type;
};

element_filter* create_element_filter(
        CMP_TYPE op_type,
        element* el);

element_filter* create_element_filter_linked(
        CMP_TYPE op_type,
        element* el,
        element_filter* prev,
        element_filter* nxt);

bool cmp_elements(CMP_TYPE cmp_type, element* first, element* second);

#endif //ENORMEDB_ELEMENT_H
