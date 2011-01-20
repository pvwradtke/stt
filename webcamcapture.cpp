/*
Timelapse - Webcam Capture

Copyright 2011, Paulo Vinicius Wolski Radtke (pvwradtke@gmail.com)

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

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>

using namespace std;

int main(int argc, char** argv) {
    int counter = 0, interval = 30, fontsize = 40, cam = -1;
    bool test = false, show = false;
    int argpos = 1;
    char text[200], tempname[80], windowname[120];
    time_t rawtime, previous;
    struct tm * timeinfo;
    // Retrieves arguments
    while (argpos < argc) {
        if (strcmp(argv[argpos], "help") == 0) {
            cout << "Usage: webcamcapture [OPTIONS]" << endl;
            cout << "Where OPTIONS are a combination of the following:" << endl;
            cout << "--test | -t: toggles on test mode for webcam positioning ad lighting test" << endl;
            cout << "--webcam | -w: defaults to auto (-1), indicates webcam number (0, 1, 2 and so on)" << endl;
            cout << "--counter | -c : defaults to zero, indicates first frame index counter" << endl;
            cout << "--interval | -i : defaults to 30, sets the seconds counter between frames" << endl;
            cout << "--fontsize | -f : defaults to 40, change accordingly to your webcam resolution" << endl;
            cout << "--query | -q : shows and details available cameras among the first 10 possible. Any other parameter is ignored." << endl;
            cout << "--help | -h : shows this message. Any other parameter is ignored." << endl;
            cout << endl << "Parameters may be passed in any given order. The test parameter may need only the cam parameter, as the other parameters will be ignored." << endl;
            cout << "To test the font size, just let the program capture one screen and see the resulting image." << endl;
            return 0;
        } else if (strcmp(argv[argpos], "query") == 0) {
            cout << "Querying available webcams. Please wait or press CTRL+C to stop" << endl;
            for (int i = 0; i < 10; i++) {
                CvCapture* capture = cvCreateCameraCapture(i);
                if (capture != 0) {
                    cout << "Found camera " << i << ", capture resolution: " << cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH) << "x" <<
                            cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT) << endl;
                    cvReleaseCapture(&capture);
                }
            }
            return 0;
        }
        else if (strcmp(argv[argpos], "--webcam") == 0 || strcmp(argv[argpos], "-w") == 0) {
            cam = atoi(argv[argpos + 1]);
            if (cam < 0)
                cam = -1;
            argpos += 2;
        }
        else if (strcmp(argv[argpos], "--counter") == 0 || strcmp(argv[argpos], "-c") == 0) {
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
        else if (strcmp(argv[argpos], "--fontsize") == 0 || strcmp(argv[argpos], "-f") == 0) {
            fontsize = atoi(argv[argpos + 1]);
            if (fontsize < 8)
                fontsize = 40;
            argpos += 2;
        }
        else if (strcmp(argv[argpos], "--test") == 0 || strcmp(argv[argpos], "-t") == 0) {
            test = true;
            argpos++;
        }
        else {
            printf("Error! Unknown \"%s\" parameter. Bailing out.\n", argv[argpos]);
            return -1;
        }
    }
    if (test) {
        sprintf(windowname, "WebCamCapture - Testing Camera %i", cam);
        cvNamedWindow(windowname, CV_WINDOW_AUTOSIZE);
        CvCapture* capture = cvCreateCameraCapture(cam);
        IplImage* frame;
        while (1) {
            frame = cvQueryFrame(capture);
            if (!frame)
                break;
            cvShowImage("Camera", frame);
            char c = cvWaitKey(33);
            if (c == 27)
                break;
        }
        cvReleaseCapture(&capture);
        cvDestroyWindow(windowname);
    }
    else {

        sprintf(windowname, "WebCamCapture - Capturing Camera %i at each %i seconds - ESC to quit", cam, interval);
        cvNamedWindow(windowname, CV_WINDOW_AUTOSIZE);
        CvCapture* capture = cvCreateCameraCapture(cam);
        IplImage* frame;
        sprintf(tempname, "cam%i_temp.jpg", cam);
        time(&previous);
        while (1) {
            frame = cvQueryFrame(capture);
            if (!frame)
                break;
            cvShowImage(windowname, frame);
            time(&rawtime);
            if (rawtime - previous > interval) {
                printf("Capturing image cam%i_%010i.jpg\n", cam, counter);
                cvSaveImage(tempname, frame);
                timeinfo = localtime(&rawtime);
                sprintf(text, "convert %s -pointsize %i -stroke black -strokewidth 1 -fill white -draw \"text 50,%i '%s'\" cam%i_%010i.jpg",
                        tempname, fontsize, fontsize, asctime(timeinfo), cam, counter++);
                system(text);
                previous += interval;
            }
            char c = cvWaitKey(33);
            if (c == 27) break;
        }
        cvReleaseCapture(&capture);
        cvDestroyWindow(windowname);
    }

}

