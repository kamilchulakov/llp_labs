BUILD_DIR = build
build: clean
	mkdir ${BUILD_DIR}
	cmake -B build && cd ${BUILD_DIR} && cmake

run: build
	./app

test: build
	./test

clean:
	rm -f app test
	rm -rf build
