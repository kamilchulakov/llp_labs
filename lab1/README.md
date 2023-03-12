# Setup
## Dependencies
- [cmake](../.github/cmake.jpg)
- make

## Makefile
Use `make build`, `run`, `test`, `clean` .

## Plain shell
### Build
```shell
cmake -B build && cd build && make
```
### Run
```shell
./app
```
### Test
```shell
./test
```
### Clean
```shell
rm -f app test && rm -rf build
```
