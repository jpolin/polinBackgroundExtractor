#include "server.hpp"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

//using namespace boost::filesystem;

bgExtractionServer::bgExtractionServer(const string &url, const uint port, const string &_data_path)
: http_listener(url + ":" + to_string(port))
{
	// Assign callbacks (have to use pointer to member wrapper)
	support(methods::POST, bind(&bgExtractionServer::post_cb, this, placeholders::_1));
	support(methods::PUT, bind(&bgExtractionServer::put_cb, this, placeholders::_1));
	support(methods::GET, bind(&bgExtractionServer::get_cb, this, placeholders::_1));
	support(methods::DEL, bind(&bgExtractionServer::del_cb, this, placeholders::_1));

	// Make sure no / on data path
	data_path = boost::filesystem::path(_data_path).string();

	cout<< "URL: " << uri().path() << endl;
	cout << "Data folder at " << data_path << endl;
}


void bgExtractionServer::put_cb(http_request request){
#ifdef DEBUG
	cout << "PUT REQUEST" << endl;
#endif

	// Extract data
//	const string content_type = request.headers().content_type();
//	cout << "COntent type: " << content_type << endl;
	const string file_name = request.relative_uri().path();
	const size_t lastIndex= file_name.find_last_of(".");
	string extension;
	if (lastIndex == string::npos)
		extension = "";
	else
		extension = file_name.substr(lastIndex, string::npos);

	// Check the type
	if (/*content_type != "video/mp4" || */extension != ".mp4") {
		cout << "Invalid video format " << extension << endl;
		request.reply(status_codes::NotFound, "Invalid format--mp4 only for the time being");
		return;
	}

	// Tokenize path
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

	// Write file
	const string output_file = data_path + "/" + path_elems[0] + "/" + path_elems[1];
	ofstream video_file(output_file);
	video_file << request.body();
	video_file.close();

	// Mission accomplished
	request.reply(status_codes::OK);

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

		// Create dir
		string dir_path = data_path + "/" + to_string(id);
		create_dir(dir_path.c_str());

		// Say ok
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

		// File names (path_elems[0] is port)
		string background_file =  data_path + "/" + path_elems[1] + "/" + path_elems[2];
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

void bgExtractionServer::post_cb(http_request request){
#ifdef DEBUG
	cout << "POST REQUEST -- Not implemented yet" << endl;
#endif
}

void bgExtractionServer::del_cb(http_request request){
#ifdef DEBUG
	cout << "DEL REQUEST -- Not implemented yet" << endl;
#endif
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

int main(int argc, char** argv){
	// Params
	if (argc != 3){
		cout << "Must provide 2 arguments: port and data folder path" << endl;
		return -1;
	}
	const uint port = atoi(argv[1]);
	const string data_path(argv[1]);

	bgExtractionServer server("http://localhost", port, data_path);
	server.open().then([&server](){cout<<"Starting Server\n";}).wait();
	while (true);
	return 0;
}
