#pragma once
#ifndef HANDLE_H
#define HANDLE_H

#include <cstddef>

class Packet;

class Handle {
public:
	void process(int fd, size_t connection_id, Packet& packet);
		
private:
	int current_fd_				= -1;
	size_t current_id_			= 0;
	Packet* current_packet_ 	= nullptr;
};

#endif