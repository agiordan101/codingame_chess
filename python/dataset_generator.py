import chess
import random

from bots_interfaces.StockfishBot import StockfishBot

"""
# TODO:
    Create a mix of 3 different datasets:
        - Random positions (TODO)
        - Random endgames (TODO)
        - Random checks (TODO)
        - Stockfish games (TODO)
"""


def save_position(file_name: str, board: chess.Board):
    
    file_path = f"datasets/{file_name}.txt"
    file = open(file_path, 'a')

    file.write(board.fen() + "\n")

    file.write(str(len(list(board.legal_moves))) + " ")
    for legal_move in board.legal_moves:
        file.write(legal_move.uci() + " ")
    file.write("\n")

    check = board.is_check()
    file.write(str(1 if check else 0) + " ")

    outcome = board.outcome(claim_draw=True)
    if outcome is None:
        outcome_num = -2 # Game is not over
    elif outcome.winner == chess.WHITE:
        outcome_num = 1 # White wins
    elif outcome.winner == chess.BLACK:
        outcome_num = -1 # Black wins
    else:
        outcome_num = 0 # Drawn
    file.write(str(outcome_num) + "\n")

    file.close()


def save_position_if_needed(board: chess.Board):

    outcome = board.outcome(claim_draw=True)
    if outcome is not None:
        # Do not save draw by repetitions, as FEN doesn't hold the history
        if outcome.termination not in [chess.Termination.THREEFOLD_REPETITION, chess.Termination.FIVEFOLD_REPETITION]:
            save_position("random_endgame_positions", board)

    elif board.is_check():
        save_position("check_positions", board)
    
    else:
        # Save 10% of the positions randomly
        randnum = random.randint(0, 9)
        if randnum == 0:
            save_position("random_positions", board)


def create_random_positions():

    for i in range(100):

        board = chess.Board.from_chess960_pos(random.randint(0, 959))

        while board.outcome(claim_draw=True) is None:

            move = random.choice(list(board.legal_moves))

            board.push(move)
            save_position_if_needed(board)

        print(f"create_random_positions(): Game {i + 1} completed.")


def create_stockfish_positions():

    players_i = 0
    players = [
        StockfishBot(1000, 10),
        StockfishBot(1000, 10),
    ]

    for i in range(100):

        board = chess.Board.from_chess960_pos(random.randint(0, 959))
        players[0].start(board)
        players[1].start(board)

        while board.outcome(claim_draw=True) is None:

            move = players[players_i].get_next_move(board)

            board.push(chess.Move.from_uci(move))
            save_position_if_needed(board)

            players_i = (players_i + 1) % 2

        print(f"create_stockfish_endgame_positions(): Game {i + 1} completed.")


if __name__ == "__main__":
    create_random_positions()
    create_stockfish_positions()
