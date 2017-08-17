	# g++ -o main -O3 -pthread  -std=c++11 $(SRCS)
	# clang++ -o main -std=c++11 -stdlib=libc++ -O3 $(SRCS)
SRCS=  z1.cpp functions.cpp controller.cpp genetic.cpp Drawer.cpp
LDFLAGS += -L/usr/local/Cellar/sfml/2.4.2_1/lib -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
all:
		clang++ -o main -std=c++11 -stdlib=libc++  $(SRCS) -framework OpenGL $(LDFLAGS) -g
clean : 
	rm -rf *.out
