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

Packet PacketCreator::send(int id, const string& file, const string& directory, const std::pair<size_t, const unsigned char*>& bytes, bool result) {
	Packet packet;
	packet.addHeader(HEADER_SEND);
	packet.addInt(id);
	packet.addString(file);
	packet.addString(directory);
	packet.addBytes(bytes);
	packet.addBool(result);
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

Packet PacketCreator::initialize(bool result) {
	Packet packet;
	packet.addHeader(HEADER_INITIALIZE);
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}