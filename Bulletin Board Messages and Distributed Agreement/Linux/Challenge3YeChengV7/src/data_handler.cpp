#include "../include/data_handler.h"

data_handler *data_handler::_instance = nullptr;
pthread_once_t data_handler::_once;
string data_handler::_path;
vector<pair<long long int, string>> data_handler::_users;
vector<pair<long long int, string>> data_handler::_messages;


int data_handler::load_data(string &path) {
	if (path.empty() ) {
		return -1;
	}
	fstream osm;
	osm.open(path, ios::in);
	if (!osm.is_open()) {
		cout << "Sorry, cannot open the file." << endl;
		return -1;
	}
	cout << "path: " << path << endl;
	_users.clear();
	_messages.clear();
	string line;
	vector<string> strs;
	
	while (!osm.eof()) {
		strs.clear();
		getline(osm, line);
		if (line.length() == 0) {
			break;
		}
		cout << line << endl;
		pair<long long int, string> p;
		split(line, strs, "/");
		p.first = stoll(strs[0]);
		p.second = strs[1];

		// cout << p.first << " " << p.second << endl;

		_users.push_back(p);
		p.second = strs[2];
		// cout << p.first << " " << p.second << endl;
		_messages.push_back(p);
		
	}
	// cout << "==========================" << endl;
	// vector<pair<long long int, string>>::iterator it = _users.begin();
	// for (; it != _users.end(); ++it) {
	// 	cout << it->first << " " << it->second << endl;
	// }
	// _users.erase(end(_users) - 1);
	// _messages.erase(end(_messages) - 1);
	cout << "Ok, load done." << endl;
	osm.close();
	return 0;
}

int data_handler::write_data(string &path) {
	if (path.empty() ) {
		return -1;
	}
	fstream osm;
	osm.open(path, ios::out);
	if (!osm.is_open()) {
		cout << "Sorry, cannot open the file." << endl;
		return -1;
	}
	string line;
	vector<pair<long long int, string>>::iterator it = _users.begin();
	for (; it != _users.end(); ++it) {
		line += to_string(it->first);
		line += "/";
		line += it->second;

		line += "/";
		auto it1 = _messages.begin();
		for (; it1 != _messages.end(); ++it1) {
			if (it1->first == it->first) {
				line += it1->second;
				break;
			}
		}
		if (it1 == _messages.end()) {
			line += " ";
		}
		line += "\n";

		osm << line;
		line.clear();
	}
	cout << "Ok, write done." << endl;
	osm.close();
	return 0;
}

void data_handler::split(string &str, vector<string> &strs, const string &pattern)
{
    if (str.empty()) {
    	return;
    }
    std::string::size_type lastPos = str.find_first_not_of(pattern, 0);
    std::string::size_type pos = str.find_first_of(pattern, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        strs.emplace_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(pattern, pos);
        pos = str.find_first_of(pattern, lastPos);
    }
}