
#include "utils.h"

bool utils::check_characters_within_range(std::string* Expression) 
{
	for (int i = 0; i < Expression->length(); i++)
	{
		//if expression char isnt: within uppercase ASCII Latin block A..Z, integers 0, 1 
		//and is not equal to ( ) ' + \n
		if (((*Expression)[i] < 65 || (*Expression)[i] > 90)
			&& (*Expression)[i] != '0' && (*Expression)[i] != '1'
			&& (*Expression)[i] != '(' && (*Expression)[i] != ')'
			&& (*Expression)[i] != '\'' && (*Expression)[i] != ' ' 
			&& (*Expression)[i] != '+' && (*Expression)[i] != '\n')
		{
			std::cout << "invalid Syntax";
			return false;
		}
	}
}

bool utils::check_parentheses(std::string* Expression)
{
	std::stack<char> ParenthesesStack;

	for (int i = 0; i < Expression->length(); i++)
	{
		//checks for empty parentheses () or open parenthesis followed by ' +
		if (i != Expression->length() && (*Expression)[i] == '(' && ((*Expression)[i + 1] == ')' || (*Expression)[i + 1] == '\''|| (*Expression)[i + 1] == '+'))
		{
			return false;
		}

		//checks for valid parentheses
		if ((*Expression)[i] == '(')
		{
			ParenthesesStack.push('(');
		}
		else if (ParenthesesStack.empty() && (*Expression)[i] == ')')
		{
			return false;
		}
		else if((*Expression)[i] == ')')
		{
			ParenthesesStack.pop();
		}
	} 

	if (!ParenthesesStack.empty())
	{
		return false;
	}

	return true;
}

int utils::verify_expression_syntax(std::string* Expression)
{
	return check_characters_within_range(Expression) && check_parentheses(Expression);
}

std::string* utils::decimal_to_binary(int Dec, int Bits)
{
	std::string* Bin = new std::string;
	
	for (int i = 0; Dec > 0; i++)
	{
		*Bin = std::to_string(Dec % 2) + *Bin;
		Dec /= 2;
	}

	int temp = (*Bin).length();

	for (int i = 0; i < Bits - temp; i++)
	{
		*Bin = '0' + *Bin;
	}

	return Bin;
}

std::set<char>* utils::unique_literals(std::string* Expression)
{
	std::set<char> *_uniqueLiterals = new std::set<char>;
	for(int i = 0; i < Expression->length(); i++)
	{
		if ((*Expression)[i] >= 65 && (*Expression)[i] <= 90)
		{
			_uniqueLiterals->insert((*Expression)[i]);
		}
	}
	return _uniqueLiterals;
}

normalizedString* utils::parse_string(std::string* Expression) //simple parser for minterms (for now), for any Boolean expression we'd need a infix to RND parser
{
	normalizedString* RetStr = new normalizedString();
	std::string temp;

	for (int i = 0; i < Expression->length(); i++)	//iterates on string and removes spaces
	{
		if ((*Expression)[i] != ' ')
			temp += (*Expression)[i];
	}

	*Expression = temp;
	std::istringstream Stream(*Expression);
	std::string s;

	while (std::getline(Stream, s, '+')) //normalizes string using delimiter +
	{
		RetStr->push_back(s);
	}

	return RetStr;
}

int utils::minterm_to_binary(std::string Minterm)
{
	int x = 0;
	for (int i = 0; i < Minterm.size(); i++)
	{
		x <<= 1; //shifting bit to its correct bit position depending on i 

		if (i != Minterm.size() && Minterm[i + 1] != '\'') //skips not operators
		{
			x |= 1; //setting LSB bit
		}
		else
		{
			i++; //part of skipping not op
		}
	}

	return x;
}

int utils::count_bits(int x)
{
	int count = 0;
	for (int i = 0; i < 32; i++) //looping on default 32 bit integer type
	{
		if (x & 1 << i) //checking LSB positions from 0 to 32th bit
		{
			count++;
		}
	}

	return count;
}
int utils::get_bitset_index(const std::vector<int>& bitset_vector, int bitset) {
	auto it = std::find(bitset_vector.begin(), bitset_vector.end(), bitset);
	if (it != bitset_vector.end()) {
		return std::distance(bitset_vector.begin(), it);
	}
	return -1;
}
std::string utils::vector_to_expression(std::vector<std::string> terms, std::set<char>* uliterals)
{
	std::string result = "";
	bool first = true;

	for (std::string term : terms)
	{
		std::string temp = "";
		bool firstChar = true;
		for (char c : term)
		{
			if (c == '0')
			{
				temp += (firstChar) ? "!" : " && !";
			}
			else if (c == '1')
			{
				temp += (firstChar) ? "" : " || ";
			}
			else
			{
				temp += (firstChar) ? "" : " || ";
				temp += (c >= 'A' && c <= 'Z') ? c : (*uliterals->find(c));
			}

			firstChar = false;
		}

		result += (first) ? temp : (" || " + temp);
		first = false;
	}

	return result;
}
bool utils::is_canonical_sop(std::string function, std::set<char> uniqueLiterals)
{
	// Check if function is in SOP form
	if (function.find('(') != std::string::npos || function.find(')') != std::string::npos)
	{
		std::cerr << "Error: Function is not in canonical SOP form\n";
		return false;
	}

	// Check if variables are in alphabetical order
	std::vector<char> literals;
	for (char c : function) {
		if (isalpha(c)) {
			literals.push_back(c);
		}
	}
	std::sort(literals.begin(), literals.end());
	std::string sortedLiterals(literals.begin(), literals.end());
	std::string sortedFunction = function;
	std::sort(sortedFunction.begin(), sortedFunction.end());
	if (sortedLiterals != sortedFunction)
	{
		std::cerr << "Error: Variables are not in alphabetical order\n";
		return false;
	}

	// Check that all variables are present
	for (auto lit : literals)
	{
		if (uniqueLiterals.find(lit) == uniqueLiterals.end())
		{
			std::cerr << "Error: Variable " << lit << " is not a valid input variable\n";
			return false;
		}
	}

	return true;
}