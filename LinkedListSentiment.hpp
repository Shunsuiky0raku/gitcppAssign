#ifndef LINKEDLISTSENTIMENT_HPP
#define LINKEDLISTSENTIMENT_HPP

#include <string>

// Node structure for the linked list
struct Node {
    std::string word;
    Node* next;
};

// LinkedListSentiment class for handling sentiment analysis using linked lists
class LinkedListSentiment {
private:
    Node* positiveWords;  
    Node* negativeWords;  

public:
    LinkedListSentiment();  
    ~LinkedListSentiment();  

    void loadWords(const std::string& positiveFile, const std::string& negativeFile);  // Load words from files
    void addWord(Node*& head, const std::string& word);  
    bool wordExists(Node* head, const std::string& word);  
    void analyzeReview(const std::string& review, int& posCount, int& negCount, Node*& posWordsUsed, Node*& negWordsUsed);  
    double calculateSentimentScore(int posCount, int negCount); 

    void deleteList(Node*& head);  
    void printWordsUsed(Node* head); 
};

#endif

