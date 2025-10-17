#include <future>
#include <iostream>
#include <thread>
#include <vector>

#define InterleaveVectorsON 0
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

		// This program simulates a client sending a network request with payload to a server,
		// the server using that payload to calculate a response, and then sending back that response.
		// The request is awaited on a separate thread (and the server does its work on yet another thread).
		// While the server is busy, control is given back to the main thread.

		const PayloadType payload{1, 2, 3, 4};

		// std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
		std::promise<ResponseType> promise;

		// Spawn a coroutine
		auto coroutine = HandleNetworkRequest(promise);
		coroutine.resume();

		std::thread serverThread(GetAnswerFromServer, std::ref(promise), std::ref(payload));

		std::cout << "Doing other stuff on the main thread..." << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate doing other stuff on the main thread

		std::cout << "Doing more other stuff on the main thread..." << std::endl;

		serverThread.join();

		std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for everything to finish
		std::cout << "Main thread exiting." << std::endl;
	}
#endif // NetworkRequestON

	return 0;
}
