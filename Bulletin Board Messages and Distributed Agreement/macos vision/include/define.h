#ifndef __DEFINE_H__
#define __DEFINE_H__

enum CMD {
	CMD_SEND,
	CMD_RECV,
};

struct Message {
	int size;
	char buf[256];
};

#endif