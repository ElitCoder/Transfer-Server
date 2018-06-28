#pragma once
#ifndef HANDLE_H
#define HANDLE_H

#include <cstddef>
#include <string>
#include <vector>
#include <mutex>
#include <list>

enum {
	ERROR_OLD_PROTOCOL
};

class Packet;

class Handle {
public:
	void process(int fd, size_t connection_id, Packet& packet);
	void disconnect(size_t id);
		
private:
	void handleJoin();
	void handleAvailable();
	void handleInform();
	void handleSend();
	void handleSendResult();
	void handleInitialize();
	
	bool exists(const std::string& name);
	size_t getID(const std::string& name);
	
	void processDisconnects();
	
	int fd_			= -1;
	size_t id_		= 0;
	Packet* packet_ = nullptr;
	
	std::vector<std::pair<size_t, std::string>> connections_;
	
	std::mutex disconnect_mutex_;
	std::list<size_t> disconnect_queue_;
};

void disconnectFunction(int fd, size_t id);

#endif