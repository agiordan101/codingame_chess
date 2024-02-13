import random

from bots_interfaces.AbstractBot import AbstractBot
from chess import Board


class RandomBot(AbstractBot):

    def __str__(self) -> str:
        return "RandomBot"

    def get_next_move(self, board: Board) -> str:
        return random.choice([m.uci() for m in list(board.legal_moves)])
