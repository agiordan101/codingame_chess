import random

import chess
from bots_interfaces.AbstractBot import AbstractBot


class GameRunner:

    def _play_game(self, p1: AbstractBot, p2: AbstractBot, rd: int):

        if rd is None:
            rd = random.randint(0, 959)
        board = chess.Board.from_chess960_pos(rd)

        players_i = 0
        players = [p1, p2]
        p1.start(board)
        p2.start(board)

        outcome = None
        while outcome is None:

            move = players[players_i].get_next_move(board)

            try:
                board.push(chess.Move.from_uci(move))
            except Exception as e:
                print(f"Exception: {e}")

                # exit(0)

                # Act like it's a draw
                # return chess.Outcome(
                #     termination=chess.Termination(2),
                #     winner=None
                # )

                # Give the win to the other player
                # winner=True when p1 wins
                return chess.Outcome(
                    termination=chess.Termination(1),
                    winner=chess.WHITE if players_i == 1 else chess.BLACK,
                )

            outcome = board.outcome(claim_draw=True)
            players_i = (players_i + 1) % 2

        p1.stop()
        p2.stop()

        print("----- Game over -----")
        print(f"Opponents: W: {str(p1)} - B: {str(p2)}")
        print("Result: " + board.result())
        print("Outcome: " + str(outcome))

        return outcome

    def play_games(
        self,
        p1: AbstractBot,
        p2: AbstractBot,
        n_game: int = 1,
        rd: int = None,
        winrate: float = False,
    ):

        p1_wins = 0
        p2_wins = 0
        draws = 0

        p1_as_white = n_game // 2
        for i in range(p1_as_white):
            outcome = self._play_game(p1, p2, rd)

            if outcome.winner is True:
                p1_wins += 1
            elif outcome.winner is False:
                p2_wins += 1
            else:
                draws += 1

        p1_as_black = n_game - p1_as_white
        for i in range(p1_as_black):
            outcome = self._play_game(p2, p1, rd)

            if outcome.winner is True:
                p2_wins += 1
            elif outcome.winner is False:
                p1_wins += 1
            else:
                draws += 1

        _average_score = (p1_wins + 0.5 * draws) / n_game

        print(f"----- {n_game} games over -----")
        print(f"P1 wins: {p1_wins} -> {str(p1)}")
        print(f"P2 wins: {p2_wins} -> {str(p2)}")
        print(f"  Draws: {draws}")
        print(f"Winrate: {_average_score}")

        if winrate:
            return _average_score

        return {"W": p1_wins, "D": draws, "L": p2_wins}
