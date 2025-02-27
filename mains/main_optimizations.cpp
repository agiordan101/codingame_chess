#include <iostream>
#include <chrono>

// uint64_t isolate_lsb_ctz(uint64_t bitboard) {
//     return 1UL << __builtin_ctzll(bitboard);
// }

// uint64_t isolate_lsb_bitwise(uint64_t bitboard) {
//     return bitboard & -bitboard;
// }

// int main() {
//     uint64_t bitboard = 0b10100;

//     // Profile isolate_lsb_ctz
//     auto start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 1000000000; ++i) {
//         isolate_lsb_ctz(bitboard);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration_ctz = end - start;
//     std::cout << "isolate_lsb_ctz duration: " << duration_ctz.count() << " seconds" << std::endl;

//     // Profile isolate_lsb_bitwise
//     start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < 1000000000; ++i) {
//         isolate_lsb_bitwise(bitboard);
//     }
//     end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration_bitwise = end - start;
//     std::cout << "isolate_lsb_bitwise duration: " << duration_bitwise.count() << " seconds" << std::endl;

//     return 0;
// }


#include <iostream>
#include <chrono>
#include <cstdint>

// Example lookup table
uint64_t pawn_capture_lookup[64][2] = {
    // Initialize with some values
    {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16},
    {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32},
    {33, 34}, {35, 36}, {37, 38}, {39, 40}, {41, 42}, {43, 44}, {45, 46}, {47, 48},
    {49, 50}, {51, 52}, {53, 54}, {55, 56}, {57, 58}, {59, 60}, {61, 62}, {63, 64},
    {65, 66}, {67, 68}, {69, 70}, {71, 72}, {73, 74}, {75, 76}, {77, 78}, {79, 80},
    {81, 82}, {83, 84}, {85, 86}, {87, 88}, {89, 90}, {91, 92}, {93, 94}, {95, 96},
    {97, 98}, {99, 100}, {101, 102}, {103, 104}, {105, 106}, {107, 108}, {109, 110}, {111, 112},
    {113, 114}, {115, 116}, {117, 118}, {119, 120}, {121, 122}, {123, 124}, {125, 126}, {127, 128}
};

uint64_t shift_operation(uint64_t src) {
    return src >> 16;
}

uint64_t lookup_operation(int index) {
    return pawn_capture_lookup[index][0];
}

int main() {
    uint64_t src = 0x123456789ABCDEF0;
    int index = 42;

    // Profile lookup_operation
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000000; ++i) {
        lookup_operation(index);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_lookup = end - start;
    std::cout << "lookup_operation duration: " << duration_lookup.count() << " seconds" << std::endl;

    // Profile shift_operation
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000000; ++i) {
        shift_operation(src);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_shift = end - start;
    std::cout << "shift_operation duration: " << duration_shift.count() << " seconds" << std::endl;

    return 0;
}
