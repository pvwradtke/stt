	/*
Timelapse - Desktop Capture

Copyright 2010-2011, Paulo Vinicius Wolski Radtke (pvwradtke@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#if defined(linux)
	#include <unistd.h>
#endif
#if defined(_MSC_VER)
	#include <windows.h>
#endif
#include <time.h>

int main(int argc, char **argv) {
    int counter = 0, interval = 30, fontsize = 40;
    char text[200];
    time_t rawtime;
    struct tm * timeinfo;
    int argpos = 1;
    // Retrieves arguments and do a sanity check, just in case
    while (argpos < argc) {
        if (strcmp(argv[argpos], "help") == 0) {
            printf("Usage: desktopcapture [ OPTIONS ]\n");
            printf("Where OPTIONS is a combination of one or more of the following:\n");
            printf("--counter | -c : defaults to zero, indicates first frame counter\n");
            printf("--interval | -i : defaults to 30 seconds, sets the interval  between frames\n");
            printf("--fontsize | -f : defaults to 40, change accordingly to your desktop resolution\n");
            printf("--help | -h : shows this message. Any other parameter is ignored.\n");
            printf("Parameters may be passed in any given order. \n");
            return 0;
        } else if (strcmp(argv[argpos], "--counter") == 0 || strcmp(argv[argpos], "-c") == 0) {
            counter = atoi(argv[argpos + 1]);
            if (counter < 0)
                counter = 0;
            argpos += 2;
        }
        else if (strcmp(argv[argpos], "--interval") == 0 || strcmp(argv[argpos], "-i") == 0) {
            interval = atoi(argv[argpos + 1]);
            if (interval <= 0)
                interval = 30;
            argpos += 2;
        }
        else if (strcmp(argv[argpos], "--fontsize") == 0 && strcmp(argv[argpos], "-f") == 0) {
            fontsize = atoi(argv[argpos + 1]);
            if (fontsize < 8)
                fontsize = 40;
            argpos += 2;
        } else {
            printf("Error! Unknown \"%s\" parameter. Bailing out.\n", argv[argpos]);
            return -1;
        }
    }
    while (1) {
				printf("Waiting %i seconds. Presss CTRL+C to quit (quick'n'dirty).\n", interval);
				#if defined(linux)
					usleep(1000000 * interval);
				#endif
				#if defined(_MSC_VER)
					Sleep(1000 * interval);
				#endif
        printf("Capturing image desktop%010i.jpg\n", counter);
        system("import -window root desktemp.png");
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        sprintf(text, "convert desktemp.png -pointsize %i -stroke black -strokewidth 1 -fill white -draw \"text 50,%i '%s'\" desktop%010i.jpg", fontsize, fontsize, asctime(timeinfo), counter++);
        system(text);
    }
    return 0;
}
