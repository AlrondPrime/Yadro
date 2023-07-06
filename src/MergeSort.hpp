#ifndef YADRO_MERGESORT_HPP
#define YADRO_MERGESORT_HPP

#include "BinaryTape.hpp"
#include "TextTape.hpp"
#include "MergeSort.hpp"

class MergeSort {
private:
    static bool _sorted;
    static std::shared_ptr<TextTape> in;
    static std::unique_ptr<TextTape> out;

    static std::shared_ptr<TextTape> tmp_1;
    static std::shared_ptr<TextTape> tmp_2;
    static std::shared_ptr<TextTape> tmp_3;

    static std::shared_ptr<TextTape> curr_tmp_tape;
    static std::shared_ptr<TextTape> curr_in_tape;

    static void split() {
        _sorted = true;
        tmp_1->cells_written = 0;
        tmp_2->cells_written = 0;
        std::cout << "tmp_3->cells_written=" << tmp_3->cells_written << std::endl;
        curr_tmp_tape = tmp_1;

        int32_t prev_val, next_val;
        prev_val = curr_in_tape->read();
        curr_in_tape->forward();
        curr_tmp_tape->write(prev_val);
        curr_tmp_tape->forward();
        ++curr_tmp_tape->cells_written;

        while (!curr_in_tape->end() && !curr_in_tape->logicalEnd()) {
            next_val = curr_in_tape->read();
            curr_in_tape->forward();
            ++curr_in_tape->current_cell;

            if (next_val < prev_val) {
                _sorted = false;
                switchTmpTapes();
            }

            prev_val = next_val;
            curr_tmp_tape->write(prev_val);
            curr_tmp_tape->forward();
            ++curr_tmp_tape->cells_written;
        }
    }

    static void merge() {
        int32_t first_val, second_val;
        tmp_3->cells_written = 0;
        std::cout << "tmp_1->cells_written=" << tmp_1->cells_written << std::endl;
        std::cout << "tmp_2->cells_written=" << tmp_2->cells_written << std::endl;

        while (!tmp_1->end() && !tmp_2->end() &&
               !tmp_1->logicalEnd() && !tmp_2->logicalEnd()) {
            first_val = tmp_1->read();
            second_val = tmp_2->read();

            if (first_val < second_val) {
                tmp_3->write(first_val);
                tmp_3->forward();
                ++tmp_3->cells_written;

                tmp_1->forward();
                ++tmp_1->current_cell;
            } else {
                tmp_3->write(second_val);
                tmp_3->forward();
                ++tmp_3->cells_written;

                tmp_2->forward();
                ++tmp_2->current_cell;
            }
        }

        std::cout << "One of tapes is over" << std::endl;

        while (!tmp_1->end() && !tmp_1->logicalEnd()) {
            first_val = tmp_1->read();
            tmp_1->forward();
            ++tmp_1->current_cell;

            tmp_3->write(first_val);
            tmp_3->forward();
            ++tmp_3->cells_written;
        }

        while (!tmp_2->end() && !tmp_2->logicalEnd()) {
            second_val = tmp_2->read();
            tmp_2->forward();
            ++tmp_2->current_cell;

            tmp_3->write(second_val);
            tmp_3->forward();
            ++tmp_3->cells_written;
        }
    }

    static void switchTmpTapes() {
        std::cout << "Switching tapes" << std::endl;
        if (curr_tmp_tape == tmp_1)
            curr_tmp_tape = tmp_2;
        else
            curr_tmp_tape = tmp_1;
    };

public:
    static void sort(std::string_view inputFile, std::string_view outputFile) {
        in = std::make_unique<TextTape>(inputFile);
        out = std::make_unique<TextTape>(outputFile);
        out->clear();
        tmp_1 = std::make_unique<TextTape>("../tmp/tmp1.txt");
        tmp_1->clear();
        tmp_2 = std::make_unique<TextTape>("../tmp/tmp2.txt");
        tmp_2->clear();
        tmp_3 = std::make_unique<TextTape>("../tmp/tmp3.txt");
        tmp_3->clear();

        // Load data from input tape into 2 temp tapes
        in->cells_written = std::numeric_limits<uint64_t>::max();
        curr_in_tape = in;
        std::cout << "\tSplitting" << std::endl;
        split();
        curr_in_tape = tmp_3;

        // Sort data using 3 temp tapes:
        // two for splitting and third for merging
        while (!_sorted) {
            std::cout << "\tMerging" << std::endl;
            tmp_1->rewindToBegin();
            tmp_2->rewindToBegin();
            tmp_3->rewindToBegin();
            merge();
            std::cout << "\tSplitting" << std::endl;
            tmp_1->rewindToBegin();
            tmp_2->rewindToBegin();
            tmp_3->rewindToBegin();
            split();
        }

        // Sorted, writing into output file
        tmp_3->rewindToBegin();
        while (!tmp_3->end() && !tmp_3->logicalEnd()) {
            out->write(tmp_3->read());
            ++out->cells_written;
            out->forward();
            tmp_3->forward();
        }
    }
};

bool MergeSort::_sorted{false};
std::shared_ptr<TextTape> MergeSort::in{};
std::unique_ptr<TextTape> MergeSort::out{};

std::shared_ptr<TextTape> MergeSort::tmp_1{};
std::shared_ptr<TextTape> MergeSort::tmp_2{};
std::shared_ptr<TextTape> MergeSort::tmp_3{};

std::shared_ptr<TextTape> MergeSort::curr_tmp_tape{};
std::shared_ptr<TextTape> MergeSort::curr_in_tape{};

#endif //YADRO_MERGESORT_HPP