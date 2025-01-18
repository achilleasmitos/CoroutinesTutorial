#pragma once

#include <future>
#include <vector>

namespace networkrequest
{

using PayloadType = std::vector<int>;

// Simulate a different machine (server) doing computations and giving back results to our requests
void GetAnswerFromServer(std::promise<PayloadType>& promise, const PayloadType& source);

struct Generator;

Generator HandleNetworkRequest(std::promise<PayloadType>& promise, const PayloadType& payload);

} // namespace networkrequest
