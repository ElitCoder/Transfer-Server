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
	HEADER_SEND_RESULT
};

class Packet;

class PacketCreator {
public:
	static Packet join(bool result);
	static Packet available(const std::vector<std::pair<size_t, std::string>>& connections);
	static Packet inform(bool result);
	static Packet send(const std::string& file, const std::vector<unsigned char>& bytes);
	static Packet sendResult(bool result);
};

#endif