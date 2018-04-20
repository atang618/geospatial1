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

#define ROW 2032
#define COL 2032
#define ALPHA 0.3
#define THRESH 30
#define WINSIZE 3
#define HISTORY 400
#define NUM_IMAGES 300
#define BRIGHTEN 1.8
#define SKIP 1

#define THRESHLO 70
#define THRESHHI 150
#define APTSIZE 3

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    // Initial camera and filename
    int cam = 5;
    int file = 393408606;
    int imgNum = 1;
    // Initialize Gaussian Mixture Model
    Ptr<BackgroundSubtractorMOG2> detector = createBackgroundSubtractorMOG2(HISTORY);
    
//    namedWindow("Mask", WINDOW_NORMAL);
//    namedWindow("Filtered Frame", WINDOW_NORMAL);
//    namedWindow("Final Result", WINDOW_NORMAL);
//    Mat finalMask, finalThresh;
    Mat cumAvg(ROW, COL, CV_64F);
    Mat cumAvgInt(ROW, COL, CV_64F);
    Mat cumAvgGrad(ROW, COL, CV_64F);
    Mat delta, deltaNorm, deltaFinal, prev;
    Mat deltaGrad, deltaGradNorm, deltaGradFinal, prevGrad;
    Mat prevEdge;
    Mat model;
    double min, max, mean;
    while (true) {
        ostringstream stringStream;
        stringStream << "../../../sample_drive/sample_drive/cam_" << cam << "/" << file << ".jpg";
        string imagefn = stringStream.str();
        Mat frame, gray, grayGrad, grayEdge;
        Mat fgmask;
        frame = imread(imagefn);
        if (frame.channels()!=3 || file%SKIP !=0) {
            file++;
            continue;
        }
        
        // Initial Processing Steps
        cvtColor(frame, gray, CV_BGR2GRAY);
        imshow("Original", frame);
        Scharr(gray, grayGrad, CV_64F, 1, 0);
        imshow("Gradient in X", grayGrad);
        Canny(gray, grayEdge, THRESHLO, THRESHHI, APTSIZE);
        imshow("Edges", grayEdge);

        //resize(frame, frame, Size(frame.cols/2, frame.rows/2));

        if (imgNum == 1){
            ;
        } else {

            //deltaFinal = delta.clone()*BRIGHTEN;
            //deltaFinal = delta.clone();
            //imshow("Delta Final", deltaFinal);
            
            
            // Update Intensity
            absdiff(grayEdge, prevEdge, delta);
            imshow("Intensity Delta", delta);
            normalize(delta, deltaNorm, 1.0, 0.0, NORM_INF);
//            minMaxLoc(delta, &min, &max);
//            cout << "Max pixel value" << max;
//            deltaNorm = delta/max;
            deltaFinal = deltaNorm.clone()*255;
            imshow("Intensity Final Delta",deltaFinal);
            add(deltaFinal,((imgNum-1)*cumAvgInt),cumAvgInt, noArray(), CV_64F);
            cumAvgInt = cumAvgInt/imgNum;
            imshow("Intensity Cumulative Average", cumAvgInt);
            
//            // Update Background Model
            detector->apply(deltaFinal, fgmask, ALPHA);
            detector->getBackgroundImage(model);
            imshow("Background Model", model);

            
            // Update Gradient
            absdiff(grayGrad, prevGrad, deltaGrad);
            imshow("Gradient Delta", deltaGrad);
            normalize(deltaGrad, deltaGradNorm, 1.0, 0.0, NORM_INF);
            deltaGradFinal = deltaGradNorm.clone()*255;
            imshow("Gradient Final Delta", deltaGradFinal);
            add(deltaGradFinal,((imgNum-1)*cumAvgGrad),cumAvgGrad);
            cumAvgGrad = cumAvgGrad/imgNum;
            imshow("Gradient Cumulative Average", cumAvgGrad);
        }

//        imshow("Background Model", model);
//        imshow("Mask", fgmask);
//        Mat thresh;
//        threshold(fgmask, thresh, THRESH, 255, THRESH_BINARY);
//        imshow("Thresh", thresh);
//        Mat filteredFrame, finalFrame;
//        frame.copyTo(filteredFrame, thresh);
        //filteredFrame.copyTo(finalFrame);
        //Mat element = getStructuringElement(MORPH_RECT, Size(WINSIZE,WINSIZE));
        //morphologyEx(finalFrame, finalFrame, MORPH_CLOSE, element);
        //dilate(finalFrame, finalFrame, element);

        //imshow("Filtered Frame",filteredFrame);
        //imshow("Final Result", finalFrame);
//        while (waitKey(30) <= 0) {
//            ;
//        }
        
        if (waitKey(15) >=0) {
            break;
        }
        gray.copyTo(prev);
        grayGrad.copyTo(prevGrad);
        grayEdge.copyTo(prevEdge);
        file++;
        imgNum++;
        //393413167
        if (file > 393408606 + NUM_IMAGES) {
//            fgmask.copyTo(finalMask);
//            thresh.copyTo(finalThresh);
            break;
        }
    }
//    imshow("Resulting Model", model);
//    imshow("Final Mask", finalMask);
//    imshow("Final Thresh", finalThresh);
//    while (waitKey(30) <= 0) {
//        ;
//    }
}
