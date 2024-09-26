#include "LinkedListSentiment.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

// Define constants for the maximum number of reviews
const int MAX_REVIEWS = 20491;

// Function to read reviews and ratings from CSV file
int readReviewsFromCSV(const std::string& filename, std::string reviews[], int ratings[]) {
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 0;
    }

    // Skip the header (if there is one)
    std::getline(file, line);

    // Read each line from the CSV
    while (std::getline(file, line) && count < MAX_REVIEWS) {
        std::stringstream ss(line);
        std::string review, ratingStr;

        // Extract the review (first column) and the rating (second column)
        std::getline(ss, review, ',');    
        std::getline(ss, ratingStr, ',');  

        // Store the review and rating
        reviews[count] = review;

        // Try to convert the rating from string to integer
        try {
            ratings[count] = std::stoi(ratingStr);  
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid rating at line " << count + 1 << ": " << ratingStr << std::endl;
            ratings[count] = 0;  
        }

        count++;
    }

    return count;  
}

int main() {
    LinkedListSentiment linkedListSentiment;
    linkedListSentiment.loadWords("positive-words.txt", "negative-words.txt");

    std::string reviews[MAX_REVIEWS];  
    int ratings[MAX_REVIEWS];          
    int reviewCount = readReviewsFromCSV("tripadvisor_hotel_reviews.csv", reviews, ratings);

    clock_t start = clock();

    // Analyze each review
    for (int i = 0; i < reviewCount; i++) {
        int posCount = 0, negCount = 0;
        Node* posWordsUsed = nullptr;
        Node* negWordsUsed = nullptr;

        // Analyze the review
        linkedListSentiment.analyzeReview(reviews[i], posCount, negCount, posWordsUsed, negWordsUsed);

        // Display positive and negative words
        std::cout << "Review " << i + 1 << ":\nPositive words: " << posCount << "\n";
        linkedListSentiment.printWordsUsed(posWordsUsed);
        std::cout << "Negative words: " << negCount << "\n";
        linkedListSentiment.printWordsUsed(negWordsUsed);

        // Calculate and display sentiment score
        double sentimentScore = linkedListSentiment.calculateSentimentScore(posCount, negCount);
        std::cout << "Sentiment Score (1-5): " << sentimentScore << "\n";
        std::cout << "Rating provided by user: " << ratings[i] << "\n";
    }

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    std::cout << "Total time taken: " << duration << " seconds.\n";

    return 0;  
}

