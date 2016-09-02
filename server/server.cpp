#include "server.hpp"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

bgExtractionServer::bgExtractionServer(const string &url, const uint port, const string &_data_path)
: http_listener(url + ":" + to_string(port))
{
	// Assign callbacks (have to use pointer to member wrapper)
	support(methods::PUT, bind(&bgExtractionServer::put_cb, this, placeholders::_1));
	support(methods::GET, bind(&bgExtractionServer::get_cb, this, placeholders::_1));
	support(methods::OPTIONS, bind(&bgExtractionServer::opt_cb, this, placeholders::_1));

	// Make sure no / on data path
	data_path = boost::filesystem::path(_data_path).string();

	// Server status
	cout << "Server port: " << port << endl;
	cout << "Data folder at ./" << data_path << endl;
	cout << "Starting server (ctrl+C to stop)" << endl;
}

// A 'PUT' request from outside of domain gets delivered as opt. For now, just pass through.
void bgExtractionServer::opt_cb(http_request request){
	cout << "OPTIONS Request" << endl;
	put_cb(request);
}

// Request from client to upload mp4 file
void bgExtractionServer::put_cb(http_request request){
	cout << "PUT REQUEST" << endl;

	// Testing asynchrosity
//	cout << "Going to sleep..." << flush;
//	sleep(30);
//	cout << "Awake!" << endl;

	// Get file root name and extension
	const string file_name = request.relative_uri().path();
	const size_t lastIndex= file_name.find_last_of(".");
	const string file_root_name = file_name.substr(0, lastIndex);
	string extension;
	if (lastIndex == string::npos)
		extension = "";
	else
		extension = file_name.substr(lastIndex, string::npos);

	// Check the type (TODO: Support more formats)
	if (extension != ".mp4") {
		cout << "Invalid video format " << extension << endl;
		request.reply(status_codes::NotFound, "Invalid format--mp4 only for the time being");
		return;
	}

	// Tokenize path (should be ID and video name)
	boost::char_separator<char> delim("/");
	boost::tokenizer<boost::char_separator<char>> path_tokens(file_name, delim);
	vector<string> path_elems;
	for (auto &t : path_tokens) path_elems.push_back(t);

	// Should be 2 items in path
	if (path_elems.size() != 2){
		cout << "Invalid path: " << file_name << endl;
		request.reply(status_codes::NotFound, "Can only post /id_num/file_name.mp4");
		return;
	}

	// Write data to file (TODO: Make sure dir exists)
	const string output_file = data_path + "/" + path_elems[0] + "/" + path_elems[1];
	ofstream video_file(output_file);
	video_file << request.body();
	video_file.close();

	// Perform extraction on video file
	bgExtractor extractor;
	if (!extractor.loadVideoFile(output_file)) {
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
	if (!extractor.writeBackgroundToFile(file_root_name + ".jpg")){
		cout << "Saving background failed: " << extractor.errorMessage << endl;
		request.reply(status_codes::NotFound, extractor.errorMessage);
		return;
	}

	// Mission accomplished--make sure client knows it is allowed to talk to us
	http_response response (status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	request.reply(response);

}

// Get either ID or background
void bgExtractionServer::get_cb(http_request request){
	cout << "GET Request" << endl;

	// Want to let client know it's ok to talk to us
	http_response response (status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));

	// Tokenize path
	const utility::string_t path = request.relative_uri().path();
	boost::char_separator<char> delim("/");
	boost::tokenizer<boost::char_separator<char>> path_tokens(path, delim);
	vector<string> path_elems;
	for (auto &t : path_tokens) path_elems.push_back(t);

	// Empty--no good
	if (path_elems.empty() || path_elems.size() > 2) {
		request.reply(status_codes::NotFound, "Invalid request");
		return;
	}

	// Requesting new ID
	if (path_elems.size() == 1 && path_elems[0] == "new_id"){
		const uint id = getNewID();
		json::value response_json;
		response_json["new_id"] = id;

		// Create directory for ID
		string dir_path = data_path + "/" + to_string(id);
		mkdir(dir_path.c_str(), 0750); // Linux-specific, I believe

		// Say ok
		response.set_body(response_json);
		request.reply(response);
		return;
	}

	// Single element that wasn't new_id
	else if (path_elems.size() == 1){
		request.reply(status_codes::NotFound, "Invalid request");
		return;
	}

	// Asking for background
	else {

		// Background image name
		string background_file =  data_path + "/" + path_elems[0] + "/" + path_elems[1];

		// Send it back
		string background_data;
		if (loadFileToString(background_file, background_data)){
			request.reply(status_codes::OK, background_data, "image/jpeg");
			return;
		}
		else {
			request.reply(status_codes::NotFound, "File does not exist");
			return;
		}

	}

}


// Manage ID's:
const uint bgExtractionServer::getNewID(){

	/* Dummy solution
	 * (Would be better to keep track of id's used before server started, and of ones deleted)
	 */
	static uint id = 1;
	return id++;

}

const bool bgExtractionServer::loadFileToString(const string &filename, string &buf){
	// Copy to string
	ifstream inputFile(filename);
	if (!inputFile.is_open()) return false;
	stringstream ss;
	ss << inputFile.rdbuf();
	buf = ss.str();
	inputFile.close();
	return true;
}


