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
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/video.hpp"

// Define if you want to visualize
#define VISUALIZE

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
	Mat *backgroundImage = NULL;
	cv::VideoCapture vid;

public:
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

	/* Return a pointer to the current background image. If it returns NULL,
	 * read errorMessage for more info.
	 */
	Mat *getBackground() const;

	// Populated with error message by any faulting method
	string errorMessage;

};


#endif /* EXTRACTOR_HPP_ */
