#include "PacketCreator.h"
#include "Packet.h"

using namespace std;

Packet PacketCreator::join(bool result) {
	Packet packet;
	packet.addHeader(HEADER_JOIN);
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::available(const vector<pair<size_t, string>>& connections) {
	Packet packet;
	packet.addHeader(HEADER_AVAILABLE);
	packet.addInt(connections.size());
	
	for (auto& peer : connections) {
		packet.addInt(peer.first);
		packet.addString(peer.second);
	}
	
	packet.finalize();
	
	return packet;
}