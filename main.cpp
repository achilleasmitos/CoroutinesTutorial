#include <future>
#include <iostream>
#include <thread>
#include <vector>

#define InterleaveVectorsON 1
#define NetworkRequestON 1

#if InterleaveVectorsON
#include "InterleaveVectors/InterleaveVectors.h"
#include "InterleaveVectors/Generator.h"
#endif // InterleaveVectorsON

#if NetworkRequestON
#include "NetworkRequest/NetworkRequest.h"
#include "NetworkRequest/Generator.h"
#endif // NetworkRequestON

int main()
{
#if InterleaveVectorsON
	{
		using namespace interleavevectors;

		std::vector<int> a{2, 4, 6, 8};
		std::vector<int> b{3, 5, 7, 9};

		Generator gen{InterleaveVectors(std::move(a), std::move(b))};
		// gen.resume(); // --> necessary if initial_suspend is set to suspend_always

		while (!gen.is_finished())
		{
			std::cout << gen.get_value() << "\n";
			// std::cout << gen.get_value() << "\n"; --> will print each element twice
			gen.resume();
		}
	}
#endif // InterleaveVectorsON

#if NetworkRequestON
	{
		using namespace networkrequest;

		const PayloadType payload{1, 2, 3, 4};

		std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
		std::promise<PayloadType> promise;

		// Spawn a coroutine
		auto coroutine = HandleNetworkRequest(promise, payload);
		coroutine.resume();

		std::thread serverThread(GetAnswerFromServer, std::ref(promise), std::ref(payload));

		std::cout << "Doing other stuff on the main thread..." << std::endl;

		// The main thread wouldn't normally exit before server answered
		serverThread.join();

		std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for everything to finish
		std::cout << "Main thread exiting." << std::endl;
	}
#endif // NetworkRequestON

	return 0;
}
