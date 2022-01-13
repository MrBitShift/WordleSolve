#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <map>

#include "Dictionary.hpp"

class WordleSolver
{
private:
	Dictionary dict;
	static std::map<char, double> letterFrequencyScores;

	size_t wordleLength;

	std::unordered_set<char> wrong;
	std::vector<std::unordered_set<char>> constraints;
	std::unordered_set<char> contains;
	std::string pattern;

	static double FrequencyScore(std::string& s);

public:
	constexpr static char patternWildcard = '*';

	WordleSolver(size_t _wordleLength, Dictionary &_dict);
	WordleSolver(size_t _wordleLength, Dictionary &_dict, std::string _wrong, std::vector<std::string> _constraints,
		std::string _contains, std::string _pattern);

	void AddWrong(std::string _wrong);
	void AddWrong(char c);
	void AddConstraints(std::vector<std::string> _constraints);
	void AddConstraints(size_t pos, char c);
	void AddContains(std::string _contains);
	void AddContains(char c);
	void UpdatePattern(std::string _pattern);
	void UpdatePattern(size_t pos, char c);
	size_t WordleLength();

	bool IsMatch(const std::string &word);

	std::string EliminatingGuess();
	std::vector<std::string> Guesses();

	int Confidence();
};

class WordLengthException : public std::length_error
{
	using std::length_error::length_error;
};