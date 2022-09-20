WINFLAGS = -lglew32 -lglfw3 -lopengl32 -lglu32 -lgdi32
MACFLAGS = -lglew -lglfw -framework OpenGL
LINFLAGS = -lGLEW -lGLU -lGL -lglfw

win: main.cpp
	g++ -o test.exe main.cpp $(WINFLAGS) 

mac: main.cpp
	g++ -o test.app main.cpp $(MACFLAGS) 

linux: main.cpp
	g++ -o test.out main.cpp $(LINFLAGS) 