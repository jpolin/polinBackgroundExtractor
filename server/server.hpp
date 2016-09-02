/*
 * server.hpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 * Description: Present REST interface for extracting background from videos
 *       Usage: (see server_main.cpp)
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_


// Background extraction
#include "bgExtractor.hpp"

// Server
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "pplx/pplxtasks.h"
#include <sys/stat.h>

// Parsing and moving data around
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <sstream>

// Namespaces in cpprestsdk
using namespace web::http;
using namespace web::http::experimental::listener;

class bgExtractionServer : public http_listener {

public:

	// Common construction: bgExtractionServer server("http://localhost", 9090, "data/")
	bgExtractionServer(const string &url, const uint port, const string &data_path);

	// Puts contents of filename into string buf
	static const bool loadFileToString(const string &filename, string &buf);


protected:

	// User uploads video file
	void post_cb(http_request request);


	// Get ID for video/process
	void get_cb(http_request request);

	// Returns new, unique ID that no other client is using
	const uint getNewID();

	// Path to data folder
	string data_path;


};

#endif /* SERVER_HPP_ */
