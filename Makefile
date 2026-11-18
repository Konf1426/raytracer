PRESET ?= unix-debug

.PHONY: build run test clean

build:
	cmake --preset $(PRESET)
	cmake --build --preset $(PRESET)

run: build
	cmake --build build/$(PRESET) --target run

test: build
	ctest --preset $(PRESET)

clean:
	cmake --build build/$(PRESET) --target clean
