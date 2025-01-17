#pragma once

#include <vector>

struct Generator;

// InterleaveVectors is our main coroutine
Generator InterleaveVectors(std::vector<int> a, std::vector<int> b);
