#include "LinkedListSentiment.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructor: Initialize head pointers to null
LinkedListSentiment::LinkedListSentiment() {
    positiveWords = nullptr;
    negativeWords = nullptr;
}

// Destructor: Clean up memory by deleting linked lists
LinkedListSentiment::~LinkedListSentiment() {
    deleteList(positiveWords);
    deleteList(negativeWords);
}

// Helper function to add a word to the beginning of a linked list
void LinkedListSentiment::addWord(Node*& head, const std::string& word) {
    Node* newNode = new Node;
    newNode->word = word;
    newNode->next = head;
    head = newNode;
}

// Check if a word exists in the linked list
bool LinkedListSentiment::wordExists(Node* head, const std::string& word) {
    Node* current = head;
    while (current != nullptr) {
        if (current->word == word) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Load positive and negative words into linked lists from files
void LinkedListSentiment::loadWords(const std::string& positiveFile, const std::string& negativeFile) {
    std::ifstream posFile(positiveFile), negFile(negativeFile);
    std::string word;

    // Load positive words into the linked list
    while (posFile >> word) {
        addWord(positiveWords, word);
    }

    // Load negative words into the linked list
    while (negFile >> word) {
        addWord(negativeWords, word);
    }
}

// Analyze a review by counting the positive and negative words found
void LinkedListSentiment::analyzeReview(const std::string& review, int& posCount, int& negCount, Node*& posWordsUsed, Node*& negWordsUsed) {
    std::stringstream ss(review);
    std::string word;
    posCount = 0;
    negCount = 0;

    while (ss >> word) {
        if (wordExists(positiveWords, word)) {
            addWord(posWordsUsed, word);  
            posCount++;
        }
        if (wordExists(negativeWords, word)) {
            addWord(negWordsUsed, word);  
            negCount++;
        }
    }
}

// Calculate sentiment score based on the positive and negative word counts
double LinkedListSentiment::calculateSentimentScore(int posCount, int negCount) {
    int N = posCount + negCount;
    int minRawScore = -N;
    int maxRawScore = N;
    int rawSentimentScore = posCount - negCount;

    if (N == 0) return 3.0;  

    double normalizedScore = (rawSentimentScore - minRawScore) / (double)(maxRawScore - minRawScore);
    return 1 + 4 * normalizedScore;  
}

// Print words used (either positive or negative)
void LinkedListSentiment::printWordsUsed(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        std::cout << "- " << current->word << std::endl;
        current = current->next;
    }
}

// Helper function to delete a linked list and free memory
void LinkedListSentiment::deleteList(Node*& head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

