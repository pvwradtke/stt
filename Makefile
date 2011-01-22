all :	cam	desktop
cam :
	g++ webcamcapture.cpp -o webcamcapture -lcv -lhighgui -O3
desktop :
	gcc desktopcapture.c -o desktopcapture -O3
clean :
	rm desktopcapture ; rm webcamcapture
install:
	cp desktopcapture /usr/bin ; cp webcamcapture /usr/bin
uninstall:
	rm /usr/bin/desktopcapture ; rm /usr/bin/webcamcapture
