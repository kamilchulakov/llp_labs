## nemongosh

<a href="https://www.mongodb.com/docs/manual/reference/method/">
<p style="background-color: antiquewhite; width: fit-content">mongonsh</p>
</a>

- use $db;

### Database Methods 
- db
  - .createCollection($collection, $schema)
  - .getCollection(query criteria)
  - .dropDatabase()
  - .getName()

### Collection Methods
- db.collection
  - .count(query criteria)
  - .insertOne($document)
  - .insertMany([$document, ...])
  - .remove(query criteria, justOne)
  - .renameCollection(target, dropTarget)
  - .update(query criteria, update/document)
  - .updateOne(query criteria, update/document)
