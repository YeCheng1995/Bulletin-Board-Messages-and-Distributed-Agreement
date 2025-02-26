#include "./include/network.h"
#include "./include/xml_parser.h"

void handler (map<string, string> &data) {
	network *_instance = network::get_instance();
	_instance->create_socket();
	const char *ip = "127.0.0.1";
	// cout << "bbport: " << data["bbport"] << endl;
	string path = "./db/";
	path += data["bbfile"];
	short port = stoi(data["bbport"]);
	_instance->set_path(path);
	_instance->bind_ip_and_port(ip, port);
	_instance->listen_socket(128);
	_instance->accept_client();
	char buf[128] = {};
	// _instance->read_data(buf, sizeof(buf));
	// const char *path = data["bbfile"].c_str();
	// cout << "recv <= " << buf << endl;
	// _instance->message_to_file(path, buf, sizeof(buf));
	
	// _instance->save_message(path);
	_instance->close_socket();
}

void conf_file() {
	const char *path = "./conf/conf.xml";
	xml_parser *_instance = xml_parser::get_instance();
	map<string, string> data;
	_instance->read_xml(path, data);
	// _instance->print(data);
	map<string, string> paras = _instance->get_parameters();

	// network
	handler(paras);
}

int main () {
	conf_file();
	return 0;
}