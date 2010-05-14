all: runmodel

runmodel: model.o runmodel.o modeloutput.o
	g++ -g -O0 -o runmodel runmodel.o model.o modeloutput.o

model.o: model.cpp
	g++ -c -g -O0  model.cpp 

runmodel.o: runmodel.cpp
	g++ -c -g -O0  runmodel.cpp 

modeloutput.o: modeloutput.cpp
	g++ -c -g -O0  modeloutput.cpp 

clean: 
	rm -rf *.o 
