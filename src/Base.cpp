#include "Base.h"
#include "Config.h"
#include "NetworkCommunication.h"
#include "Handle.h"

Config Base::settings_;
NetworkCommunication Base::network_;
Handle Base::handle_;

Config& Base::settings() {
	return settings_;
}

NetworkCommunication& Base::network() {
	return network_;
}

Handle& Base::handle() {
	return handle_;
}