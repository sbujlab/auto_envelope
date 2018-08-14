make reader:
	g++ -c reader.c -o reader.o `root-config --cflags --glibs`
	g++ reader.o libremoll.so -o reader `root-config --cflags --glibs` -L. -lremoll -Wl,-R.
	rm reader.o

make pruneTreeEnvelope:
	g++ -g -c pruneTreeEnvelope.c -o pruneTreeEnvelope.o `root-config --cflags --glibs`
	g++ pruneTreeEnvelope.o libremoll.so -o pruneTreeEnvelope `root-config --cflags --glibs` -L. -lremoll -Wl,-R.
	rm pruneTreeEnvelope.o

clean:
	rm ./*.o
	rm ./fom
