#include "server.hpp"

using namespace std;
using namespace web::http;
using namespace web::http::experimental::listener;

void _post_cb(http_request req){
	cout << "POST REQUEST" << endl;
}

bgExtractionServer::bgExtractionServer(const string &urlPort) : http_listener(urlPort){
	// Assign callbacks
//	METHOD _post_cb = &post_cb;
//	support(methods::POST, this.*_post_cb);
	support(methods::POST, _post_cb);
//	support(methods::PUT, put_cb);
//	support(methods::GET, get_cb);
//	support(methods::DEL, del_cb);
}


void bgExtractionServer::post_cb(http_request request){};
void bgExtractionServer::get_cb(http_request request){};
void bgExtractionServer::put_cb(http_request request){};
void bgExtractionServer::del_cb(http_request request){};

int main(int argc, char** argv){
	bgExtractionServer server("http://localhost:9999");
	server.open();
	while (true);
	return 0;
}
