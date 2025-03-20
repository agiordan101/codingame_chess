import argparse
import random
import time

import chess
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
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

    outcome = None
    while outcome is None:

        if players_i == 0:
            data = p1.get_next_move_and_stat(board)
            move = data["move"]

            stats["states"].append(int(data["states"]))

        else:
            move = p2.get_next_move(board)

        board.push(chess.Move.from_uci(move))

        outcome = board.outcome()
        players_i = (players_i + 1) % 2

    p1.stop()
    p2.stop()

    return stats


def _update(frame, fig, ax1, ax2, lines, bot1, stats: dict):

    # Check if the figure has been closed
    if not plt.fignum_exists(fig.number):
        return

    game_stats = play_game_and_return_stats(bot1)

    stats["nodes_count"].append(game_stats["states"])

    stats["max_game_length"] = max(stats["max_game_length"], len(game_stats["states"]))

    # ISSUE: We shouldn't compute the average with zeros, coming from games that ended early
    # TODO: Fix this issue

    # Make sure all the arrays have the same length, the maximum game length of over all games
    for i, game_states in enumerate(stats["nodes_count"]):
        stats["nodes_count"][i].extend(
            [0] * (stats["max_game_length"] - len(game_states))
        )

    # Filter out zeros before computing the average
    all_turn_nodes = []
    for game_i in range(stats["max_game_length"]):
        turn_nodes = []
        for game_states in stats["nodes_count"]:
            if game_i < len(game_states):
                turn_nodes.append(game_states[game_i])
        all_turn_nodes.append(turn_nodes)

    # Find the average nodes computed per turns, filtering out zeros
    nodes_per_turn = np.mean(stats["nodes_count"], axis=0)

    # Update the line data
    xdata = np.arange(stats["max_game_length"])
    ydata = np.array(nodes_per_turn)
    lines[0].set_data(xdata, ydata)

    ax1.set_xlim(0, stats["max_game_length"])
    ax1.set_ylim(0, max(nodes_per_turn) * 1.1)
    ax1.set_title(f"Nodes count per turn ({len(stats['nodes_count'])} games)")

    # Find the average nodes computed per millisecond, for each turns
    nodes_per_ms = nodes_per_turn / 50

    # Update the line data
    xdata = np.arange(stats["max_game_length"])
    ydata = np.array(nodes_per_ms)
    lines[1].set_data(xdata, ydata)

    ax2.set_xlim(0, stats["max_game_length"])
    ax2.set_ylim(0, max(nodes_per_ms) * 1.1)
    ax2.set_title(
        f"Explored Nodes per milliseconds ({len(stats['nodes_count'])} games)"
    )

    # Redraw the plot
    fig.canvas.draw()


def _display_stats(bot1: CGBot):
    # Create a figure and two axes
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))

    lines = []
    lines.append(ax1.plot([], [], color="blue", label="Explored nodes")[0])
    lines.append(
        ax2.plot([], [], color="purple", label="Explored Nodes per milliseconds")[0]
    )

    stats = {
        "max_game_length": 0,
        "nodes_count": [],
    }

    # Set the axis labels and the title for the first plot
    ax1.set_xlabel("Turns")
    ax1.set_ylabel("Node")
    ax1.set_title("Nodes count per turn")

    # Set the axis labels and the title for the second plot
    ax2.set_xlabel("Turns")
    ax2.set_ylabel("Nodes / ms")
    ax2.set_title("Explored Nodes per milliseconds")

    # Update the legend for both plots
    ax1.legend()
    ax2.legend()

    # Create an animation
    ani = animation.FuncAnimation(
        fig,
        _update,
        fargs=[fig, ax1, ax2, lines, bot1, stats],
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
