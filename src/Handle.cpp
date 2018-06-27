#include "Handle.h"
#include "Packet.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"
#include "NetworkCommunication.h"
#include "Config.h"

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

	fd_ = fd;
	id_ = connection_id;
	packet_ = &packet;
	
	auto header = packet.getByte();
	
	switch (header) {
		case HEADER_JOIN: handleJoin();
			break;
			
		case HEADER_AVAILABLE: handleAvailable();
			break;
			
		case HEADER_INFORM: handleInform();
			break;
			
		case HEADER_SEND: handleSend();
			break;
			
		case HEADER_SEND_RESULT: handleSendResult();
			break;
			
		case HEADER_INITIALIZE: handleInitialize();
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

void Handle::handleAvailable() {
	auto packet = PacketCreator::available(connections_);
	
	Base::network().send(fd_, packet);
}

void Handle::handleInform() {
	auto to = packet_->getString();
	auto file = packet_->getString();
	auto directory = packet_->getString();
	
	// Accept if the client is connected
	Base::network().send(fd_, PacketCreator::inform(exists(to)));
}

void Handle::handleSend() {
	auto to = packet_->getString();
	auto file = packet_->getString();
	auto directory = packet_->getString();
	auto bytes = packet_->getBytes();
	auto first = packet_->getBool();
	
	// Find out where to send the bytes
	auto id = getID(to);
	
	Base::network().sendUnique(id, PacketCreator::send(id_, file, directory, bytes, first));
}

void Handle::handleSendResult() {
	auto id = packet_->getInt();
	auto result = packet_->getBool();
		
	Base::network().sendUnique(id, PacketCreator::sendResult(result));
}

void Handle::handleInitialize() {
	auto version = packet_->getString();
	
	auto required = Base::config().get<string>("standard", "");
	auto accepted = lexicographical_compare(version.begin(), version.end(), required.begin(), required.end());
	
	if (version == required)
		accepted = true;

	Base::network().sendUnique(id_, PacketCreator::initialize(accepted));
}

bool Handle::exists(const string& name) {
	auto iterator = find_if(connections_.begin(), connections_.end(), [&name] (auto& peer) { return peer.second == name; });
	
	return iterator != connections_.end();
}

size_t Handle::getID(const string& name) {
	auto iterator = find_if(connections_.begin(), connections_.end(), [&name] (auto& peer) { return peer.second == name; });

	if (iterator == connections_.end())
		throw exception();

	return iterator->first;
}