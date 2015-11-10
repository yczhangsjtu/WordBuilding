building: Building.cpp draw.o wordType.o
	g++ $< draw.o wordType.o -o $@ -lGL -lGLU -lglut

draw.o: draw.cpp
	g++ -c $< -o $@

wordType.o: wordType.cpp
	g++ -c $< -o $@
