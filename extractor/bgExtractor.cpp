/*
 * extractor.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 */

#include "bgExtractor.hpp"

using namespace std;
using namespace cv;

bgExtractor::bgExtractor() : errorMessage("No error"), progress(0) {}

const bool bgExtractor::loadVideoFile(const string &filename){
	// Try opening file
	vid.open(filename);

	progress = 0;

	// Make sure it succeeded
	if (vid.isOpened()){
		errorMessage = "No error";
		return true;
	}
	else {
		errorMessage = "Could not open " + filename;
		return false;
	}
}

const bool bgExtractor::analyzeFile(){
	// Make sure it's open
	if (!vid.isOpened()){
		errorMessage = "Video not opened";
		return false;
	}

	// Only look at a fixed number of evenly spaced frame (at most)
	size_t frameBufferSize = MAXBUFFERFRAMES;
	const size_t frameCount = vid.get(CV_CAP_PROP_FRAME_COUNT);
	const size_t skipFrames = frameCount / frameBufferSize;
	// Since the skipFrames operation rounds down, get more accurate estimate of buffer
	frameBufferSize = frameCount / (1 + skipFrames);

	// Use mixture of Gaussians
	BackgroundSubtractorMOG2 bgSubtractor(
			/*int history*/ frameBufferSize,
			/*float varThreshold*/ 0.1,
			/*bool bShadowDetection=true*/ true);

	// Iterate through frames
	Mat frame, mask;
	size_t skipFrameIndex = 0;
	size_t frameIndex = 0;
	while (vid.read(frame)){
		// Only run when frame_skip says so (=0)
		if (!skipFrameIndex) {
			// Run extractor
			bgSubtractor(frame, mask);
			// Update progress
			progress = (uint)(100 * float(frameIndex)/frameCount);
		}

		frameIndex++;

		// Reset skipFrames sometimes
		if (skipFrameIndex++ == skipFrames) skipFrameIndex = 0;

	}

	// Update class member
	bgSubtractor.getBackgroundImage(backgroundImage);

	errorMessage = "No error";
	return true;
}

const bool bgExtractor::writeBackgroundToFile(const string& filename) {
	// Make sure we have an image
	if (backgroundImage.empty()){
		errorMessage = "No background image; please run analyzeFile() first";
		return false;
	}

	bool s = imwrite(filename, backgroundImage);
	errorMessage = "No error";
	return true;
}



