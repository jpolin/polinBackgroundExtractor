#include "server.hpp"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

bgExtractionServer::bgExtractionServer(const string &urlPort) : http_listener(urlPort){
	// Assign callbacks (have to use pointer to member wrapper)
	support(methods::POST, bind(&bgExtractionServer::post_cb, this, placeholders::_1));
	support(methods::PUT, bind(&bgExtractionServer::put_cb, this, placeholders::_1));
	support(methods::GET, bind(&bgExtractionServer::get_cb, this, placeholders::_1));
	support(methods::DEL, bind(&bgExtractionServer::del_cb, this, placeholders::_1));
}


void bgExtractionServer::post_cb(http_request request){
#ifdef DEBUG
	cout << "POST REQUEST" << endl;
#endif
	pplx::task<web::json::value> val = request.extract_json();

}

void bgExtractionServer::get_cb(http_request request){
#ifdef DEBUG
	cout << "GET REQUEST" << endl;
#endif

	// Find out what client wants
	const utility::string_t path = request.relative_uri().path();

	// Split paths into tokens
	boost::char_separator<char> delim("/");
	boost::tokenizer<boost::char_separator<char>> path_tokens(path, delim);
	vector<string> path_elems;
	for (auto &t : path_tokens) path_elems.push_back(t);

	// Empty
	if (path_elems.empty() || path_elems.size() > 2) {
		request.reply(status_codes::NotFound, "Invalid request");
		return;
	}

	// Requesting new ID
	if (path_elems.size() == 1 && path_elems[0] == "new_id"){
		const uint id = getNewID();
#ifdef DEBUG
		cout << "Requesting new ID, giving " << id << endl;
#endif
		json::value response;
		response["new_id"] = id;
		request.reply(status_codes::OK, response);
		return;
	}

	// Single element that wasn't new_id
	else if (path_elems.size() == 1){
		request.reply(status_codes::NotFound, "Invalid request");
		return;
	}

	// Trying to get background (path = /id_num/video_name.mp4)
	else {

		// File names
		string background_file = "data/" + path_elems[0] + "/" + path_elems[1];
		const size_t lastIndex= background_file.find_last_of(".");
		string video_name;
		if (lastIndex == string::npos){
			// Default to jpeg
			video_name = background_file;
			background_file += ".jpg";
		}
		else {
			video_name = background_file.substr(0, lastIndex);
		}
		// Add extension
		video_name += ".mp4";

#ifdef DEBUG
		cout << "Requesting file " << path_elems[0] << " " << path_elems[1] << endl;
#endif
		// Check if file is already in there
		string background_data;
		if (loadFileToString(background_file, background_data)){
			cout << "File already exists; returning it" << endl;
			request.reply(status_codes::OK, background_data, "image/jpeg");
			return;
		}

		// Load file into extractor
		bgExtractor extractor;
		if (!extractor.loadVideoFile(video_name)) {
			cout << "Extraction Failed: " << extractor.errorMessage << endl;
			request.reply(status_codes::NotFound, extractor.errorMessage);
			return;
		}

		// Analyze file
		if (!extractor.analyzeFile()){
			cout << "File analysis failed: " << extractor.errorMessage << endl;
			request.reply(status_codes::NotFound, extractor.errorMessage);
			return;
		}

		// Write background to file
		if (!extractor.writeBackgroundToFile(background_file)){
			cout << "Saving background failed: " << extractor.errorMessage << endl;
			request.reply(status_codes::NotFound, extractor.errorMessage);
			return;
		}

		// Send file as response
		loadFileToString(background_file, background_data);
		request.reply(status_codes::OK, background_data, "image/jpeg");

	}

}

void bgExtractionServer::put_cb(http_request request){
#ifdef DEBUG
	cout << "PUT REQUEST" << endl;
#endif
}

void bgExtractionServer::del_cb(http_request request){
#ifdef DEBUG
	cout << "DEL REQUEST" << endl;
#endif
}

// Manage ID's:
const uint bgExtractionServer::getNewID(){
	return 5;
}

const bool bgExtractionServer::loadFileToString(const string &filename, string &buf){
	// Copy to string
	ifstream inputFile(filename);
	if (!inputFile.is_open()) return false;
	stringstream ss;
	ss << inputFile.rdbuf();
	buf = ss.str();
	return true;
}

int main(int argc, char** argv){
	bgExtractionServer server("http://localhost:5007");
	server.open().then([&server](){cout<<"Starting Server\n";}).wait();
	while (true);
	return 0;
}
