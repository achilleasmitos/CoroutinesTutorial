#include <iostream>
#include <vector>

#include "CoroutinesTutorial.h"
#include "Generator.h"

using namespace std;

int main()
{
	std::vector<int> a{ 2, 4, 6, 8 };
	std::vector<int> b{ 3, 5, 7, 9 };

	Generator gen{ InterleaveVectors(std::move(a), std::move(b)) };

	while (!gen.is_finished())
	{
		std::cout << gen.get_value() << "\n";
		// std::cout << gen.get_value() << "\n"; --> will print each element twice
		gen.resume();
	}

	return 0;
}
