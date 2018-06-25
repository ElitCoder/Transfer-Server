#include "Log.h"
#include "Base.h"
#include "Config.h"
#include "NetworkCommunication.h"
#include "Packet.h"
#include "Handle.h"

using namespace std;

static void printStart() {
	Log(NONE) << "Transfer-Server [alpha] [" << __DATE__ << " @ " << __TIME__ << "]\n";
}

static void process() {
	auto port = Base::settings().get<int>("port", 12000);
	auto send_threads = Base::settings().get<int>("sending_threads", 1);
	auto recv_threads = Base::settings().get<int>("receiving_threads", 1);
	
	Base::network().start(port, send_threads, recv_threads);
	
	while (true) {
		auto packet = Base::network().waitForProcessingPackets();
		
		Base::handle().process(get<0>(packet), get<1>(packet), get<2>(packet));
	}	
}

int main() {
	printStart();
	
	Base::settings().parse("config");
	process();
	
	return 0;
}