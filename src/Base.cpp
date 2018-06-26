#include "Base.h"
#include "Config.h"
#include "NetworkCommunication.h"
#include "Handle.h"

Config Base::config_;
NetworkCommunication Base::network_;
Handle Base::handle_;

Config& Base::config() {
	return config_;
}

NetworkCommunication& Base::network() {
	return network_;
}

Handle& Base::handle() {
	return handle_;
}