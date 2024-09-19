#include <iostream>
#include <vector>
#include <string>           
#include <cstdlib>          // For random number generation
#include <ctime>            // For seeding the random number generator with the current time

// Function to display the current state of the guessed word
void displayword(const std::string& word, const std::string& guessedWord) {
    // Loop through each character of the word
    for(size_t i = 0; i < word.length(); ++i) {
        // Print the corresponding guessed character or underscore
        std::cout << guessedWord[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    // List of possible words to guess from
    std::vector<std::string> words = {"computer", "hangman", "programming", "code", "developer"};

    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));

    // Select a random word from the words vector
    std::string selectedWord = words[rand() % words.size()];

    // Initialize the guessedWord with underscores, same length as selectedWord
    std::string guessedWord(selectedWord.length(), '_');

    // Initialize a string to store incorrect guesses
    std::string incorrectGuesses;

    // Set the maximum number of incorrect gueses allowed
    int maxGuesses = 6;

    // Track the number of incorrect guesses
    int incorrectCount = 0;

    // Main game loop: continue until all letters are guessed or max incorrect gueses are reached
    while (incorrectCount < maxGuesses && guessedWord != selectedWord) {
        // Display the current guessed word (letters and underscores)
        std::cout << "Guessed word: ";
        displayword(selectedWord, guessedWord);

        // Show the incorrect guesses made so far
        std::cout << "Incorrect gueses: " << incorrectGuesses << std::endl;

        // Prompt the user to guess a letter
        std::cout << "Enter a letter: ";
        char guess;             // Variable to store the user's guess
        std::cin >> guess;      // Read the guess from input

        // Flag to check if the guess is correct
        bool correctGuess = false;

        // Loop through the selected word to check if the guess matches any letter
        for (size_t i = 0; i < selectedWord.length(); i++)
        {
            if (selectedWord[i] == guess)
            {
                // If the guessed letter matches, update guessedWord at that position
                guessedWord[i] = guess;
                correctGuess = true; // Mark the guess as correct
            }
            
        }
        
        // If the guess was incorrect, update incorrect guesses
        if (!correctGuess) {
            incorrectGuesses += guess;      // Add the wrong guess to the incorrect guesses list
            ++incorrectCount;
            std::cout << "Incorrect! " << (maxGuesses - incorrectCount) << " guesses left." << std::endl;
        }
    }
    
    // Check if the user guessed the entire word correctly
    if (guessedWord == selectedWord)
    {
        std::cout << "Congratulations! You guessed the word: " << selectedWord << std::endl;
    } else {
        // If the user reached the maxium number of incorrect guesses, show the correct word
        std::cout << "Game over! The correct word was: " << selectedWord << std::endl;
    }
    
    return 0;
}

