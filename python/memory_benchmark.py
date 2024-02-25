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
        "max_depth": 0,
        "states": [],
        "ttsizes": [],
    }

    players_i = 0
    p1.start(board)
    p2.start(board)

    outcome = None
    while outcome is None:

        if players_i == 0:
            data = p1.get_next_move_and_stat(board)
            move = data["move"]

            stats["max_depth"] = max(stats["max_depth"], int(data["depth"]))
            stats["states"].append(int(data["states"]))

            if "ttsize" in data:
                stats["ttsizes"].append(int(data["ttsize"]))

        else:
            move = p2.get_next_move(board)

        board.push(chess.Move.from_uci(move))

        outcome = board.outcome()
        players_i = (players_i + 1) % 2

    p1.stop()
    p2.stop()

    return stats


def _update(frame, fig, ax, lines, bot1):

    # Check if the figure has been closed
    if not plt.fignum_exists(fig.number):
        return

    game_stats = play_game_and_return_stats(bot1)

    # Update the line data
    max_x = 0
    max_y = 0

    xdata = np.arange(len(game_stats["states"]))
    ydata = np.array(game_stats["states"])
    lines[0].set_data(xdata, ydata)

    max_x = max(max_x, max(xdata))
    max_y = max(max_y, max(ydata))
    
    if len(game_stats["ttsizes"]) > 0:
        xdata = np.arange(len(game_stats["ttsizes"]))
        ydata = np.array(game_stats["ttsizes"])
        lines[1].set_data(xdata, ydata)

        max_x = max(max_x, max(xdata))
        max_y = max(max_y, max(ydata))
    else:
        lines[1].set_label("_nolegend_")

    # Adjust the plot limits
    ax.set_xlim(0, max_x)
    ax.set_ylim(0, max_y)
    ax.set_title(f"Nodes count per turn (Max depth = {game_stats['max_depth']})")

    # Update the legend
    ax.legend()

    # Redraw the plot
    fig.canvas.draw()


def _display_stats(bot1: CGBot):
    # Create a figure and an axis
    fig, ax = plt.subplots()

    lines = []
    lines.append(ax.plot([], [], color="red", label="Explored nodes")[0])
    lines.append(ax.plot([], [], color="purple", label="Transposition table size")[0])

    # Set the axis labels and the title
    ax.set_xlabel("Turns")
    ax.set_ylabel("Node count")
    ax.set_title("Nodes count per turn")

    # Update the legend
    ax.legend()

    # Create an animation
    ani = animation.FuncAnimation(
        fig,
        _update,
        fargs=[fig, ax, lines, bot1],
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

    # Add two positional arguments
    parser.add_argument("bot_name", type=str, help="Names bot.")

    # Parse the arguments
    args = parser.parse_args()

    bot1 = CGBot(args.bot_name)

    # Set random seed from current time
    random.seed(int(time.time()))

    _display_stats(bot1)
