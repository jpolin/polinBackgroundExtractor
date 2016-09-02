/*
 * extractor.hpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 * Description: Library used for loading video, analyzing, and returning image of background.
 *       Usage:
 *
 *       		bgExtractor extractor;
 *       		extractor.loadVideoFile(inputFile);
 *       		extractor.analyzeFile();
 *       		extractor.writeBackgroundToFile(outputFile);
 *
 */

#ifndef EXTRACTOR_HPP_
#define EXTRACTOR_HPP_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <atomic>

#include "opencv2/opencv.hpp"
#include "opencv2/video/background_segm.hpp"

// How many frames the algorithm will use to get background
#define MAXBUFFERFRAMES 3000

using namespace std;
using namespace cv;

class bgExtractor {

	string videoName;
	Mat backgroundImage;
	cv::VideoCapture vid;

public:

	// Default constructor
	bgExtractor();

	// Load a video, return true if found successfully.
	const bool loadVideoFile(const string &filename);

	/* Analyze video and populate backgroundImage.
	 *
	 * If returns true, then background image was successfully extracted and
	 * can be recovered by calling getBackgroundImage();
	 *
	 * If returns false, then read errorMessage for more info.
	 */
	const bool analyzeFile();

	/*
	 * Write current background to filename; return true if success or false
	 * (along with update to errorMessage) if failed.
	 */
	const bool writeBackgroundToFile(const string& filename) ;

	// Populated with error message by any faulting method
	string errorMessage;

	// Store current progress of analysis in threadsafe var
	atomic_uint progress;

};


#endif /* EXTRACTOR_HPP_ */
