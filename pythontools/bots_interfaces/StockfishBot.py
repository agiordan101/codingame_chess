import stockfish

from chess import Board

from bots_interfaces.AbstractBot import AbstractBot


class StockfishBot(AbstractBot):

    def __init__(self, ELO: int = 1000, time_per_turn: int = None):

        self.time_per_turn = time_per_turn
        self.stockfish_path = "/home/antoine/stockfish/stockfish/stockfish-ubuntu-x86-64-avx2"
        self.parameters = {
            "Debug Log File": "",
            "Contempt": 0,
            "Min Split Depth": 0,
            "Threads": 6, # More threads will make the engine stronger, but should be kept at less than the number of logical processors on your computer.
            "Ponder": "false",
            "Hash": 2048, # Default size is 16 MB. It's recommended that you increase this value, but keep it as some power of 2. E.g., if you're fine using 2 GB of RAM, set Hash to 2048 (11th power of 2).
            "MultiPV": 1,
            "Skill Level": 20,
            "Move Overhead": 18,
            "Minimum Thinking Time": 20,
            "Slow Mover": 100,
            "UCI_Chess960": "true",
            "UCI_LimitStrength": "true",
            "UCI_Elo": ELO
        }
        self.stockfish = stockfish.Stockfish(
            path=self.stockfish_path,
            parameters=self.parameters
        )

    def __str__(self) -> str:
        return f"StockfishBot(ELO={self.parameters['UCI_Elo']},TPT={self.time_per_turn}ms)"

    def start(self, board: Board):
        self.stockfish.set_elo_rating(self.parameters["UCI_Elo"])
        self.stockfish.set_fen_position(board.fen(shredder=True))

    def get_next_move(self, board: Board) -> str:
        
        # print(f"Before last moves: {self.stockfish.get_fen_position()}")    
        if len(board.move_stack) > 0:
            self.stockfish.make_moves_from_current_position(
                [move.uci() for move in board.move_stack[-2:]]
            )
        # print(f"After last moves: {self.stockfish.get_fen_position()}")

        if self.time_per_turn is None:
            move = self.stockfish.get_best_move()
        else:
            move = self.stockfish.get_best_move_time(self.time_per_turn)
        # print(f"{self} chose: {move}")

        return move

    def stop(self):
        # del self.stockfish
        pass

    def set_elo_to(self, elo: int):
        self.parameters["UCI_Elo"] = elo
