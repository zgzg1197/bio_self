CXX = g++
CXXFLAGS = -std=c++11 -O3 -D__DEBUG__
INCLUDES =   

OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

.PHONY: all
all: build 

build: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o test $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

run:
	./test 
sim:
	./test ../data/vgref.txt /home/zengg/tmp/data/6m24K_2K/x_1800E.sim ./error.result  


.PHONY: clean
clean:
	-rm test
	-rm *.o