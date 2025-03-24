#include "../srcs/chessengine/Board.hpp"

class Position
{
    public:
        string         fen;
        int            move_count;
        vector<string> moves;
        bool           ischeck;
        int            outcome;

        Position(stringstream &ss);
        void print();
};

Position::Position(stringstream &ss)
{
    string buff;

    getline(ss, fen);

    getline(ss, buff, ' ');
    move_count = stoi(buff);

    for (int i = 0; i < move_count; i++)
    {
        getline(ss, buff, ' ');
        moves.push_back(buff);
    }

    getline(ss, buff);
    getline(ss, buff, ' ');
    ischeck = buff[0] == '1';

    getline(ss, buff);
    outcome = stoi(buff);
}

void Position::print()
{
    cerr << "FEN: " << fen << endl;
    cerr << "Is check: " << ischeck << endl;
    cerr << "Outcome: " << outcome << endl;
    cerr << "Move count: " << move_count << endl;
    cerr << "Moves: ";
    for (string move : moves)
        cerr << move << " ";
    cerr << endl;
}

int compare_string_lists(vector<string> &list1, vector<string> &list2)
{
    if (list1.size() != list2.size())
        return 0;

    sort(list1.begin(), list1.end());
    sort(list2.begin(), list2.end());

    for (size_t i = 0; i < list1.size(); i++)
        if (list1[i] != list2[i])
            return 0;

    return 1;
}

int test_position(Position *position)
{
    Board board(position->fen, true, false);
    int   success = 1;

    vector<Move> board_moves = board.get_available_moves();
    bool         check_state = board.get_check_state();
    int          game_state = board.get_game_state();

    // Compare moves
    vector<string> board_moves_uci;
    for (Move move : board_moves)
        board_moves_uci.push_back(move.to_uci());

    if (compare_string_lists(board_moves_uci, position->moves) == 0)
    {
        success = 0;
        cerr << "--- Test failed ---" << endl;

        cerr << "Got: ";
        for (string move : board_moves_uci)
            cerr << move << " ";
        cerr << endl << endl;

        cerr << "Expected: ";
        position->print();

        cerr << "-------------------" << endl << endl;
    }

    // Compare check
    if (check_state != position->ischeck)
    {
        success = 0;
        cerr << "--- Test failed ---" << endl;

        cerr << "Got check: " << check_state << endl << endl;

        cerr << "Expected: ";
        position->print();
        cerr << "-------------------" << endl << endl;
    }

    // Compare outcome
    if (game_state != position->outcome)
    {
        success = 0;
        cerr << "--- Test failed ---" << endl;

        cerr << "Got outcome: " << game_state << endl << endl;

        cerr << "Expected: ";
        position->print();
        cerr << "-------------------" << endl << endl;
    }

    return success;
}

void test_dataset(string file_name)
{
    ifstream file("datasets/" + file_name + ".txt");
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        return;
    }

    string lines;
    getline(file, lines, ',');
    stringstream ss(lines);

    int max_len = 50000;
    int dataset_len = 0;
    int success_count = 0;
    while (!ss.eof())
    {
        Position position(ss);
        success_count += test_position(&position);

        // cerr << "[DATASET TEST] " << file_name << " in progress ... " << success_count << "/" <<
        // dataset_len << endl;
        dataset_len++;
        if (dataset_len == max_len)
            break;
    }

    cerr << "[DATASET TEST] " << file_name << " - End: " << success_count << "/" << dataset_len
         << " tests were successfull !" << endl;
}

int main()
{
    cerr << "[DATASET TEST] Start ..." << endl;

    // test_dataset("positions_from_fen");
    test_dataset("positions");
    test_dataset("check_positions");
    test_dataset("endgame_positions");

    return 0;
}
