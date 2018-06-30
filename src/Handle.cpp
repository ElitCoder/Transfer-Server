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
			
		case HEADER_INFORM_RESULT: handleInformResult();
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
	
	Base::network().sendFD(fd_, PacketCreator::join(unique));
}

void Handle::handleAvailable() {
	auto packet = PacketCreator::available(connections_);
	
	Base::network().sendFD(fd_, packet);
}

void Handle::handleInform() {
	auto to = packet_->getString();
	auto file = packet_->getString();
	auto directory = packet_->getString();
	
	if (exists(to)) {
		// Ask receiving client for local IP to determine if local direct connection is possible
		Base::network().sendID(getID(to), PacketCreator::informResult(id_, file, directory));
	} else {
		// Deny since the client is not connected
		Base::network().sendFD(fd_, PacketCreator::inform(false));
	}
}

void Handle::handleInformResult() {
	auto accepted = packet_->getBool();
	auto id = packet_->getInt();
	auto num_addresses = packet_->getInt();
	
	vector<string> addresses;
	
	for (int i = 0; i < num_addresses; i++)
		addresses.push_back(packet_->getString());
		
	auto ip_sender = Base::network().getIP(id);
	auto ip_receiver = Base::network().getIP(id_);
	
	bool same_external = false;
	
	// See if they have the same connecting IP, hence same NAT network
	if (ip_sender == ip_receiver)
		same_external = true;
		
	Base::network().sendID(id, PacketCreator::inform(accepted, same_external, addresses));
}

void Handle::handleSend() {
	auto to = packet_->getString();
	auto file = packet_->getString();
	auto directory = packet_->getString();
	auto bytes = packet_->getBytes();
	auto first = packet_->getBool();
	
	// Find out where to send the bytes
	auto id = getID(to);
	
	Base::network().sendID(id, PacketCreator::send(id_, file, directory, bytes, first), false);
}

void Handle::handleSendResult() {
	auto id = packet_->getInt();
	auto result = packet_->getBool();
		
	Base::network().sendID(id, PacketCreator::sendResult(result));
}

void Handle::handleInitialize() {
	auto version = packet_->getString();
	
	auto required = Base::config().get<string>("standard", "");
	auto accepted = !lexicographical_compare(version.begin(), version.end(), required.begin(), required.end());
	
	if (version == required)
		accepted = true;
		
	auto client_url = Base::config().get<string>("client_url", "");
	auto update_url = Base::config().get<string>("update_url", "");

	Base::network().sendID(id_, PacketCreator::initialize(accepted, ERROR_OLD_PROTOCOL, client_url, update_url));
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