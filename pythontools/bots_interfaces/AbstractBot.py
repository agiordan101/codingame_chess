from abc import ABC, abstractmethod
from chess import Board


class AbstractBot(ABC):

    time_per_turn: int = None

    def __str__(self) -> str:
        raise NotImplementedError()

    def start(self, board: Board):
        pass

    def get_next_move(self, _: Board) -> str:
        raise NotImplementedError()

    def stop(self):
        pass
