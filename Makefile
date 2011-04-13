all: clean build test

build:
	node-waf configure build

clean:
	@rm -rf ./build

test:
	node test.js
