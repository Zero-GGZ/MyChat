#ifndef API_JSON_H
#define API_JSON_H


#include <iostream>
#include <string>
#include <json/json.h>
using namespace std;

Json::Value read_json_from_string(string str);
string write_json_to_string(Json::Value root);

#endif

