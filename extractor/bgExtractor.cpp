/*
 * extractor.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "bgExtractor.hpp"

using namespace std;
using namespace cv;

const bool bgExtractor::loadVideoFile(const string &filename){
	vid.open(filename);
	return vid.isOpened();
}

const bool bgExtractor::analyzeFile(){
	// Make sure it's open
	if (!vid.isOpened()) return false;

	// Parameters for background extractor/subtractor (set for method)
	int history, nmixtures;
	double backgroundRatio;

	// Use mixture of Gaussians
	BackgroundSubtractorMOG2 bgSubtractor(
			/*int history*/ vid.get(CV_CAP_PROP_FRAME_COUNT),
			/*float varThreshold*/ 0.01,
			/*bool bShadowDetection=true*/ true);

#ifdef VISUALIZE
	// Open window to visualize whatever is done in loop
	namedWindow("Visualizer", 1);
	double period = 1.0/vid.get(CV_CAP_PROP_FPS);
#endif

	// Iterate through frames
	Mat frame, mask;
	while (vid.read(frame)){

		// Update subtractor
		bgSubtractor(frame, mask);

#ifdef VISUALIZE
//		imshow("Visualizer", mask);
//		cvWaitKey(1e3 * period);
#endif

	}

	Mat backgroundImage;
	bgSubtractor.getBackgroundImage(backgroundImage);
#ifdef VISUALIZE
	imshow("Visualizer",backgroundImage);
	waitKey(0);
#endif
	return true;
}



