# include "../srcs/chessengine/Board.hpp"

int find_board_possibilities(Board *board, int depth, int max_depth)
{
    if (depth == max_depth)
        return 1;

    vector<Move> moves = board->get_available_moves();
    // cerr << "Depth: " << depth << " Move count: " << moves.size() << endl;
    
    int possibilities = 0;
    for (Move move : moves)
    {
        // cerr << "Depth: " << depth << " Move: " << move.to_uci() << endl;

        Board *new_board = board->clone();
        new_board->apply_move(move);
        possibilities += find_board_possibilities(new_board, depth + 1, max_depth);
        delete new_board;
    }

    // board->log();

    return possibilities;
}

int test_perft_board(string fen, int *expected_moves, int expected_moves_count)
{
    for (int i = 0; i < expected_moves_count; i++)
    {
        Board board(fen);

        int possible_moves = find_board_possibilities(&board, 0, i + 1);
        if (possible_moves != expected_moves[i])
        {
            cerr << "--- Test failed ---" << endl;
            cerr << "FEN: " << fen << endl;
            cerr << "Depth: " << i << endl;
            cerr << "Expected: " << expected_moves[i] << endl;
            cerr << "Got: " << possible_moves << endl;
            cerr << "-------------------" << endl << endl;
            return 0;
        }

        // cerr << "Depth " << i << " SUCCEED - FEN: " << fen << endl;
    }

    return 1;
}

int main()
{
    // PArse csv file
    ifstream file("perft_dataset/perft_dataset.csv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    cerr << "[PERFT TEST] Start ..." << endl;

    string line;
    getline(file, line);

    int id;
    string fen;
    int depths_count = 2;
    int depths[depths_count];

    int success_count = 0;
    int total_count = 0;
    while (getline(file, line))
    {
        // cerr << line << endl;

        stringstream ss(line);
        string buffer;

        getline(ss, buffer, ',');
        id = stoi(buffer);
        getline(ss, buffer, ',');
        fen = buffer;
        for (int i = 0; i < depths_count; i++)
        {
            getline(ss, buffer, ',');
            depths[i] = stoi(buffer);
        }

        success_count += test_perft_board(fen, depths, depths_count);
        total_count++;

        // cerr << "Id: " << id << endl;
        // cerr << "FEN: " << fen << endl;
        // cerr << "Depths: " << depths[0] << " " << depths[1] << " " << depths[2] << " " << depths[3] << " " << depths[4] << " " << depths[5] << endl;
    }

    cerr << "[PERFT TEST] End: " << success_count << "/" << total_count << " tests were successfull !" << endl;

    return 0;
}
