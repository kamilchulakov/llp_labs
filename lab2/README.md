## Lab 2
- Variant: [MONGOSH](MONGOSH.md)
- Task: [TASK](TASK.md)
- Status: SUBMITTED
- Report: [PDF](Отчёт%20LLP2.pdf)

### Examples
#### createCollection
```mongodb
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
```mongodb
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
```mongodb
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

#### collection.insert
```mongodb
INPUT:
db.products.insertOne( { item: "card", qty: 15 } );

OUTPUT:
insert:
	collection: products
	document:
		element:
			field: item
			value:
				type: string
				val: "card"
		element:
			field: qty
			value:
				type: int32
				val: 15
```

#### collection.update
```mongodb
INPUT:
db.listingsAndReviews.update(
  { security_deposit: { $lt: 100 } },
  {
    $set: { security_deposit: 100, minimum_nights: 1 }
  }
);

OUTPUT:
update:
	collection: listingsAndReviews
	criteria:
		field_criteria:
			field: security_deposit
			op: less
			value:
				type: int32
				val: 100
	document:
		element:
			field: security_deposit
			value:
				type: int32
				val: 100
		element:
			field: minimum_nights
			value:
				type: int32
				val: 1
```

### TODO
- [x] check mongosh
- [x] prepare build
- [x] lexer + parser
- [x] printer
- [x] free mem

#### Lexer
- [x] regular expressions for tokens
  - [x] mongosh keywords
  - [x] bool expressions
- [ ] handle big strings

#### Parser
- [x] tokens
- [x] parsing tokens with print
- [x] db queries
- [x] collection queries
- [x] types

### Project structure
- src/lexer.l = lexer
- src/parser.y = parser
- src/main.c = main for easier manual testing