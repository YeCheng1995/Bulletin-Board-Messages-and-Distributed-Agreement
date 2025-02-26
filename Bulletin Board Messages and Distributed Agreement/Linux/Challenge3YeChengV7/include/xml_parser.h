#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <iostream>
#include <pthread.h>
#include <map>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class xml_parser {
public:
	static void init() {
		_instance = new xml_parser;
	}

	static xml_parser *get_instance () {
		pthread_once(&_once, init);
		return _instance;
	}
	void destroy_instance() {
		if (_instance) {
			delete _instance;
			_instance = nullptr;
		}
	}
public:
	xml_parser() = default;
	xml_parser(const xml_parser&) = default;
	xml_parser& operator=(const xml_parser&) = default;
	~xml_parser() = default;
public:
	static int read_xml(const char *path, map<string, string> &data);
	static int write_xml(const char *path, map<string, string> &data);
	static void print(map<string, string> &data);
	map<string, string>& get_parameters();
private:
	static pthread_once_t _once;
	static xml_parser *_instance;
	static map<string, string> _parameters;
};

#endif