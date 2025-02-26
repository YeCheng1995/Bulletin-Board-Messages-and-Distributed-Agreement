#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "define.h"
#include "data_handler.h"

using namespace std;

class network {
public:
	void init_param (const char *ip, const short port) {
		m_sock = -1;
		m_sin.sin_family = AF_INET;
		m_sin.sin_port = htons(port);
		m_sin.sin_addr.s_addr = inet_addr(ip);

		m_client = -1;
	}

	static void init() {
		_instance = new network;
	}

	static network *get_instance () {
		pthread_once(&_once, init);
		return _instance;
	}

	void destroy_instance () {
		if (_data_instance) {
			data_handler::destroy_instance();
		}
		if (_instance) {
			delete _instance;
			_instance = nullptr;
		}
	}
private:
	network() {
		_data_instance = data_handler::get_instance();
		if (!_data_instance) {
			cout << "Sorry, create data handler instance fail." << endl;
			return;
		}
	}
	network(const network &) = default;
	network &operator=(const network &) = delete;
	~network() = default;
public:
	int create_socket ();
	int bind_ip_and_port (const char *ip, const short port);
	int listen_socket (const int backlog);

	int accept_client ();
	ssize_t read_data (char *buf, const int len);
	ssize_t write_data (const char *buf, const int len);

	void close_socket ();

	int message_to_file (const char *path, const char *buf, const int len);
	int user_info_to_file(const char *path);
	int load_user_info(const char *path, map<string, string> &users);
	static void save_message (string &path);
	static void load_message (string &path);
	static void *routine (void *args);

	static void split(string &str, vector<string> &strs, const string &pattern);
	static void set_path(string &path);
	static void print(vector<pair<long long int, string> > &data);
private:
	static network *_instance;
	static pthread_once_t _once;

	int m_sock;
	struct sockaddr_in m_sin;
	vector<int> m_socks;
	int m_client;
	struct sockaddr_in m_client_sin;

	static map<int, struct sockaddr_in> m_socket_info;
	static map<string, string> m_user_info;

	static vector<pair<long long, string> > m_message;
	static vector<pair<long long, string> > m_users;
	static long long m_message_number;

	static string m_path;
	static data_handler *_data_instance;
};

#endif
