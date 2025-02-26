#include <iostream>
#include <chrono>

uint64_t isolate_lsb_ctz(uint64_t bitboard) {
    return 1UL << __builtin_ctzll(bitboard);
}

uint64_t isolate_lsb_bitwise(uint64_t bitboard) {
    return bitboard & -bitboard;
}

int main() {
    uint64_t bitboard = 0b10100;

    // Profile isolate_lsb_ctz
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000000; ++i) {
        isolate_lsb_ctz(bitboard);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_ctz = end - start;
    std::cout << "isolate_lsb_ctz duration: " << duration_ctz.count() << " seconds" << std::endl;

    // Profile isolate_lsb_bitwise
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000000; ++i) {
        isolate_lsb_bitwise(bitboard);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_bitwise = end - start;
    std::cout << "isolate_lsb_bitwise duration: " << duration_bitwise.count() << " seconds" << std::endl;

    return 0;
}
