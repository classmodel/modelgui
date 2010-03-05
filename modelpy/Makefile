ribtol.so : ribtol.o
	g++ -O3 -shared -Wl -z -def -o ribtol.so -lpython2.6 -lboost_python ribtol.o

ribtol.o : ribtol.cpp
	g++ -c -O3 -fPIC ribtol.cpp -I/usr/include/python2.6

clean : 
	rm -rf ribtol.o ribtol.so
