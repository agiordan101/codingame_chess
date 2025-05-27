
#include "../srcs/gameengine/GameEngineIntTests.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

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

    getline(ss, buff, ' ');
    ischeck = buff[1] == '1';

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

void test_dataset(string file_name)
{
    ifstream file("datasets/" + file_name + ".txt");
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        return;
    }

    // PiecesHeuristic *heuristic = new PiecesHeuristic();
    Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1");

    string lines;
    getline(file, lines, ',');
    stringstream ss(lines);

    int max_len = 50000;
    int dataset_len = 0;
    // float elapsed_times[4] = {0, 0, 0, 0};
    float elapsed_time = 0;
    while (!ss.eof())
    {
        Position position(ss);

        int random_move = rand() % position.move_count;

        // clock_t start_time = clock();
        // board.get_available_moves();
        // elapsed_times[0] += (float)(clock() - start_time) / CLOCKS_PER_SEC * 1000;

        // start_time = clock();
        // board.apply_move(Move(position.moves[random_move]));
        // elapsed_times[1] += (float)(clock() - start_time) / CLOCKS_PER_SEC * 1000;

        // start_time = clock();
        // board.get_check_state();
        // elapsed_times[2] += (float)(clock() - start_time) / CLOCKS_PER_SEC * 1000;

        // start_time = clock();
        // board.get_game_state();
        // elapsed_times[3] += (float)(clock() - start_time) / CLOCKS_PER_SEC * 1000;

        clock_t start_time = clock();

        board = Board(position.fen);

        // Board newboard = board;

        // heuristic->evaluate(&board);

        // board.get_available_moves();
        // board.apply_move(Move(position.moves[random_move]));
        // board.get_game_state();
        // board.get_check_state();
        // board.get_available_moves();

        elapsed_time += (float)(clock() - start_time) / CLOCKS_PER_SEC * 1000;

        // cerr << "[TIME TEST] " << file_name << " in progress ... " << success_count << "/" <<
        // dataset_len << endl;
        dataset_len++;
        if (dataset_len == max_len)
            break;
    }

    // cerr << "[TIME TEST] " << file_name << " - End: get_available_moves() -> " <<
    // elapsed_times[0] << "ms \tfor " << dataset_len << " positions" << endl; cerr << "[TIME TEST]
    // " << file_name << " - End: apply_move()          -> " << elapsed_times[1] << "ms \tfor " <<
    // dataset_len << " positions" << endl; cerr << "[TIME TEST] " << file_name << " - End:
    // get_check_state()     -> " << elapsed_times[2] << "ms \tfor " << dataset_len << " positions"
    // << endl; cerr << "[TIME TEST] " << file_name << " - End: get_game_state()      -> " <<
    // elapsed_times[3] << "ms \tfor " << dataset_len << " positions" << endl;
    cerr << "[TIME TEST] " << file_name << " - End: 4 main functions      -> " << elapsed_time
         << "ms \tfor " << dataset_len << " positions" << endl;
}

int main()
{
    cerr << "[TIME TEST] Start ..." << endl;

    test_dataset("positions");
    test_dataset("check_positions");

    return 0;
}
