#include "MergeSort.hpp"

bool MergeSort::_sorted{false};
std::shared_ptr<Tape> MergeSort::in{};
std::unique_ptr<Tape> MergeSort::out{};

std::shared_ptr<Tape> MergeSort::tmp_1{};
std::shared_ptr<Tape> MergeSort::tmp_2{};
std::shared_ptr<Tape> MergeSort::tmp_3{};

std::shared_ptr<Tape> MergeSort::curr_tmp_tape{};
std::shared_ptr<Tape> MergeSort::curr_in_tape{};
