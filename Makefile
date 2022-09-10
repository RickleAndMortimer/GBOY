gpu : gpu.cpp
	g++ gpu.cpp test_gpu.cpp -o gpu `sdl2-config --cflags --libs`
