BUILD_DIR = build
build: clean
	mkdir ${BUILD_DIR}
	cmake -B build && cmake --build ${BUILD_DIR}

run: build
	./app

test: build
	./test

clean:
	rm -f app test
	rm -rf build
