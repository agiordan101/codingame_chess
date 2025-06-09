
#include "../srcs/chessengine/ChessEngine.hpp"

// int main()
// {
//     MctsAgent *agent = new MctsAgent(new PiecesHeuristic(), 50);

//     Board *board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1");

//     float game_state = GAME_CONTINUE;
//     while (game_state == GAME_CONTINUE)
//     {
//         vector<Move> moves = board->get_available_moves();
//         Move         move = agent->choose_from(board, moves);

//         cerr << "Move: " << move.to_uci() << endl;
//         board->apply_move(move);
//         board->log();

//         game_state = board->get_game_state();
//     }

//     cerr << "\nGameEngine: Game is over : " << game_state << endl;

//     return 0;
// }

// Function to serialize castles into 8 bits in a uint8_t
uint8_t _serialized_fen_castles_rights(uint64_t castles)
{
    uint8_t result = 0;
    uint8_t availability_shift = 7;
    uint8_t data_shift = 4; // Start with the higher 3 bits

    while (castles)
    {
        // Set first bit to 1 if there is a castle available
        result |= 1 << availability_shift;

        uint64_t rook = _get_least_significant_bit(castles);
        uint8_t  column = _count_trailing_zeros(rook) % 8;

        // Pack into result
        result |= column << data_shift;

        // Move to the next lower 4-bit section
        availability_shift -= 4;
        data_shift -= 4;

        // Remove the actual rook from castles, so we can find the next one
        castles ^= rook;
    }

    return result;
}

// Function to serialize en_passant into 6 bits in a uint8_t
uint8_t _serialize_en_passant(uint64_t en_passant)
{
    if (en_passant == 0)
    {
        return 0; // No en passant available
    }

    uint64_t bit = _get_least_significant_bit(en_passant);
    uint8_t  pos = _count_trailing_zeros(bit);

    // Pack column and row into a uint8_t
    return (pos % 8) << 3 | (pos / 8);
}

int main()
{
    // FEN data: Pieces
    uint64_t white_pawns = 0xFFFF000000000000;
    uint64_t white_knights = 0x0000000000000042;
    uint64_t white_bishops = 0x0000000000000024;
    uint64_t white_rooks = 0x0000000000000081;
    uint64_t white_queens = 0x0000000000000008;
    uint64_t white_king = 0x0000000000000010;
    uint64_t black_pawns = 0x000000000000FFFF;
    uint64_t black_knights = 0x4200000000000000;
    uint64_t black_bishops = 0x2400000000000000;
    uint64_t black_rooks = 0x8100000000000000;
    uint64_t black_queens = 0x0800000000000000;
    uint64_t black_king = 0x1000000000000000;

    // Pack the data into __int128 variables
    __int128 packed1 = white_pawns | (__int128(white_knights) << 64);
    __int128 packed2 = white_bishops | (__int128(white_rooks) << 64);
    __int128 packed3 = white_queens | (__int128(white_king) << 64);
    __int128 packed4 = black_pawns | (__int128(black_knights) << 64);
    __int128 packed5 = black_bishops | (__int128(black_rooks) << 64);
    __int128 packed6 = black_queens | (__int128(black_king) << 64);

    // Print the packed values
    std::cout << "Packed 1: " << std::hex << uint64_t(packed1) << " " << uint64_t(packed1 >> 64)
              << std::endl;
    std::cout << "Packed 2: " << std::hex << uint64_t(packed2) << " " << uint64_t(packed2 >> 64)
              << std::endl;
    std::cout << "Packed 3: " << std::hex << uint64_t(packed3) << " " << uint64_t(packed3 >> 64)
              << std::endl;
    std::cout << "Packed 4: " << std::hex << uint64_t(packed4) << " " << uint64_t(packed4 >> 64)
              << std::endl;
    std::cout << "Packed 5: " << std::hex << uint64_t(packed5) << " " << uint64_t(packed5 >> 64)
              << std::endl;
    std::cout << "Packed 6: " << std::hex << uint64_t(packed6) << " " << uint64_t(packed6 >> 64)
              << std::endl;

    bool     white_turn = true;
    uint64_t white_castles = 0x2800000000000000UL; // Example value
    uint64_t black_castles = 0x0000000000000004UL; // Example value
    uint64_t en_passant = 0x0020000000000000UL;    // Example value
    // uint64_t half_turn_rule = 10;                  // Example value
    // uint64_t game_turn = 123;                      // Example value

    // Serialize additional data into a 32-bit number
    uint8_t turns_bit = (white_turn ? 0b0 : 0b11111111);
    uint8_t white_castles_bits = _serialized_fen_castles_rights(white_castles);
    uint8_t black_castles_bits = _serialized_fen_castles_rights(black_castles);
    uint8_t en_passant_bits = _serialize_en_passant(en_passant);

    uint32_t result = turns_bit | ((uint32_t)white_castles_bits << 8) |
                      ((uint32_t)black_castles_bits << 16) | ((uint32_t)en_passant_bits << 24);
    // // Serialize half_turn_rule (6 bits starting at position 23)
    // result |= (half_turn_rule & 0x3F) << 23;

    // // Serialize game_turn (11 bits maximum starting at position 29)
    // result |= game_turn << 29;

    std::cout << "Serialized result: " << std::bitset<32>(result) << std::endl;

    return 0;
}
