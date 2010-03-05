all: runmodel

runmodel: model.o runmodel.o
	g++ -g -O0 -o runmodel runmodel.o model.o

model.o: model.cpp
	g++ -c -g -O0  model.cpp 

runmodel.o: runmodel.cpp
	g++ -c -g -O0  runmodel.cpp 

clean: 
	rm -rf *.o 
