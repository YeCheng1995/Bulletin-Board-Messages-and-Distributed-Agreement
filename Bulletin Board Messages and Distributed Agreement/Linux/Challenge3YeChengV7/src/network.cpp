#include "../include/network.h"

network *network::_instance = nullptr;
pthread_once_t network::_once = PTHREAD_ONCE_INIT;
map<int, struct sockaddr_in> network::m_socket_info;
map<string, string> network::m_user_info;
vector<pair<long long, string>> network::m_message;
vector<pair<long long, string>> network::m_users;
string network::m_path;
data_handler *network::_data_instance = nullptr;

long long network::m_message_number = 0;

int network::create_socket () {
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == -1) {
		cout << "sorry, create socket fail..." << endl;
		return -1;
	}
	else {
		cout << "ok, create socket successful..." << endl;
		return m_sock;
	}
}

int network::bind_ip_and_port (const char *ip, const short port) {
	//init_param(ip, port);
	int opt = 1;
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(port);
	m_sin.sin_addr.s_addr = inet_addr(ip);
	int ret = bind(m_sock, (struct sockaddr*)&m_sin, sizeof(struct sockaddr_in));
	if (ret < 0) {
		cout << "sorry, bind ip and port fail..." << endl;
		return -1;
	}
	else {
		cout << "ok, bind ip and port soccessful..." << endl;
		return ret;
	}
}

int network::listen_socket (const int backlog) {
	if (backlog <= 0) {
		cout << "sorry, parameters error..." << endl;
		return -1;
	}
	cout << "ok, listen successful." << endl;
	return listen(m_sock, backlog);
}

int network::accept_client () {
	socklen_t len = sizeof(struct sockaddr_in);
	// m_client = accept(m_sock, (struct sockaddr*)&m_client_sin, &len);
	// if (m_client < 0) {
	// 	cout << "sorry, accept client fail..." << endl;
	// 	return -1;
	// }
	// return m_client;

	while (true) {
		m_client = accept(m_sock, (struct sockaddr*)&m_client_sin, &len);
		if (m_client < 0) {
			return -1;
		}
		m_socks.push_back(m_client);

		pthread_t tid;
		pthread_create(&tid, NULL, routine, &m_client);
	}
	return 0;
}

ssize_t network::read_data (char *buf, const int len) {
	if (buf == nullptr || len <= 0) {
		cout << "sorry, parameters error..." << endl;
		return -1;
	}
	return recv(m_client, buf, len, 0);
}

ssize_t network::write_data (const char *buf, const int len) {
	if (buf == nullptr || len <= 0) {
		cout << "sorry, parameters error..." << endl;
		return -1;
	}
	cout << "send data => " << buf << endl;
	return send(m_client, buf, len, 0);
}

void network::close_socket () {
	close(m_sock);
}

int network::message_to_file (const char *path, const char *buf, const int len) {
	if (path == nullptr || buf == nullptr || len <= 0) {
		cout << "sorry, parameters error..." << endl;
		return -1;
	}
	int fd = open(path, O_RDWR | O_APPEND, 0666);
	if (fd == -1) {
		cout << "sorry, cannot open the file..." << endl;
		return -1;
	}

	write(fd, buf, len);
	close(fd);
	return 0;
}

void *network::routine (void *args) {
	int fd = *(int*)args;
// #if _DEBUG
	cout << "fd: " << fd << endl;
	cout << "path: " << m_path << endl;
// #endif
	// m_users.clear();
	// m_message.clear();

	// load data from file
	// string filename = m_path;
	_data_instance->load_data(m_path);
	m_users = _data_instance->get_users();
	m_message = _data_instance->get_message();
	m_message_number = m_users[m_users.size()-1].first;

	cout << "message_number: " << m_message_number << endl;
	print(m_message);

	char recv_buff[256] = {};
	char send_buff[256] = {};
	char str1[80] = {};
	char str2[80] = {};
	char message[256] = {};
	string name;

	string recv_data, send_data;
	vector<string> strs;

	while (1) {
		ssize_t recv_size = recv(fd, recv_buff, sizeof(recv_buff), 0);
		if (recv_size <= 0) {
			break;
		}
		recv_data = recv_buff;
		cout << "recv: " << recv_data << endl;
		
		if (recv_data.find("USER") != string::npos) {
			strs.clear();
			split(recv_data, strs, " ");
			name = strs[1];
			cout << "USER." << endl;
			send_data.clear();
			send_data += "HELLO_";
			send_data += strs[1];
			send_data += "_welcome";

			strncpy(message, send_data.c_str(), send_data.size()+1);
			send(fd, message, sizeof(message), 0);
		}
		else if (recv_data.find("READ") != string::npos) {
			cout << "=======================" << endl;
			print(m_message);
			print(m_users);

			strs.clear();
			split(recv_data, strs, " ");
			cout << "READ: " << strs[1] << endl;
			send_data.clear();
			long long int no = stoll(strs[1]);
			cout << "no: " << no << endl;
			send_data += "MESSAGE_";
			bool flag = true;
			string msg, this_message, this_name;
			vector<pair<long long int, string>>::iterator it = m_users.begin();
			for (; it != m_users.end(); ++it) {
				if (it->first == no) {
					send_data += strs[1];
					send_data += "_";
					send_data += it->second;

					break;
				}

			}
			auto it1 = m_message.begin();
			for (; it1 != m_message.end(); ++it1) {
				if (it1->first == no) {
					send_data += "/";
					send_data += it1->second;
					msg = it1->second;
					break;
				}
			}
			

			cout << "read: " << send_data << endl;

			if (it == m_users.end()) {
				send_data = "UNKNOWN_";
				send_data += strs[1];
				send_data += "_not_found.";
			}
			if (msg.length() == 0) {
				send_data = "ERROR_READ_not_found.";

			}
			cout << send_data << endl;
			strncpy(send_buff, send_data.c_str(), send_data.size() + 1);
			send(fd, send_buff, sizeof(send_buff), 0);
		}

		else if (recv_data.find("WRITE") != string::npos) {
			m_message_number++;
			cout << "m_message_number: " << m_message_number << endl;
			strs.clear();
			split(recv_data, strs, " ");
			cout << "WRITE." << endl;
			send_data.clear();
			string line;
			if (name.size() > 0) {
				// m_message_number++;
				pair<long long int, string> p;
				p.first = m_message_number;
				p.second = strs[1];
				line += to_string(p.first);


				pair<long long int, string> puser;
				puser.first = m_message_number;
				if (name.length() > 0) {
					puser.second = name;
				}
				else {
					puser.second = " ";
				}
				line += "/";
				line += puser.second;
				line += "/";
				line += p.second;
				line += "\n";
				m_users.push_back(puser);

				m_message.push_back(p);

				// write file
				fstream osm;
				osm.open(m_path, ios::out | ios::app);
				if (!osm.is_open()) {
					cout << "Sorry, cannot open the file." << endl;
					return nullptr;
				}
				cout <<"line: " <<line << endl;
				osm << line;
				osm.flush();
				osm.close();

				send_data = "WROTE done   .";
                send_data += to_string(m_message_number);

				print(m_users);
				print(m_message);
				strncpy(send_buff, send_data.c_str(), send_data.size() + 1);
				send(fd, send_buff, sizeof(send_buff), 0);
			}
			else {
				send_data = "ERROR WRITE sorry.";
				strncpy(send_buff, send_data.c_str(), send_data.size() + 1);
				send(fd, send_buff, sizeof(send_buff), 0);
			}
			
		}
		else if (recv_data.find("REPLACE") != string::npos) {
			strs.clear();
			split(recv_data, strs, " ");
			cout << "REPLACE: " << recv_data << endl;
			send_data.clear();
			vector<string> res;
			split(strs[1], res, "/");
			cout << "this is a test." << endl;
			cout << "=> " << res[0] << endl;
			long long int no = stoll(res[0]);
			vector<pair<long long int, string>>::iterator it = m_message.begin();
			for (; it != m_message.end(); ++it) {
				if (it->first == no) {
					it->second = res[1];
					break;
				}
			}
			if (it == m_message.end()) {
				send_data = "UNKONWN_";
				send_data += res[0];

			}
			else {
				send_data = "REPLACE_ok.";
			}
			_data_instance->set_users(m_users);
			_data_instance->set_messages(m_message);
			_data_instance->write_data(m_path);

			strncpy(send_buff, send_data.c_str(), send_data.size() + 1);
			send(fd, send_buff, sizeof(send_buff), 0);
		}
		else if (recv_data.find("QUIT") != string::npos) {
			cout << "QUIT." << endl;
			send_data.clear();
			send_data = "Bye.";

			strncpy(send_buff, send_data.c_str(), send_data.size() + 1);
			send(fd, send_buff, sizeof(send_buff), 0);

			// save_message(m_path);
			// _data_instance->set_users(m_users);
			// _data_instance->set_messages(m_message);
			// _data_instance->write_data(m_path);
			cout << "ok, client " << fd << " exited." << endl;
			return nullptr;
		}
		else {
			continue;
		}


		sleep(1);
	}

	return nullptr;
}

int network::user_info_to_file(const char *path) {
	return 0;
}

int network::load_user_info(const char *path, map<string, string> &users) {
	return 0;
}

void network::split(string &str, vector<string> &strs, const string &pattern)
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

void network::save_message (string &path) {
	if (path.length() == 0) {
		return;
	}
	fstream osm;
	osm.open(path, ios::out);
	if (!osm.is_open()) {
		printf("Sorry, cannot open the file...\n");
		return ;
	}
	vector<pair<long long, string>>::iterator it = m_users.begin();
	for (; it != m_users.end(); ++it) {
		osm << it->first << " " << it->second << "/";
		auto it1 = m_message.begin();
		for (; it1 != m_message.end(); ++it1) {
			if (it->first == it1->first) {
				osm << it1->second << "\n";
			}
		}
	}
	osm.close();
}

void network::load_message (string &path) {
	if (path.length() == 0) {
		return;
	}
	fstream osm;
	osm.open(path, ios::in);
	if (!osm.is_open()) {
		printf("Sorry, cannot open the file...\n");
		return;
	}

	// do something

	osm.close();
}

void network::set_path(string &path) {
	m_path = path;
}

void network::print(vector<pair<long long int, string>> &data) {
	if (data.size() == 0) {
		cout << "Sorry, no data." << endl;
		return;
	}
	vector<pair<long long int, string>>::iterator it = data.begin();
	for (; it != data.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
}
