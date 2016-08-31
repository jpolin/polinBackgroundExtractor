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

using namespace web::http;
using namespace web::http::experimental::listener;



class bgExtractionServer : public http_listener {

public:
	// Provide url:port (eg http://localhost:9999)
	bgExtractionServer(const string &urlPort);// : http_listener(urlPort);

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


};

// Treat member functions as function pointers
typedef void(bgExtractionServer::*METHOD)(http_request);


#endif /* SERVER_HPP_ */
