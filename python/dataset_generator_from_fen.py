import chess


def save_position(file_name: str, board: chess.Board):

    file_path = f"datasets/{file_name}.txt"
    file = open(file_path, "a")

    file.write(board.fen(shredder=True) + "\n")

    file.write(str(len(list(board.legal_moves))) + " ")
    for legal_move in board.legal_moves:
        file.write(legal_move.uci() + " ")
    file.write("\n")

    check = board.is_check()
    file.write(str(1 if check else 0) + " ")

    outcome = board.outcome(claim_draw=True)
    if outcome is None:
        outcome_num = -2  # Game is not over
    elif outcome.winner == chess.WHITE:
        outcome_num = 1  # White wins
    elif outcome.winner == chess.BLACK:
        outcome_num = -1  # Black wins
    else:
        outcome_num = 0  # Drawn
    file.write(str(outcome_num) + "\n")

    file.close()


def create_positions(board, depth_max: int, depth: int = 1):

    while board.outcome(claim_draw=True) is None:

        for move in list(board.legal_moves):

            board.push(move)
            save_position("positions_from_fen", board)

            if depth < depth_max:
                create_positions(board, depth_max, depth + 1)

            board.pop()


if __name__ == "__main__":

    board = chess.Board(
        "2nnrbkr/p1qppppp/8/1ppb4/6PP/3PP3/PPP2P2/BQNNRBKR w HEhe - 1 9", chess960=True
    )
    create_positions(board, 2)
