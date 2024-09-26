#include "ArraySentiment.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

// Define maximum number of reviews and words
const int MAX_REVIEWS = 25000;
const int MAX_WORDS = 25000;  // Add this to fix 'MAX_WORDS' error

// Function to read reviews and ratings from CSV file
int readReviewsFromCSV(const std::string& filename, std::string reviews[], int ratings[]) {
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 0;
    }

    // Skip the header
    std::getline(file, line);

    // Read each line from the CSV
    while (std::getline(file, line) && count < MAX_REVIEWS) {
        std::stringstream ss(line);
        std::string review;
        std::string ratingStr;

        // Extract the review and rating
        std::getline(ss, review, ',');  // Skip first column
        std::getline(ss, review, ',');  // Extract review
        std::getline(ss, ratingStr, ',');  // Extract rating
        std::cout << "Parsed review: " << review << " with rating: " << ratingStr << std::endl;
        // Store review
        reviews[count] = review;

        // Try to convert rating to an integer
        try {
            ratings[count] = std::stoi(ratingStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid rating at line " << count + 1 << ": " << ratingStr << std::endl;
            ratings[count] = 0;  // Assign a default value in case of invalid rating
        }

        count++;
    }

    return count;  // Return the number of reviews read
}

int main() {
    ArraySentiment arraySentiment;
    arraySentiment.loadWords("positive-words.txt", "negative-words.txt");

    std::string reviews[MAX_REVIEWS];  // Manual array for reviews
    int ratings[MAX_REVIEWS];          // Manual array for ratings
    int reviewCount = readReviewsFromCSV("tripadvisor_hotel_reviews.csv", reviews, ratings);

    clock_t start = clock();

    // Analyze each review
    for (int i = 0; i < reviewCount; i++) {
        int posCount, negCount, posWordCount, negWordCount;
        std::string posWords[MAX_WORDS];  // Declare array for positive words
        std::string negWords[MAX_WORDS];  // Declare array for negative words

        // Analyze the review
        arraySentiment.analyzeReview(reviews[i], posCount, negCount, posWords, negWords, posWordCount, negWordCount);

        // Display positive and negative words
        std::cout << "Review " << i + 1 << ":\nPositive words: " << posCount << "\n";
        for (int j = 0; j < posWordCount; ++j) {
            std::cout << "- " << posWords[j] << "\n";
        }
        std::cout << "Negative words: " << negCount << "\n";
        for (int j = 0; j < negWordCount; ++j) {
            std::cout << "- " << negWords[j] << "\n";
        }

        // Calculate and display sentiment score
        double sentimentScore = arraySentiment.calculateSentimentScore(posCount, negCount);
        std::cout << "Sentiment Score (1-5): " << sentimentScore << "\n";
        std::cout << "Rating provided by user: " << ratings[i] << "\n";

        // Comparison
        if (ratings[i] != (int)sentimentScore) {
            std::cout << "User's rating doesn't match sentiment score.\n";
        } else {
            std::cout << "User's rating matches the sentiment score.\n";
        }

        std::cout << "-------------------\n";
    }

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    std::cout << "Total time taken: " << duration << " seconds.\n";

    return 0;
}

