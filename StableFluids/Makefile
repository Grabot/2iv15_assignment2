
CXX = g++
CXXFLAGS = -g -O2 -Wall -Wno-sign-compare -Iinclude -DHAVE_CONFIG_H 
OBJS = StableFluids.o ScalarField.o VectorField.o FieldToolbox.o imageio.o

project2: $(OBJS)
	$(CXX) -o $@ $^ -lGLU -lglut -lpng 
clean:
	rm $(OBJS) project2
