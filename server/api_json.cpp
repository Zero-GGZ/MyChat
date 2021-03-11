#include "api_json.h"
using namespace std;


Json::Value read_json_from_string(string str){
    Json::Reader reader;
    Json::Value root;
	cout <<str<<endl;
	if(!reader.parse(str,root)){
        std::cout <<"read json from string erro"<<endl;
    }
    return root;
}

string write_json_to_string(Json::Value root){
    Json::FastWriter writer;
    return writer.write(root);
}
