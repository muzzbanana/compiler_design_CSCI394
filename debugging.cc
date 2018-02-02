#include <iostream>

const int* highest(int val, const int* start, const int* finish){
    const int start_value = *start;
    const int middle_value = (finish-start)/2;
    const int* mid = start + middle_value; // pointer to value in the middle of the array
    const int m = *mid;   // value in the middle of the array

    int* non_constant_finish = const_cast<int*>(finish);
    // if val less than smallest value or greater than largest, return nullptr
    if (val > *(--non_constant_finish)|| val < start_value){
        return nullptr;
    }
    if (m == val){
        const int* c = mid + 1;
        if (*c == m){
            return highest(val, c, finish);
        }
        return mid;
    } else if (m > val){
        return highest(val, start, mid);
    } else {
        return highest(val, mid, finish);
    }
}

const int* lowest(int val, const int* start, const int* finish){
    const int start_value = *start;
    if (start_value == val){
        return start;
    }
    const int middle_value = (finish-start)/2;
    const int* mid = start + middle_value; // pointer to value in the middle of the array
    const int m = *mid;   // value in the middle of the array

    int* non_constant_finish = const_cast<int*>(finish);
    // if val less than smallest value or greater than largest, return nullptr   
    if (val > *(--non_constant_finish)|| val < start_value){
        return nullptr;
    }
    if (m == val){
        const int* c = mid - 1;
        if (*c == m){
            return lowest(val, c, finish);
        }
        return mid;
    } else if (m > val){
        return lowest(val, start, mid);
    } else {
        return lowest(val, mid, finish);
    } 
}

int main()
{
    // const int DATA_LEN = 9;
    const int LEN = 7;
    // const int L = 1;
    // const int data[DATA_LEN] = { 1, 1, 1, 4, 4, 5, 6, 7, 10 };
    // const int data1[LEN] = {1, 2, 3, 4, 5, 6, 7};
    const int data3[LEN] = {1, 1, 1, 1, 1, 1, 1};
    // const int data2[L] = {4};

    for (auto search : { 0, 1, 2, 4, 7, 8, 9, 10, 11 }) {
        // const auto where = highest(search, data1, data1 + LEN);
        // const auto where = highest(search, data2, data2 + L);
        // const auto where = highest(search, data3, data3 + LEN);
        const auto where = lowest(search, data3, data3 + LEN);
        // const auto where = lowest(search, data, data + DATA_LEN);
        // const auto where = highest(search, data, data + DATA_LEN);
        std::cout << "Index of elem: " << search << " is: ";
        if (where == nullptr) {
            std::cout << "not found!" << std::endl;
        } else {
            // std::cout << (where-data) << std::endl;
            // std::cout << (where-data1) << std::endl;
            // std::cout << (where-data2) << std::endl;
            std::cout << (where-data3) << std::endl;
        }

    }
    return 0;

}