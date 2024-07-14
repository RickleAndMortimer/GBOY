gpu : src/gpu.cpp test/test_gpu.cpp
	mkdir -p bin
	g++ src/gpu.cpp test/test_gpu.cpp -Isrc/ -o bin/gpu `sdl2-config --cflags --libs`
clean : 
	rm -rf bin/
