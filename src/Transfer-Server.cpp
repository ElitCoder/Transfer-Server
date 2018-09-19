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
	auto port = Base::config().get<int>("port", 12000);
	auto send_threads = Base::config().get<int>("sending_threads", 1);
	auto recv_threads = Base::config().get<int>("receiving_threads", 1);

	Base::network().start(port, send_threads, recv_threads);
	Base::network().registerDisconnectFunction(disconnectFunction);

	while (true) {
		auto packet = Base::network().waitForProcessingPackets();

		Base::handle().process(get<0>(packet), get<1>(packet), get<2>(packet));
	}
}

int main() {
	printStart();

	Base::config().parse("config");
	process();

	return 0;
}