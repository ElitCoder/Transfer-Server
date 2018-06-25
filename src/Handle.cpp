#include "Handle.h"
#include "Packet.h"

void Handle::process(int fd, size_t connection_id, Packet& packet) {
	current_fd_ = fd;
	current_id_ = connection_id;
	current_packet_ = &packet;
	
	auto header = packet.getByte();
	
	switch (header) {
	}
}