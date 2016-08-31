/*
 * extractor.hpp
 *
 *  Created on: Aug 29, 2016
 *      Author: jpolin
 * Description: Class used for loading video, analyzing, and returning image of background.
 *       Usage: TODO
 */

#ifndef EXTRACTOR_HPP_
#define EXTRACTOR_HPP_

#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/video/background_segm.hpp"
#include <atomic>

#ifdef VISUALIZE
#include <unistd.h>
#endif

// TODO: Logging macros
//#define STATUS(x)
//#define WARNING(X)
//#define ERROR(x)
//#define STATUS printf

using namespace std;
using namespace cv;

class bgExtractor {

	string videoName;
	Mat backgroundImage;
	cv::VideoCapture vid;

public:

	// Default constructor
	bgExtractor();

	// Load a video, return true if found successfully
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
