all: runmodel

runmodel: model.o runmodel.o
	g++ -O3 -o runmodel runmodel.o model.o

model.o: model.cpp
	g++ -c -O3 model.cpp 

runmodel.o: runmodel.cpp
	g++ -c -O3 runmodel.cpp 

clean: 
	rm -rf *.o 
