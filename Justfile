# Default recipe
default: build test

# Build the project
build:
    mkdir -p build
    cd build && cmake .. && make

# Run all tests
test: build
    ./build/memory_tests

# Clean build directory
clean:
    rm -rf build/
    rm -rf cmake-build-debug

# Format code
format:
    find . -name "*.c" -o -name "*.h" | xargs clang-format -i

# Run memcheck
memcheck: build
    valgrind --leak-check=full ./build/memory_tests

# Docker commands
docker-build:
    docker build -t memory-alloc .

docker-test: docker-build
    docker run --rm memory-alloc ./build/memory_tests

docker-clean: clean

docker-memcheck: docker-build
    docker run --rm memory-alloc valgrind --leak-check=full ./build/memory_tests