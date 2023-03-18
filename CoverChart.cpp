#include "coverChart.h"

coverChart::coverChart()
{

}

coverChart::coverChart(std::vector<coveredBool> coveredBools, std::map<int, int> chart)
{

}

void coverChart::set_primeImplicants(std::vector<coveredBool> coveredBools)
{
	_primeImplicants = coveredBools;
}
void coverChart::build_chart(int bits)
{
	int primeImplicantIndex = 0;
	for (int i = 0; i < 2 << bits; i++) //loops on undashed terms
	{
		for (auto pi : _primeImplicants) //loops on primeimplicants (rows)
		{
			if ((pi.value & (~pi.coverIndexes)) == (i & (~pi.coverIndexes))) //bitmasks undashed bits and sees if they are contianed in an implicant
			{
				_chart[i] |= 1 << primeImplicantIndex; //sets bit corresponding to the bool value of the coveredBool in chart 
			}
			primeImplicantIndex++;
		}

		primeImplicantIndex = 0;
	}
}
std::vector<coveredBool> coverChart::get_essential_primes() {
	
	std::vector<int> bitmask_indexes;
	std::map<int, int> occurences;
	utils util;

	for (auto i : _chart) {
		if (util.count_bits(i.second) == 1) {
			bitmask_indexes.push_back(i.second);
		}
	}

	for (auto i : bitmask_indexes) {
		occurences[i]++;
	}

	for (auto i : occurences) {
		if (i.second == 1) {
			essential_primes.push_back(_primeImplicants[util.get_bitset_index(bitmask_indexes, i.first)]);
		}
	}

	return essential_primes;
}

bool coverChart::reduce_chart(int bits)
{
	auto temp = _chart;
	int count = 0;
	for (auto i : _chart)
	{
		if (i.second == 1)
		{
			essential_primes.push_back(_primeImplicants[count]); //add essential prime to vector
			for (auto j : _chart)
			{
				j.second &= 0; //clears bits in the rows and columns covered by EPI
			}
		}
		count++;
	}

	return _chart == temp;
}


bool coverChart::remove_chart_redundancy(int bits) //redesign
{
	bool temp = false;

	for (auto i : _chart) //iterations to check if there is a dominating column to remove
	{
		for (auto j : _chart)
		{
			temp = i.second & j.second;
		}

		if (!temp)
		{
			i.second &= 0; //clears column
		}

		temp = false;
	}

	return _chart == _chart;
}

bool coverChart::three_step_heuristic(int bits)
{
	bool temp = false;

begin:

	if (reduce_chart(bits))
	{

		if (remove_chart_redundancy(bits))
		{
			print_chart(bits);
			return temp; //returns false if it's not simplified (no change to chart)
		}
		temp = true;

		goto begin; //if redundancy is resolved repeat the 3 step heuristic again
	}

	return true;
}

void coverChart::print_chart(int bits)
{
	std::vector<int> IncludedMinterms;
	for (auto i : _chart)
	{
		if (i.second > 1)
		{
			IncludedMinterms.push_back(i.first);
		}
	}

	std::cout << std::right << std::setw(bits * 1.5 + 1) << " | ";

	for (auto i : IncludedMinterms)
	{
		std::cout << i << "  ";
	}

	std::cout << "\n";

	for (int i = 0; i < IncludedMinterms.size() * 4 + 1; i++)
	{
		std::cout << "_";
	}

	std::cout << "\n";

	for (int i = 0; i < _primeImplicants.size(); i++)
	{
		std::cout << *coveredBool::coveredBool_to_binary(_primeImplicants[i], bits) << std::setw(bits * 1.5 - bits) << " | ";
		for (auto j : IncludedMinterms)
		{
			if (_chart[j] >> i & 1)
			{
				std::cout << "X" << std::setw(bits * 1.5 + std::to_string(j).length() - bits);
			}
			else
			{
				std::cout << " " << std::setw(bits * 1.5 + std::to_string(j).length() - bits);
			}
		}
		std::cout << "\n";
	}


}