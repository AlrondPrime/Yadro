#ifndef YADRO_TESTS_CPP
#define YADRO_TESTS_CPP

#include "../src/pch.h"
#include "../src/Tape.hpp"
#include "../src/MergeSort.hpp"
#include "gtest/gtest.h"
#include <random>

int32_t getRandomNumber() {
    std::random_device rd;
    std::mt19937 gen32{rd()};
    std::uniform_int_distribution<int32_t> dist;

    return dist(gen32);
}

TEST(TapeSuite, ReadingSingleValue) {
    // Arrange
    std::filesystem::create_directory("../tests");
    std::string path{"../tests/tape.txt"};
    std::ofstream fout{path, std::fstream::trunc};
    fout << "-2147483648\n";
    fout.close();

    // Act
    std::unique_ptr<Tape> tape = std::make_unique<Tape>(path);
    int32_t val = tape->read();

    // Assert
    ASSERT_EQ(val, -2147483648);
}

TEST(TapeSuite, WritingSingleValue) {
    // Arrange
    std::filesystem::create_directory("../tests");
    std::string path{"../tests/tape.txt"};
    std::ofstream fout{path, std::fstream::trunc};
    fout.close();

    // Act
    std::unique_ptr<Tape> tape = std::make_unique<Tape>(path);
    int32_t val1 = 2147483647;
    tape->write(val1);

    // Assert
    std::ifstream fin{path};
    int32_t val2{};
    fin >> val2;
    fin.close();

    ASSERT_EQ(val1, val2);
}

TEST(TapeSuite, MovingForward) {
    // Arrange
    std::filesystem::create_directory("../tests");
    std::string path{"../tests/tape.txt"};
    std::ofstream fout{path, std::fstream::trunc};
    fout << "-0000000001\n";
    fout << "+0000000002\n";
    fout.close();

    // Act
    std::unique_ptr<Tape> tape = std::make_unique<Tape>(path);
    tape->forward();
    int32_t val = tape->read();

    // Assert
    ASSERT_EQ(val, 2);
}

TEST(TapeSuite, MovingBackward) {
    // Arrange
    std::filesystem::create_directory("../tests");
    std::string path{"../tests/tape.txt"};
    std::ofstream fout{path, std::fstream::trunc};
    fout << "-0000000001\n";
    fout << "+0000000002\n";
    fout.close();

    // Act
    std::unique_ptr<Tape> tape = std::make_unique<Tape>(path);
    tape->forward();
    tape->backward();
    int32_t val = tape->read();

    // Assert
    ASSERT_EQ(val, -1);
}

TEST(TapeSuite, RewritingCell) {
    // Arrange
    std::filesystem::create_directory("../tests");
    std::string path{"../tests/tape.txt"};
    std::ofstream fout{path, std::fstream::trunc};
    fout << "-0000000001\n";
    fout << "+0000000002\n";
    fout.close();
    auto file_size1 = std::filesystem::file_size(path);

    // Act
    std::unique_ptr<Tape> tape = std::make_unique<Tape>(path);
    int32_t val = 256;
    tape->write(val);

    // Assert
    auto file_size2 = std::filesystem::file_size(path);
    ASSERT_EQ(file_size1, file_size2);

    std::ifstream fin{path};
    int32_t val2_1{}, val2_2{};
    fin >> val2_1 >> val2_2;
    fin.close();
    ASSERT_EQ(val2_1, 256);
    ASSERT_EQ(val2_2, 2);
}

TEST(MergeSortSuite, SortRandomNumbers) {
    const int numbers_amount{100};

    // Arrange
    std::filesystem::create_directory("../tests");
    std::string inputFile{"../tests/tapeIn.txt"};
    std::string outputFile{"../tests/tapeOut.txt"};
    std::ofstream inputFile_{inputFile};
    std::ofstream file_creator{outputFile};
    file_creator.close();

    std::vector<int32_t> testData(numbers_amount);
    int32_t val{};
    for (int i{0}; i < numbers_amount; ++i) {
        val = getRandomNumber();
        testData[i] = val;
        inputFile_ << Tape::format(val);
    }
    inputFile_.close();
    std::sort(testData.begin(), testData.end());

    // Act
    MergeSort::sort(inputFile, outputFile);

    // Assert
    std::ifstream outputFile_{outputFile};
    int32_t val2{};
    for (int i{0}; i < numbers_amount; ++i) {
        outputFile_ >> val2;
        ASSERT_EQ(val2, testData[i]);
    }
    outputFile_.close();
}

#endif // YADRO_TESTS_CPP