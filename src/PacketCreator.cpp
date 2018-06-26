#include "PacketCreator.h"
#include "Packet.h"

Packet PacketCreator::join(bool result) {
	Packet packet;
	packet.addHeader(HEADER_JOIN);
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}