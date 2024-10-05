#include "ArraySentiment.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // For formatting floating-point output

const int MAX_REVIEWS = 20491;  // Adjust as needed
const int MAX_WORDS = 20491;     // Maximum number of words in a review

// Custom structure to track word frequencies
struct WordFrequency {
    std::string word;
    int count;
};

// Function to add a word to the frequency array
void addWordToFrequency(WordFrequency freqArray[], int& size, const std::string& word) {
    for (int i = 0; i < size; i++) {
        if (freqArray[i].word == word) {
            freqArray[i].count++;
            return;  // Word already exists, increment count and exit
        }
    }
    // If word doesn't exist, add it to the array
    freqArray[size].word = word;
    freqArray[size].count = 1;
    size++;
}

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
        std::string review, ratingStr;

        // Extract the review (first column) and the rating (second column)
        std::getline(ss, review, ',');    // First column: Review text
        std::getline(ss, ratingStr, ',');  // Second column: User rating

        // Store the review and rating
        reviews[count] = review;

        // Try to convert the rating from string to integer
        try {
            ratings[count] = std::stoi(ratingStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid rating at line " << count + 1 << ": " << ratingStr << std::endl;
            ratings[count] = 0;  // Assign a default value in case of invalid rating
        }

        count++;
    }

    return count;  // Return the number of reviews read
}

// Bubble sort function to sort by sentiment score
void bubbleSort(std::string reviews[], int ratings[], double sentimentScores[], int reviewCount) {
    for (int i = 0; i < reviewCount - 1; ++i) {
        for (int j = 0; j < reviewCount - i - 1; ++j) {
            if (sentimentScores[j] > sentimentScores[j + 1]) {
                // Swap sentiment scores
                double tempScore = sentimentScores[j];
                sentimentScores[j] = sentimentScores[j + 1];
                sentimentScores[j + 1] = tempScore;

                // Swap reviews
                std::string tempReview = reviews[j];
                reviews[j] = reviews[j + 1];
                reviews[j + 1] = tempReview;

                // Swap ratings
                int tempRating = ratings[j];
                ratings[j] = ratings[j + 1];
                ratings[j + 1] = tempRating;
            }
        }
    }
}

int main() {
    ArraySentiment arraySentiment;
    arraySentiment.loadWords("positive-words.txt", "negative-words.txt");

    std::string reviews[MAX_REVIEWS];  // Array for reviews
    int ratings[MAX_REVIEWS];          // Array for ratings
    double sentimentScores[MAX_REVIEWS];  // Array to store sentiment scores
    WordFrequency freqArray[MAX_WORDS];  // Array to track word frequencies
    int freqSize = 0;  // Size of the frequency array

    int totalPositiveWords = 0;
    int totalNegativeWords = 0;

    int reviewCount = readReviewsFromCSV("tripadvisor_hotel_reviews.csv", reviews, ratings);

    clock_t start = clock();

    // Analyze each review and calculate sentiment score
    for (int i = 0; i < reviewCount; i++) {
        int posCount = 0, negCount = 0;
        std::string posWords[MAX_WORDS];
        std::string negWords[MAX_WORDS];
        int posWordCount = 0, negWordCount = 0;

        // Analyze the review
        arraySentiment.analyzeReview(reviews[i], posCount, negCount, posWords, negWords, posWordCount, negWordCount);

        // Add words to the frequency array
        for (int j = 0; j < posWordCount; ++j) {
            addWordToFrequency(freqArray, freqSize, posWords[j]);
        }
        for (int j = 0; j < negWordCount; ++j) {
            addWordToFrequency(freqArray, freqSize, negWords[j]);
        }

        // Accumulate total positive and negative words
        totalPositiveWords += posCount;
        totalNegativeWords += negCount;

        // Calculate and store sentiment score
        sentimentScores[i] = arraySentiment.calculateSentimentScore(posCount, negCount);

        // Display number of positive and negative words
        std::cout << "Review: " << reviews[i] << std::endl;
        std::cout << "Positive words: " << posCount << std::endl;
        for (int j = 0; j < posWordCount; ++j) {
            std::cout << "- " << posWords[j] << std::endl;
        }

        std::cout << "Negative words: " << negCount << std::endl;
        for (int j = 0; j < negWordCount; ++j) {
            std::cout << "- " << negWords[j] << std::endl;
        }

        // Sentiment summary based on score
        if (sentimentScores[i] >= 4) {
            std::cout << "Overall Sentiment: Positive (Score: " << sentimentScores[i] << ")" << std::endl;
        } else if (sentimentScores[i] <= 2) {
            std::cout << "Overall Sentiment: Negative (Score: " << sentimentScores[i] << ")" << std::endl;
        } else {
            std::cout << "Overall Sentiment: Neutral (Score: " << sentimentScores[i] << ")" << std::endl;
        }

        std::cout << "----------------------------" << std::endl;
    }

    // Sort reviews by sentiment score
    bubbleSort(reviews, ratings, sentimentScores, reviewCount);

    // Display the sorted results
    for (int i = 0; i < reviewCount; i++) {
        std::cout << "Review: " << reviews[i] << std::endl;
        std::cout << "Rating given by user: " << ratings[i] << std::endl;
        std::cout << std::fixed << std::setprecision(2);  // Set precision to 2 decimal places
        std::cout << "Sentiment score (1 to 5) is " << sentimentScores[i] << std::endl;
        std::cout << "----------------------------" << std::endl;
    }

    // Display the total summary
    std::cout << "Total Reviews = " << reviewCount << std::endl;
    std::cout << "Total Counts of positive words = " << totalPositiveWords << std::endl;
    std::cout << "Total Counts of negative words = " << totalNegativeWords << std::endl;

    // Sort frequency array in ascending order of count (simple bubble sort)
    for (int i = 0; i < freqSize - 1; ++i) {
        for (int j = 0; j < freqSize - i - 1; ++j) {
            if (freqArray[j].count > freqArray[j + 1].count) {
                // Swap
                WordFrequency temp = freqArray[j];
                freqArray[j] = freqArray[j + 1];
                freqArray[j + 1] = temp;
            }
        }
    }

    // Output frequency of words used in reviews
    std::cout << "\nFrequency of each word used in overall reviews, listed in ascending order based on frequency:\n";
    for (int i = 0; i < freqSize; ++i) {
        std::cout << freqArray[i].word << " = " << freqArray[i].count << " times\n";
    }

    // Maximum and minimum used words
    if (freqSize > 0) {
        std::cout << "\nMaximum used word in the reviews: " << freqArray[freqSize - 1].word << " = " << freqArray[freqSize - 1].count << " times\n";
        std::cout << "Minimum used word in the reviews: " << freqArray[0].word << " = " << freqArray[0].count << " times\n";
    }

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

        std::cout << "Total time taken: " << duration << " seconds.\n";

    return 0;
}


