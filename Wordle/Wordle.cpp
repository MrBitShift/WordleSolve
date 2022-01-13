#include <iostream>

#include <string>
#include <numeric>

#include "Dictionary.hpp"
#include "WordleSolver.hpp"

int main()
{
    Dictionary dict("20k.txt", 5);
    WordleSolver solver(5, dict);

    std::string guessType;
    std::string guess;
    std::string colorCode;
    while (true)
    {
        std::cout << "Current confidence: " << solver.Confidence() << ". ";
        std::cout << "Enter guess type (e/g): ";
        std::getline(std::cin, guessType);
        if (guessType == "e")
        {
            guess = solver.EliminatingGuess();
        }
        else
        {
            auto guesses = solver.Guesses();
            guess = std::accumulate(guesses.begin(), guesses.end(), std::string(""), [](const std::string& left, const std::string& right)
                -> std::string { if (left == "") return right; else return left + ", " + right; });
        }

        if (guess.size() == 0)
        {
            std::cout << "No matches" << std::endl;
            continue;
        }
        std::cout << "Guess: " << guess <<  std::endl;
        std::cout << "Enter square color code: ";
        while (true)
        {
            std::getline(std::cin, colorCode);
            if (colorCode.size() != solver.WordleLength())
            {
                std::cout << "Must be length " + solver.WordleLength() << std::endl;
                continue;
            }
            break;
        }

        for (int i = 0; i < colorCode.size(); i++)
        {
            std::vector<std::string> constraints(solver.WordleLength());

            switch (tolower(colorCode[i]))
            {
            case 'g': // green
                solver.AddContains(guess[i]);
                solver.UpdatePattern(i, guess[i]);
                break;
            case 'y': // yellow
                solver.AddContains(guess[i]);
                solver.AddConstraints(i, guess[i]);
                break;
            case 'b': // black
                solver.AddWrong(std::string(1, guess[i]));
                solver.AddConstraints(i, guess[i]);
                break;
            default:
                break;
            }
        }
    }
}