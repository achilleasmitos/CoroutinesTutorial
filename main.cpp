#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "InterleaveVectors/InterleaveVectors.h"
#include "InterleaveVectors/Generator.h"

#include "NetworkRequest/NetworkRequest.h"
#include "NetworkRequest/Generator.h"

int main()
{
	{
		using namespace interleavevectors;

		std::vector<int> a{2, 4, 6, 8};
		std::vector<int> b{3, 5, 7, 9};

		Generator gen{InterleaveVectors(std::move(a), std::move(b))};

		while (!gen.is_finished())
		{
			std::cout << gen.get_value() << "\n";
			// std::cout << gen.get_value() << "\n"; --> will print each element twice
			gen.resume();
		}
	}

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

		std::cout << "Main thread exiting." << std::endl;
	}

	return 0;
}
