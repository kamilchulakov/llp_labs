## Lab 2
- Variant: [MONGOSH](MONGOSH.md)
- Task: [TASK](TASK.md)
- Status: IN PROGRESS...

### Examples
#### createCollection
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

#### collection.count
```
INPUT:
db.movies.count( {
  year: 2023,
  $or: [ author: "Gofard", rating: 10 ]
});

OUTPUT:
count:
	collection: movies
	criteria:
		field_criteria:
			field: year
				op: equal
			value: 2023
		operator_criteria:
			operator: or
			criteria:
				field_criteria:
					field: author
					op: equal
					value: "Gofard"
				field_criteria:
					field: rating
					op: equal
					value: 10
```

#### collection.find
```
INPUT:
db.inventory.find( { status: "A", qty: { $lt: 30 } } ).limit(5);

OUTPUT:
find:
	limit: 5
	collection: inventory
	criteria:
		field_criteria:
			field: status
			op: equal
			value: "A"
		field_criteria:
			field: qty
			op: less
			value: 30
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
  - [x] bool expressions
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