find . -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' -type f | xargs etags && cmake -B build -S . && cmake --build build && ./build/src/breakout-lev
