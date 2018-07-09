#pragma once
#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

#include <vector>
#include <string>

enum {
	HEADER_JOIN,
	HEADER_AVAILABLE,
	HEADER_INFORM,
	HEADER_SEND,
	HEADER_SEND_RESULT,
	HEADER_INITIALIZE,
	HEADER_INFORM_RESULT,
	HEADER_CLIENT_DISCONNECT
};

class Packet;

class PacketCreator {
public:
	static Packet join(bool result);
	static Packet available(const std::vector<std::pair<size_t, std::string>>& connections);
	static Packet inform(bool result, bool try_direct = false, int port = 0, const std::vector<std::string>& receiving_addresses = {});
	static Packet informResult(int id, const std::string& file, const std::string& directory, bool direct_possible);
	static Packet send(int id, const std::string& file, const std::string& directory, const std::pair<size_t, const unsigned char*>& bytes, bool result);
	static Packet sendResult(bool result);
	static Packet initialize(bool result, int code, const std::string& client_linux, const std::string& update_linux, const std::string& client_windows);
	static Packet disconnect(int id);
};

#endif