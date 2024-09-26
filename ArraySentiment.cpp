#include "ArraySentiment.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

ArraySentiment::ArraySentiment() {
    positiveWords = new std::string[25000];
    negativeWords = new std::string[25000];
    positiveSize = 0;
    negativeSize = 0;
}

ArraySentiment::~ArraySentiment() {
    delete[] positiveWords;
    delete[] negativeWords;
}

void ArraySentiment::loadWords(const std::string& positiveFile, const std::string& negativeFile) {
    std::ifstream posFile(positiveFile), negFile(negativeFile);
    std::string word;

    while (posFile >> word) {
        positiveWords[positiveSize++] = word;
    }

    while (negFile >> word) {
        negativeWords[negativeSize++] = word;
    }
}

// Analyze review, counting positive and negative words
void ArraySentiment::analyzeReview(const std::string& review, int& posCount, int& negCount, std::string posWords[], std::string negWords[], int& posWordCount, int& negWordCount) {
    std::stringstream ss(review);
    std::string word;

    posCount = 0;
    negCount = 0;
    posWordCount = 0;
    negWordCount = 0;

    while (ss >> word) {
        for (int i = 0; i < positiveSize; ++i) {
            if (word == positiveWords[i]) {
                posWords[posWordCount++] = word;
                posCount++;
            }
        }

        for (int i = 0; i < negativeSize; ++i) {
            if (word == negativeWords[i]) {
                negWords[negWordCount++] = word;
                negCount++;
            }
        }
    }
}

// Calculate sentiment score based on positive and negative word counts
double ArraySentiment::calculateSentimentScore(int posCount, int negCount) {
    int N = posCount + negCount;
    int minRawScore = -N;
    int maxRawScore = N;
    int rawSentimentScore = posCount - negCount;

    if (N == 0) return 3.0;  

    double normalizedScore = (rawSentimentScore - minRawScore) / (double)(maxRawScore - minRawScore);
    return 1 + 4 * normalizedScore;  
}

