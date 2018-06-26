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

Packet PacketCreator::inform(bool result) {
	Packet packet;
	packet.addHeader(HEADER_INFORM);
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::send(const string& file, const vector<unsigned char>& bytes) {
	Packet packet;
	packet.addHeader(HEADER_SEND);
	packet.addString(file);
	packet.addBytes(bytes);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::sendResult(bool result) {
	Packet packet;
	packet.addHeader(HEADER_SEND_RESULT);
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}