make:
	g++ -c reader.c -o reader.o `root-config --cflags --glibs`
	g++ reader.o libremoll.so -o reader `root-config --cflags --glibs` -L. -lremoll -Wl,-R.
	rm reader.o

clean:
	rm ./*.o
	rm ./fom
