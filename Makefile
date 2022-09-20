LDFLAGS = -lglew32 -lglfw3 -lopengl32 -lglu32 -lgdi32

compilacion: main.cpp
	g++ -o test.exe main.cpp $(LDFLAGS) 