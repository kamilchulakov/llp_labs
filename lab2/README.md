## Lab 2
- Variant: [MONGOSH](MONGOSH.md)
- Task: [TASK](TASK.md)
- Status: IN PROGRESS...

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