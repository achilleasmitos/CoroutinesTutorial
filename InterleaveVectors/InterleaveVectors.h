#pragma once

#include <vector>

namespace interleavevectors
{

struct Generator;

// InterleaveVectors is our main coroutine
Generator InterleaveVectors(std::vector<int> a, std::vector<int> b);

} // namespace interleavevectors
