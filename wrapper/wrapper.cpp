/*
 * wrapper.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 */

#include "bgExtractor.hpp"

using namespace std;

int main(int argc, char** argv){
	cout << "Loading video 1" << endl;
	bgExtractor bge;
	if (bge.loadVideoFile("../../sample_videos/ring_sample_1.mp4"))
		bge.analyzeFile();
	cout << "Loading video 2" << endl;
	bgExtractor bge2;
	bge2.loadVideoFile("dog");
	return 0;
}



