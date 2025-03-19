import argparse
import random
import time
import chess
import numpy as np

import matplotlib.animation as animation
import matplotlib.pyplot as plt
from bots_interfaces.CGBot import CGBot
from bots_interfaces.StockfishBot import StockfishBot


def play_game_and_return_stats(p1: CGBot):

    p2 = StockfishBot(500, 1)
    board = chess.Board.from_chess960_pos(random.randint(0, 959))

    stats = {
        "states": [],
    }

    players_i = 0
    p1.start(board)
    p2.start(board)

    try:
        outcome = None
        while outcome is None:

            if players_i == 0:
                data = p1.get_next_move_and_stat(board)
                move = data["move"]

                if "states" in data:
                    stats["states"].append(int(data["states"]))

            else:
                move = p2.get_next_move(board)

            board.push(chess.Move.from_uci(move))

            outcome = board.outcome()
            players_i = (players_i + 1) % 2

    except Exception as e:
        print(f"Error while playing a game with {p1.name}: {e}")
        print(f"FEN was {board.fen()}")
        return stats

    p1.stop()
    p2.stop()

    return stats


def _update_bot_plot(line, bot, bot_stats: dict):

    game_stats = play_game_and_return_stats(bot)

    bot_stats["nodes_count"].append(game_stats["states"])

    #Remove odd number that are more than two times greater than the 2nd gretaest number
    for i, game_states in enumerate(bot_stats["nodes_count"]):
        if len(game_states) > 1:
            max_val = max(game_states)
            second_max_val = max([val for val in game_states if val != max_val])
            if max_val > 2 * second_max_val:
                for j in range(len(game_states)):
                    if game_states[j] == max_val:
                        game_states[j] = second_max_val

    bot_stats["max_game_length"] = max(
        bot_stats["max_game_length"],
        len(game_stats["states"])
    )

    # Compute the average number of nodes explored for each turn
    nodes_per_turn = np.array([
        np.mean(
            [
                game[turn]
                for game in bot_stats["nodes_count"]
                if turn < len(game)
            ]
        )
        for turn in range(bot_stats["max_game_length"])
    ])

    # Find the average nodes computed per millisecond, for each turns
    bot_stats["nodes_per_ms"] = nodes_per_turn / 50

    # Update the line data
    xdata = np.arange(bot_stats["max_game_length"])
    ydata = np.array(bot_stats["nodes_per_ms"])
    line.set_data(xdata, ydata)


def _update(frame, fig, ax, lines: list, bots: list[CGBot], stats: dict):

    # Check if the figure has been closed
    if not plt.fignum_exists(fig.number):
        return

    # Run a game for each bot and update their plots
    for line, bot in zip(lines, bots):
        _update_bot_plot(line, bot, stats[bot.name])

    max_game_length = max(
        stats[bot.name]["max_game_length"]
        for bot in bots
    )
    max_nodes_per_ms = max(
        np.max(stats[bot.name]["nodes_per_ms"])
        for bot in bots
    )

    ax.set_xlim(0, max_game_length)
    ax.set_ylim(0, max_nodes_per_ms * 1.1)
    ax.set_title(f"Explored Nodes per milliseconds ({frame + 1} games)")

    #TODO: Find intersection of the two last bot lines: stats[bot.name]["nodes_per_ms"]


    # Redraw the plot
    fig.canvas.draw()


def _display_stats(bots: list[CGBot]):
    # Create a figure and one axis
    fig, ax = plt.subplots(1, 1, figsize=(12, 6))

    lines = [
        ax.plot([], [], label=bot.name)[0]
        for bot in bots
    ]

    stats = {
        bot.name: {
            "max_game_length": 0,               # int: The maximum turn number of all games
            "nodes_count": [],                  # list[list[int]]: The number of nodes explored at each turn, for all games
            "nodes_per_turn": np.ndarray([]),   # np.ndarray: The actual average number of nodes explored for each turn (Computed at each game ends)
        }
        for bot in bots
    }

    ax.set_xlabel("Turns")
    ax.set_ylabel("Nodes / ms")
    ax.legend()

    # Create an animation
    ani = animation.FuncAnimation(
        fig,
        _update,
        fargs=[fig, ax, lines, bots, stats],
        frames=None,
        blit=False,
    )

    plt.show()


if __name__ == "__main__":
    # Create an ArgumentParser
    parser = argparse.ArgumentParser(
        description="Display turn statistic about bots.",
        epilog="""You can use the following arguments :
                  - C++ executable name. Will be searched in the ./bin/ directory.
                        Example : "mm2" or "mmid".""",
    )

    # Add a positional argument that can take multiple values
    parser.add_argument("bot_names", type=str, nargs='+', help="Names of the bots.")

    # Parse the arguments
    args = parser.parse_args()

    # Create a list of CGBot instances from the provided bot names
    bots = [CGBot(bot_name) for bot_name in args.bot_names]

    # Set random seed from current time
    random.seed(int(time.time()))

    _display_stats(bots)
