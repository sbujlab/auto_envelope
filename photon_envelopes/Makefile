save2file:
	g++ -c save2file_UScol4.c -o save2file.o `root-config --cflags --glibs`
	g++ save2file.o -o save2file `root-config --cflags --glibs`
	rm save2file.o

PhotonCol3:
	g++ -c centerPhotonEnv_col3.c -o centerPhotonEnv_col3.o `root-config --cflags --glibs`
	g++ centerPhotonEnv_col3.o -o PhotonCol3 `root-config --cflags --glibs`
	rm centerPhotonEnv_col3.o

PhotonNewCol4:
	g++ -c centerPhotonEnv_newCol4.c -o centerPhotonEnv_newCol4.o `root-config --cflags --glibs`
	g++ centerPhotonEnv_newCol4.o -o PhotonNewCol4 `root-config --cflags --glibs`
	rm centerPhotonEnv_newCol4.o

Angles:
	g++ -c centralAngles.c -o centralAngles.o `root-config --cflags --glibs`
	g++ centralAngles.o -o Angles `root-config --cflags --glibs`
	rm centralAngles.o
