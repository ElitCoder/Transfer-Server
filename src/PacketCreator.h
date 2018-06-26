#pragma once
#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

#include <vector>
#include <string>

enum {
	HEADER_JOIN,
	HEADER_AVAILABLE
};

class Packet;

class PacketCreator {
public:
	static Packet join(bool result);
	static Packet available(const std::vector<std::pair<size_t, std::string>>& connections);
};

#endif