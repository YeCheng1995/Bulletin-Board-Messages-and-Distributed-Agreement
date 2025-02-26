#ifndef __DATA_HANDLER__
#define __DATA_HANDLER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class data_handler {
public:
	static void init() {
		_instance = new data_handler;
	}

	static data_handler *get_instance () {
		pthread_once(&_once, init);
		return _instance;
	}

	static void destroy_instance () {
		if (_instance) {
			delete _instance;
			_instance = nullptr;
		}
	}
public:
	void split(string &str, vector<string> &strs, const string &pattern);
	int load_data(string &path);
	int write_data(string &path);
	vector<pair<long long int, string> >& get_users() {
		return _users;
	}

	vector<pair<long long int, string> >& get_message() {
		return _messages;
	}

	void set_path(string &path) {
		_path = path;
	}

	string &get_path() {
		return _path;
	};

	void set_users(vector<pair<long long int, string> > &data) {
		_users = data;
	}

	void set_messages(vector<pair<long long int, string> > &data) {
		_messages = data;
	}
private:
	data_handler() {
		
	}

	data_handler (const data_handler &)=delete;
	data_handler &operator= (const data_handler &) = delete;
	~data_handler () = default;
private:
	static data_handler *_instance;
	static pthread_once_t _once;
	static vector<pair<long long int, string> > _users;
	static vector<pair<long long int, string> > _messages;
	static string _path;
};

#endif
