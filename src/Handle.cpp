#include "Handle.h"
#include "Packet.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"
#include "NetworkCommunication.h"

#include <algorithm>

using namespace std;

void disconnectFunction(int fd, size_t id) {
	if (fd) {}
	
	Base::handle().disconnect(id);	
}

void Handle::disconnect(size_t id) {
	lock_guard<mutex> lock(disconnect_mutex_);
	
	disconnect_queue_.push_back(id);
}

void Handle::processDisconnects() {
	lock_guard<mutex> lock(disconnect_mutex_);
		
	while (!disconnect_queue_.empty()) {
		auto& id = disconnect_queue_.front();
		connections_.erase(remove_if(connections_.begin(), connections_.end(), [&id] (auto& peer) { return peer.first == id; }), connections_.end());
		disconnect_queue_.pop_front();
		
		Log(DEBUG) << "Removed " << id << endl;
	}
}

void Handle::process(int fd, size_t connection_id, Packet& packet) {
	processDisconnects();
	
	auto header = packet.getByte();
	
	// See if the user exists before returning packets unless it's a join packet
	if (header != HEADER_JOIN && !exists(connection_id))
		return;

	fd_ = fd;
	id_ = connection_id;
	packet_ = &packet;
	
	switch (header) {
		case HEADER_JOIN: handleJoin();
			break;

		default: {
			Log(WARNING) << "Unknown packet header ";
			printf("%02X", header);
			Log(NONE) << "\n";
		}
	}
}

void Handle::handleJoin() {
	auto name = packet_->getString();
	
	// Add name to connections_ if it's unique
	bool unique = !exists(name);
	
	if (unique)
		connections_.push_back({ id_, name });
	
	Base::network().send(fd_, PacketCreator::join(unique));
}

bool Handle::exists(const string& name) {
	auto iterator = find_if(connections_.begin(), connections_.end(), [&name] (auto& peer) { return peer.second == name; });
	
	return iterator != connections_.end();
}

bool Handle::exists(size_t id) {
	auto iterator = find_if(connections_.begin(), connections_.end(), [&id] (auto& peer) { return peer.first == id; });
	
	return iterator != connections_.end();
}