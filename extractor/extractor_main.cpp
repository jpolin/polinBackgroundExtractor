/*
 * wrapper.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 * Description: Simple app that exercises the bgExtractor library
 *       Usage:
 *              ./extractor_main input_file.mp4 output_file.jpg
 */

#include "bgExtractor.hpp"
#include <thread>
#include <unistd.h>


using namespace std;

void printHelp(){
	cout << "Usage" << endl << endl;
	cout << "    ./bgExtractorWrapper input_video.mp4 output_path.jpg" << endl;
	cout << endl;
}

// Run in separate thread
void printExtractionProgress(bgExtractor &extractor){
	unsigned progress;
	cout << endl;
	while (progress < 100){
		progress = (unsigned)extractor.progress;
		cout << "\xdProgress: " << progress << flush;
		sleep(1);
		// See if we've frozen
		if (progress == extractor.progress)	break;
	}
	cout << endl;
}

int main(int argc, char** argv){

	// Expect two arguments (not including executable)
	if (argc != 3) {
		printHelp();
		return 1;
	}

	const string inputFile(argv[1]);
	const string outputFile(argv[2]);

	// Load video file
	bgExtractor extractor;
	if (!extractor.loadVideoFile(inputFile)) {
		cout << "Extraction Failed: " << extractor.errorMessage << endl;
		return 2;
	}

	// Monitor progress of analysis
	thread progressThread(printExtractionProgress, ref(extractor));

	// Analyze file
	bool visualize_output = true;
	if (!extractor.analyzeFile(visualize_output)){
		cout << "File analysis failed: " << extractor.errorMessage << endl;
		progressThread.join();
		return 3;
	}

	// Write background to file
	if (!extractor.writeBackgroundToFile(outputFile)){
		cout << "Saving background failed: " << extractor.errorMessage << endl;
		return 4;
	}

	cout << inputFile << " was successfully analyzed. Background written to ";
	cout << outputFile << endl;

	progressThread.join();

	return 0;
}



