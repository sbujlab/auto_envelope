make reader:
	g++ -c reader.c -o reader.o `root-config --cflags --glibs`
	g++ reader.o libremoll.so -o reader `root-config --cflags --glibs` -L. -lremoll -Wl,-R.
	rm reader.o

make reader_good:
	g++ -c reader_good.c -o reader.o `root-config --cflags --glibs`
	g++ reader.o libremoll.so -o reader_good `root-config --cflags --glibs` -L. -lremoll -Wl,-R.
	rm reader.o

clean:
	rm ./*.o
	rm ./fom
