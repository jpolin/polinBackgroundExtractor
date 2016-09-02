/*
 * server_main.cpp
 *
 *  Created on: Sep 2, 2016
 *      Author: Joe Polin
 * Description: Launch server
 *       Usage: ./backgroundExtractionServer <port> <path_to_data_folder>
 */

#include "server.hpp"
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char** argv){

	// Params
	if (argc != 3){
		cout << "Must provide 2 arguments: port and data folder path" << endl;
		return -1;
	}
	const uint port = atoi(argv[1]);
	const string data_path(argv[2]);

	bgExtractionServer server("http://localhost", port, data_path);
	server.open().then([&server](){}).wait();
	while (true);
	return 0;
}



