#include "WordleSolver.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include <iostream>

WordleSolver::WordleSolver(size_t _wordleLength, Dictionary &_dict)
{
	wordleLength = _wordleLength;
	constraints.resize(wordleLength);
	pattern.resize(wordleLength, patternWildcard);
	dict = _dict;
}

WordleSolver::WordleSolver(size_t _wordleLength, Dictionary &_dict, std::string _wrong, std::vector<std::string> _constraints,
	std::string _contains, std::string _pattern) : WordleSolver(_wordleLength, _dict)
{
	AddWrong(_wrong);
	AddConstraints(_constraints);
	AddContains(_contains);
	UpdatePattern(_pattern);
}

void WordleSolver::AddWrong(std::string _wrong)
{
	wrong.insert(_wrong.begin(), _wrong.end());
}

void WordleSolver::AddWrong(char c)
{
	wrong.insert(c);
}

void WordleSolver::AddConstraints(std::vector<std::string> _constraints)
{
	if (_constraints.size() != wordleLength)
		throw WordLengthException("Constraints must be an array of length " + wordleLength);

	auto src = _constraints.begin();
	auto dest = constraints.begin();
	for (; src < _constraints.end() && dest < constraints.end(); src++, dest++)
	{
		dest->insert(src->begin(), src->end());
	}
}


void WordleSolver::AddConstraints(size_t pos, char c)
{
	if (pos >= wordleLength)
		throw WordLengthException("Pos must be 0-" + wordleLength);

	constraints[pos].insert(c);
}

void WordleSolver::AddContains(std::string _contains)
{
	contains.insert(_contains.begin(), _contains.end());
}

void WordleSolver::AddContains(char c)
{
	contains.insert(c);
}

void WordleSolver::UpdatePattern(std::string _pattern)
{
	if (_pattern.size() != wordleLength)
		throw WordLengthException("Pattern must be a string of length" + wordleLength);

	auto src = _pattern.begin();
	auto dest = pattern.end();
	for (; src < _pattern.end() && dest < pattern.end(); src++, dest++)
		if (*src != patternWildcard)
			*dest = *src;
}

void WordleSolver::UpdatePattern(size_t pos, char c)
{
	if (pos >= wordleLength)
		throw WordLengthException("Pos must be 0-" + wordleLength);

	pattern[pos] = c;
}

size_t WordleSolver::WordleLength()
{
	return wordleLength;
}

bool WordleSolver::IsMatch(const std::string& word)
{
	if (word.size() != wordleLength)
		return false;

	for (char c: word)
		if (wrong.count(c) > 0)
			return false;

	auto c = word.begin();
	auto cs = constraints.begin();
	for (; c < word.end() && cs < constraints.end(); c++, cs++)
		if ((*cs).count(*c) > 0)
			return false;

	if (contains.size() > 0)
	{
		for (char c : contains)
			if (word.find(c) == std::string::npos)
				return false;
	}

	c = word.begin();
	auto p = pattern.begin();
	for (; c < word.end() && p < pattern.end(); c++, p++)
		if (*p != patternWildcard && *c != *p)
			return false;

	return true;
}

double WordleSolver::FrequencyScore(std::string& s)
{
	std::unordered_set<char> chars;
	chars.insert(s.begin(), s.end());
	return std::accumulate(chars.begin(), chars.end(), 0.0, [](const double& a, const char& b) ->
		double {
			return a + letterFrequencyScores[toupper(b)]; 
		});
}

std::string WordleSolver::EliminatingGuess()
{
	double maxScore = -1;
	std::string topGuess;
	
	for (std::string word : dict.values)
	{
		if (IsMatch(word))
		{
			double score = FrequencyScore(word);
			if (score > maxScore)
			{
				topGuess = word;
				maxScore = score;
			}
		}
	}

	return topGuess;
}

std::vector<std::string> WordleSolver::Guesses()
{
	auto result = std::find_if(dict.values.begin(), dict.values.end(), [this](const std::string& e)
		{ return IsMatch(e); });
	if (result != dict.values.end())
		return std::vector<std::string> {*result};
	else
		return { "" };
}

int WordleSolver::Confidence()
{
	auto match1 = std::find_if(dict.values.begin(), dict.values.end(), [this](const std::string& e)
		{ return IsMatch(e); });
	if (match1 != dict.values.end())
		return std::distance(match1, std::find_if(match1 + 1, dict.values.end(), [this](const std::string& e)
			{ return IsMatch(e); }));
	else
		return 0;
}

std::map<char, double> WordleSolver::letterFrequencyScores = {
	{ 'E', 56.88 },
	{ 'A', 43.31 },
	{ 'R', 38.64 },
	{ 'I', 38.45 },
	{ 'O', 36.51 },
	{ 'T', 35.43 },
	{ 'N', 33.92 },
	{ 'S', 29.23 },
	{ 'L', 27.98 },
	{ 'C', 23.13 },
	{ 'U', 18.51 },
	{ 'D', 17.25 },
	{ 'P', 16.14 },
	{ 'M', 15.36 },
	{ 'H', 15.31 },
	{ 'G', 12.59 },
	{ 'B', 10.56 },
	{ 'F', 9.24 },
	{ 'Y', 9.06 },
	{ 'W', 6.57 },
	{ 'K', 5.61 },
	{ 'V', 5.13 },
	{ 'X', 1.48 },
	{ 'Z', 1.39 },
	{ 'J', 1.00 },
	{ 'Q', 1 }
};