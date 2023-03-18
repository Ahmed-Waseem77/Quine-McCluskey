#pragma once



#ifndef COVERCHART_H
#define COVERCHART_H


#include "coveredBool.h"

class coverChart
{
private:
	std::vector<coveredBool> _primeImplicants;
	std::map<int, int> _chart; //each minterm inclusions are represented as a binary number (integer element) bit size == # of coveredBools
	std::vector<coveredBool> essential_primes;
public:
	coverChart();
	coverChart(std::vector<coveredBool> coveredBools, std::map<int, int> chart);

	void set_primeImplicants(std::vector<coveredBool> coveredBools);
	void build_chart(int bits); //iterates on pis, bitmasks them and builds the map
	std::vector<coveredBool> get_essential_primes();

	bool reduce_chart(int bits);
	bool remove_chart_redundancy(int bits); //removes dominating columns,then dominated rows if present

	std::string get_minimized_expression(int bits);
	bool three_step_heuristic(int bits);
	void print_chart(int bits);
};

#endif 