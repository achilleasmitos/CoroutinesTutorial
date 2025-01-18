#include "InterleaveVectors.h"

#include "Generator.h"

namespace interleavevectors
{

// Another coroutine, helper for InterleaveVectors
static Generator YieldElementsInVectorA(const std::vector<int>& vec)
{
	for (const auto& element : vec)
	{
		co_yield element;
	}
	// co_return; // --> equivalent without it being here
}

// Another coroutine, helper for InterleaveVectors
static Generator YieldElementsInVectorB(const std::vector<int>& vec)
{
	for (const auto& element : vec)
	{
		co_yield element;
	}
	// co_return; // --> equivalent without it being here
}

Generator InterleaveVectors(std::vector<int> a, std::vector<int> b)
{
	auto firstGen = YieldElementsInVectorA(a);
	// firstGen.resume(); // --> necessary if initial_suspend is set to suspend_always
	auto secondGen = YieldElementsInVectorB(b);
	// secondGen.resume(); // --> necessary if initial_suspend is set to suspend_always

	while (!firstGen.is_finished() || !secondGen.is_finished())
	{
		if (!firstGen.is_finished())
		{
			co_yield firstGen.get_value();
			firstGen.resume();
		}

		if (!secondGen.is_finished())
		{
			co_yield secondGen.get_value();
			secondGen.resume();
		}
	}
	// co_return; // --> equivalent without it being here
}

} // namespace interleavevectors
