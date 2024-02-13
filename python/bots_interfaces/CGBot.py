import subprocess

from bots_interfaces.AbstractBot import AbstractBot
from chess import Board


class CGBot(AbstractBot):

    # Turn inputs
    lastmove = False
    fen = False
    moves = False

    def __init__(self, name: str):
        self.name = name
        self.path = "bins/" + name

    def __str__(self) -> str:
        return self.name

    def start(self, board: Board):
        self.process = subprocess.Popen(
            self.path,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            universal_newlines=True,
        )

        # Send 2 variables to the bot
        self.process.stdin.write("2\n")
        self.process.stdin.flush()
        self.process.stdin.write("crazyHouse 0\n")
        self.process.stdin.flush()
        self.process.stdin.write("maxMoves 125\n")
        self.process.stdin.flush()

        # Get requested turn inputs
        response = self.process.stdout.readline().strip()

        if "lastmove" in response:
            self.lastmove = True
        if "fen" in response:
            self.fen = True
        if "moves" in response:
            self.moves = True

    def get_next_move(self, board: Board) -> str:

        if self.lastmove:
            if len(board.move_stack) > 0:
                self.process.stdin.write(board.peek().uci() + "\n")
            else:
                self.process.stdin.write("none\n")

        if self.fen:
            # Each part of the fen is sent on a new line
            for f in board.fen(shredder=True).split():
                self.process.stdin.write(f + "\n")

        if self.moves:
            available_moves = [m.uci() for m in list(board.legal_moves)]

            self.process.stdin.write(len(available_moves))
            for m in available_moves:
                self.process.stdin.write(m + "\n")
        self.process.stdin.flush()

        # print("Waiting for the response ...")
        response = self.process.stdout.readline().strip()
        return response

    def stop(self):
        self.process.kill()
