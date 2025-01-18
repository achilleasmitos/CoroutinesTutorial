#include "NetworkRequest.h"

#include <algorithm>
#include <future>
#include <thread>

#include "Generator.h"

namespace networkrequest
{
void GetAnswerFromServer(std::promise<PayloadType>& promise, const PayloadType& source)
{
	std::cout << "Server thread: " << std::this_thread::get_id() << std::endl;

	PayloadType answer;
	std::copy(source.cbegin(), source.cend(), std::back_inserter(answer));
	std::transform(answer.cbegin(),
		answer.cend(),
		answer.begin(),
		[](int element) { return element * element; });

	// Simulate asynchronicity and more heavy work by the server, network traffic etc
	std::this_thread::sleep_for(std::chrono::seconds(3));

	// Server responded, session will be notified
	promise.set_value(answer);
}

Generator HandleNetworkRequest(std::promise<PayloadType>& promise, const PayloadType& payload)
{
	// Simulate fetching info asynchronously
	std::future<PayloadType> future = promise.get_future();

	std::cout << "Request sent, waiting for data..." << std::endl;

	const auto& result = co_await FutureAwaiter<PayloadType>{future}; // Suspend until data is ready

	std::cout << "Data received." << std::endl;
	std::cout << "Answer from server:\n";
	for (const auto el : result)
	{
		std::cout << el << std::endl;
	}

	// Continue after receiving the data
	std::cout << "Resuming main work..." << std::endl;
}

} // namespace networkrequest
