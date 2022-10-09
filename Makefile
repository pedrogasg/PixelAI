CFLAGS = -std=c++17 -I. -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

main.out: *.cpp *.hpp
	g++ $(CFLAGS) -o main.out *.cpp $(LDFLAGS)

.PHONY: test clean

test: main.out
	./main.out

clean:
	rm -f main.out