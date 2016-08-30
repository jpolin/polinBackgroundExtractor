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

// TODO: Logging macros
//#define STATUS(x)
//#define WARNING(X)
//#define ERROR(x)
//#define STATUS printf

using namespace std;

class bgExtractor {

	string videoName;
	string errorMessage;

public:
	const bool loadVideoFile(const string &filename);
	const bool analyzeFile();
//	 getBackground() const;


};


#endif /* EXTRACTOR_HPP_ */
