//
//  main.cpp
//  denoise
//
//  Created by Allen Tang on 4/17/18.
//  Copyright © 2018 Allen Tang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define ALPHA 0.01
#define THRESH 30
#define WINSIZE 3
#define HISTORY 4000

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    // Initial camera and filename
    int cam = 1;
    int file = 393408606;

    // Initialize Gaussian Mixture Model
    Ptr<BackgroundSubtractorMOG2> detector = createBackgroundSubtractorMOG2(HISTORY);
    
//    namedWindow("Mask", WINDOW_NORMAL);
//    namedWindow("Filtered Frame", WINDOW_NORMAL);
//    namedWindow("Final Result", WINDOW_NORMAL);
    Mat finalMask, finalThresh;
    
    while (true) {
        ostringstream stringStream;
        stringStream << "../../../sample_drive/sample_drive/cam_" << cam << "/" << file << ".jpg";
        string imagefn = stringStream.str();
        Mat frame, gray, fgmask;
        frame = imread(imagefn);
        if (frame.channels()!=3) {
            file++;
            continue;
        }
        cvtColor(frame, gray, CV_BGR2GRAY);
        //imshow("Original", frame);
        //resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        detector->apply(gray, fgmask, ALPHA);
        //imshow("Mask", fgmask);
        Mat thresh;
        threshold(fgmask, thresh, THRESH, 255, THRESH_BINARY);
        //imshow("Thresh", thresh);
        Mat filteredFrame, finalFrame;
        frame.copyTo(filteredFrame, thresh);
        //filteredFrame.copyTo(finalFrame);
        //Mat element = getStructuringElement(MORPH_RECT, Size(WINSIZE,WINSIZE));
        //morphologyEx(finalFrame, finalFrame, MORPH_CLOSE, element);
        //dilate(finalFrame, finalFrame, element);

        //imshow("Filtered Frame",filteredFrame);
        //imshow("Final Result", finalFrame);
//        while (waitKey(30) <= 0) {
//            ;
//        }
//        if (waitKey() >=0) {
//            break;
//        }
        file++;
        //393413167
        if (file > 393411000) {
            fgmask.copyTo(finalMask);
            thresh.copyTo(finalThresh);
            break;
        }
    }
    Mat model;
    detector->getBackgroundImage(model);
    imshow("Resulting Model", model);
    imshow("Final Mask", finalMask);
    imshow("Final Thresh", finalThresh);
    while (waitKey(30) <= 0) {
        ;
    }
}
