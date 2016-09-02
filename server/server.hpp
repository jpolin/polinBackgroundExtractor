/*
 * server.hpp
 *
 *  Created on: Aug 29, 2016
 *      Author: Joe Polin
 * Description: Present REST interface for extracting background from videos
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_


// Background extraction
#include "bgExtractor.hpp"

// Server
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "pplx/pplxtasks.h"
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <sstream>

using namespace web::http;
using namespace web::http::experimental::listener;

// Creating dir (modified from jch's suggestion on Stack Overflow)
#ifdef WIN32
#include <direct.h>
#define create_dir(filename) _mkdir(filename)
#else
#include <sys/stat.h>
#define create_dir(filename) mkdir(filename, 0755)
#endif

class bgExtractionServer : public http_listener {

public:

	bgExtractionServer(const string &url, const uint port, const string &data_path);

	// Puts contents of filename into string buf
	static const bool loadFileToString(const string &filename, string &buf);


protected:

	// Callbacks
	void post_cb(http_request request);

	// Puts video file that needs extraction
	void put_cb(http_request request);

	/* Gets:
	 * - ID for video
	 * - Progress on video
	 * - Final image
	 */
	void get_cb(http_request request);

	// Removes video/image on server
	void del_cb(http_request request);

	// Returns new, unique ID that no other client is using
	const uint getNewID();

	// Path to data folder
	string data_path;


};

// Treat member functions as function pointers
typedef void(bgExtractionServer::*METHOD)(http_request);


#endif /* SERVER_HPP_ */
