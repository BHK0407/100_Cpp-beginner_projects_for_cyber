#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"

// Movie structure to hold movie details
struct Movie {
    std::string title;
    int releaseYear;
    std::string director;
    std::string genre;
    std::vector<std::string> cast;
    std::string plot;
    double rating;
};

// Function to convert Movie to JSON format
nlohmann::json movieToJson(const Movie& movie) {
    return {
        {"title", movie.title},
        {"release_year", movie.releaseYear},
        {"director", movie.director},
        {"genre", movie.genre},
        {"cast", movie.cast},
        {"plot", movie.plot},
        {"rating", movie.rating}
    };
}

// Function to load movies from a JSON file
std::vector<Movie> loadMovies(const std::string& filename) {
    std::vector<Movie> movies;
    std::ifstream file(filename);
    if (file.is_open()) {
        nlohmann::json jsonData;
        file >> jsonData;

        for (const auto& movieJson : jsonData) {
            Movie movie;
            movie.title = movieJson["title"];
            movie.releaseYear = movieJson["release_year"];
            movie.director = movieJson["director"];
            movie.genre = movieJson["genre"];
            movie.cast = movieJson["cast"].get<std::vector<std::string>>();
            movie.plot = movieJson["plot"];
            movie.rating = movieJson["rating"];
            movies.push_back(movie);
        }
    }
    return movies;
}

// Function to save movies to a JSON file
void saveMovies(const std::vector<Movie>& movies, const std::string& filename) {
    nlohmann::json jsonData;
    for (const auto& movie : movies) {
        jsonData.push_back(movieToJson(movie));
    }
    std::ofstream file(filename);
    file << jsonData.dump(4); // 4-space indentation
}

// Function to add a new movie
void addMovie(std::vector<Movie>& movies) {
    Movie newMovie;
    std::cout << "Enter movie title: ";
    std::getline(std::cin >> std::ws, newMovie.title);
    std::cout << "Enter release year: ";
    std::cin >> newMovie.releaseYear;
    std::cout << "Enter director: ";
    std::getline(std::cin >> std::ws, newMovie.director);
    std::cout << "Enter genre: ";
    std::getline(std::cin, newMovie.genre);
    std::cout << "Enter cast (comma separated): ";
    std::string castInput;
    std::getline(std::cin, castInput);
    // Split cast input by commas
    std::stringstream ss(castInput);
    std::string actor;
    while (std::getline(ss, actor, ',')) {
        newMovie.cast.push_back(actor);
    }
    std::cout << "Enter plot summary: ";
    std::getline(std::cin, newMovie.plot);
    std::cout << "Enter rating (out of 10): ";
    std::cin >> newMovie.rating;

    movies.push_back(newMovie);
    std::cout << "Movie added successfully.\n";
}

// Function to display movies
void displayMovies(const std::vector<Movie>& movies) {
    for (const auto& movie : movies) {
        std::cout << "Title: " << movie.title << "\n"
                  << "Release Year: " << movie.releaseYear << "\n"
                  << "Director: " << movie.director << "\n"
                  << "Genre: " << movie.genre << "\n"
                  << "Rating: " << movie.rating << "/10\n"
                  << "Plot: " << movie.plot << "\n\n";
    }
}

// Main program
int main() {
    std::vector<Movie> movies;
    std::string filename = "movies.json";

    // Load existing movies from file
    movies = loadMovies(filename);

    int choice;
    do {
        std::cout << "1. Add Movie\n2. Display Movies\n3. Save and Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addMovie(movies);
                break;
            case 2:
                displayMovies(movies);
                break;
            case 3:
                saveMovies(movies, filename);
                std::cout << "Movies saved. Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 3);

    return 0;
}
