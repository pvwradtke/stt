all :	cam	desktop
cam :
	g++ webcamcapture.cpp -o webcamcapture -lcv -lhighgui
desktop :
	gcc desktopcapture.c -o desktopcapture
clean :
	rm desktopcapture ; rm webcamcapture
install:
	cp desktopcapture /usr/bin ; cp webcamcapture /usr/bin
uninstall:
	rm /usr/bin/desktopcapture ; rm /usr/bin/webcamcapture
