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

using namespace std;

void split(string &str, vector<string> &strs, const string &pattern)
{
    if (str.empty()) {
    	return;
    }
    std::string::size_type lastPos = str.find_first_not_of(pattern, 0);
    std::string::size_type pos = str.find_first_of(pattern, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        strs.__emplace_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(pattern, pos);
        pos = str.find_first_of(pattern, lastPos);
    }
}

int main () {

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(3456);
	_sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	// char buf[256] = {"Hello, this is client...\n"};
	// char recv_buf[256] = {};
	if (connect(sock, (struct sockaddr*)&_sin, sizeof(struct sockaddr_in)) < 0) {
		cout << "sorry." << endl;
		return -1;
	}

	char send_buff[256] = {"USER"};
	char recv_buff[256] = {};

	string send_data;

	while (1) {
		printf("Hi Please input your data: ");
		getline(cin, send_data);
		strncpy(send_buff, send_data.c_str(), send_data.size()+1);
		send(sock, send_buff, sizeof(send_buff), 0);

		recv(sock, recv_buff, sizeof(recv_buff), 0);
		printf("recv: %s\n", recv_buff);
		if (strstr(recv_buff, "Bye")) {
			cout << "Bye." << endl;
			break;
		}
		sleep(1);
	}

	close(sock);
	return 0;
}
