#ifndef ARRAYSENTIMENT_HPP
#define ARRAYSENTIMENT_HPP

#include <string>

class ArraySentiment {
  private:
    std::string* positiveWords;
    std::string* negativeWords;
    int positiveSize;
    int negativeSize;

  public:
    ArraySentiment();
    ~ArraySentiment();

    void loadWords(const std::string& positiveFile, const std::string& negativeFile);
    void analyzeReview(const std::string& review, int& posCount, int& negCount, std::string posWords[], std::string negWords[], int& posWordCount, int& negWordCount);

    double calculateSentimentScore(int posCount, int negCount);

};

#endif
