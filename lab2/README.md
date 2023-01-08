## Lab 2
- Variant: [MONGOSH](MONGOSH.md)
- Task: [TASK](TASK.md)
- Status: IN PROGRESS...

### Examples
```
INPUT:
db.createCollection("example", 
  {
    name: "string",
    int_field: "int32",
    double_field: "double",
    is_example: "bool"
  } 
);

OUTPUT:
createCollection:
	collection: "example"
	schema:
		schema_field:
			field_name: name
			field_type: "string"
		schema_field:
			field_name: int_field
			field_type: "int32"
		schema_field:
			field_name: double_field
			field_type: "double"
		schema_field:
			field_name: is_example
			field_type: "bool"
```

### TODO
- [x] check mongosh
- [x] prepare build
- [ ] lexer
- [ ] parser
- [ ] visitor

#### Lexer
- [x] regular expressions for tokens
  - [x] mongosh keywords
  - [ ] bool expressions
- [ ] regular expressions for values
- [ ] handle big strings

#### Parser
- [x] tokens
- [x] parsing tokens with print
- [x] db queries
- [ ] collection queries
- [ ] types

### Project structure
- src/lexer.l = lexer
- src/parser.y = parser
- src/main.c = main for easier manual testing