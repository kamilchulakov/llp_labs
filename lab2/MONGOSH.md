## mongosh

<a href="https://www.mongodb.com/docs/manual/reference/method/">
mongodb docs
</a>

- use $db;

### Database Methods 
- db
  - .createCollection($collection, $schema)
  - .getCollection(query criteria)
  - .dropDatabase()

### Collection Methods
- db.collection
  - .count(query criteria)
  - .find(query criteria)
  - .findOne(query criteria)
  - .insertOne($document)
  - .insertMany([$document, ...])
  - .remove(query criteria)
  - .removeOne(query criteria)
  - .update(query criteria, update/document)

### Comparison
$eq - Matches all values that are equal to a specified value.

$ne - Matches all values that are not equal to a specified value.

$gt - Matches numeric values that are greater than a specified value.

$gte - Matches numeric values that are greater than or equal to a specified value.

$lt - Matches numeric values that are less than a specified value.

$lte - Matches numeric values that are less than or equal to a specified value.

$regex - Matches string values that match pattern

### Conditions
https://www.mongodb.com/docs/manual/tutorial/query-documents/#specify-or-conditions


### Examples

#### Data
```
db.movies.find( {
     year: 2010,
     $or: [ { "awards.wins": { $gte: 5 } }, { genres: "Drama" } ]
} );

db.movies.count( {
  year: 2023,
  $or: [ { author: { $ne: "Truffaut" } ]
});

db.movies.count( {
  year: 2022,
  released: 2023
});
```

```
db.inventory.find( { status: "A", qty: { $lt: 30 } } );

db.inventory.removeOne( { $and: [ { amount: { $gt: 2}, ytq: { $lte: 1 } } ] } );

db.movies.insertOne(
  {
    title: "The_Favourite",
    genre: "Drama",
    runtime: 121,
    rated: "R",
    year: 2018,
    type: "movie"
  }
);

db.listingsAndReviews.update(
  { security_deposit: { $lt: 100 } },
  {
    $set: { security_deposit: 100, minimum_nights: 1 }
  }
);

db.Employee.insertOne({
  emp_id: 1,
  emp_name: "ABC",
  emp_address: "Mumbai", 
  emp_phone: 1234567890,
  emp_married: true,
  emp_salary: 2.5
});
```

#### Schemas (as I imagined)
```
db.createCollection("example", 
  {
    name: "string",
    int_field: "int32",
    double_field: "double",
    is_example: "bool"
  } 
);

db.getCollection("example");
```


## AST
![ast](AST.png)