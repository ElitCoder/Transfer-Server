#pragma once
#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

enum {
	HEADER_JOIN
};

class Packet;

class PacketCreator {
public:
	static Packet join(bool result);
};

#endif