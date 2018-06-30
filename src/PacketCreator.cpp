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

Packet PacketCreator::inform(bool result, bool try_direct, int port, const vector<string>& receiving_addresses) {
	Packet packet;
	packet.addHeader(HEADER_INFORM);
	packet.addBool(result);
	packet.addBool(try_direct);
	packet.addInt(receiving_addresses.size());
	packet.addInt(port);
	
	for (auto& address : receiving_addresses)
		packet.addString(address);

	packet.finalize();
	
	return packet;
}

Packet PacketCreator::informResult(int id, const string& file, const string& directory) {
	Packet packet;
	packet.addHeader(HEADER_INFORM_RESULT);
	packet.addInt(id);
	packet.addString(file);
	packet.addString(directory);
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
	
	// To not mess up direct connection packet structure
	packet.addInt(0);
	
	packet.addBool(result);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::initialize(bool result, int code, const string& client, const string& update) {
	Packet packet;
	packet.addHeader(HEADER_INITIALIZE);
	packet.addBool(result);
	packet.addInt(code);
	packet.addString(client);
	packet.addString(update);
	packet.finalize();
	
	return packet;
}