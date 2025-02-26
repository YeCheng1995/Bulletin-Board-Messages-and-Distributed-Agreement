#include "../include/xml_parser.h"

pthread_once_t xml_parser::_once;
xml_parser* xml_parser::_instance = nullptr;
map<string, string> xml_parser::_parameters;

int xml_parser::read_xml(const char *path, map<string, string> &data) {
	if (!path) {
		return -1;
	}
	XMLDocument xml;
	if (XML_SUCCESS != xml.LoadFile(path)) {
		cout << "Sorry, cannot open the file..." << endl;
		return -1;
	}
	XMLElement* root = xml.RootElement();
	if (!root) {
		return -1;
	}
	
	XMLElement* thmax = root->FirstChildElement("THMAX");
	const XMLAttribute* thmaxAttr = thmax->FirstAttribute();
	cout << "thmax: " << thmaxAttr->Value() << endl;
	_parameters["thmax"] = thmaxAttr->Value();
	XMLElement* bbport = thmax->NextSiblingElement();
	const XMLAttribute* bbportAttr = bbport->FirstAttribute();
	cout << "bbport: " << bbportAttr->Value() << endl;
	_parameters["bbport"] = bbportAttr->Value();
	XMLElement* syncport = bbport->NextSiblingElement();
	const XMLAttribute* syncportAttr = syncport->FirstAttribute();
	cout << "syncport: " << syncportAttr->Value() << endl;
	_parameters["syncport"] = syncportAttr->Value();
	XMLElement* bbfile = syncport->NextSiblingElement();
	const XMLAttribute* bbfileAttr = bbfile->FirstAttribute();
	cout << "bbfile: " << bbfileAttr->Value() << endl;
	_parameters["bbfile"] = bbfileAttr->Value();


	XMLElement* peers = bbfile->NextSiblingElement();
	const XMLAttribute *peersAttr = peers->FirstAttribute();
	cout << "peers: " << peersAttr->Value() << endl;
	_parameters["peers"] = peersAttr->Value();

	// DAEMON = d
	// 	DEBUG = D
	XMLElement *daemon = peers->NextSiblingElement();
	const XMLAttribute *daemonAttr = daemon->FirstAttribute();
	cout << "daemon: " << daemonAttr->Value() << endl;
	_parameters["daemon"] = daemonAttr->Value();

	XMLElement *debug = daemon->NextSiblingElement();
	const XMLAttribute *debugAttr = debug->FirstAttribute();
	cout << "debug: " << debugAttr->Value() << endl;
	_parameters["debug"] = debugAttr->Value();

	print(_parameters);

	return 0;
}

int xml_parser::write_xml(const char *path, map<string, string> &data) {
	if (path == nullptr) {
		printf("Sorry, path is invaild...\n");
		return -1;
	}
	XMLDocument doc;
	XMLDeclaration* decl = doc.NewDeclaration();
	doc.InsertFirstChild(decl);
	// 创建根节点
	XMLElement* root = doc.NewElement("Root");
	doc.LinkEndChild(root);
	// 创建子节点
	
	XMLElement* thmax = doc.NewElement("THMAX");
	doc.LinkEndChild(thmax);
	XMLElement* bbport = doc.NewElement("BBPORT");
	doc.LinkEndChild(bbport);
	XMLElement* syncport = doc.NewElement("SYNCPORT");
	doc.LinkEndChild(syncport);
	XMLElement* bbfile = doc.NewElement("BBFILE");
	doc.LinkEndChild(bbfile);
	XMLElement* peers = doc.NewElement("PEERS");
	doc.LinkEndChild(peers);
	XMLElement* daemon = doc.NewElement("DAEMON");
	doc.LinkEndChild(daemon);
	XMLElement* debug = doc.NewElement("DEBUG");
	doc.LinkEndChild(debug);

	thmax->SetAttribute("tmax", "10");
	bbport->SetAttribute("bp", "3456");
	syncport->SetAttribute("sp", "3456");
	bbfile->SetAttribute("bbfile", "BulletinBoardFile.ims");
	peers->SetAttribute("peers", "peers");
	daemon->SetAttribute("daemon", "d");
	debug->SetAttribute("debug", "D");

	root->InsertEndChild(thmax);
	root->InsertEndChild(bbport);
	root->InsertEndChild(syncport);
	root->InsertEndChild(bbfile);
	root->InsertEndChild(peers);
	root->InsertEndChild(daemon);
	root->InsertEndChild(debug);

	doc.SaveFile(path);
	return 0;
}

void xml_parser::print(map<string, string> &data) {
	if (data.size() == 0) {
		cout << "Sorry, data is empty..." << endl;
		return;
	}
	map<string, string>::iterator it = data.begin();
	for (; it != data.end(); ++it) {
		cout << it->first << "\t" << it->second << endl;
	}
}

map<string, string>& xml_parser::get_parameters() {
	return _parameters;
}