import argparse
import random
import time

import matplotlib.animation as animation
import matplotlib.pyplot as plt
from bots_interfaces.CGBot import CGBot
from bots_interfaces.RandomBot import RandomBot
from bots_interfaces.StockfishBot import StockfishBot
from tools.GameRunner import GameRunner


# Update function
def _update(frame, fig, ax, xdata, ydata, results, line, bot1, bot2):

    # Check if the figure has been closed
    if not plt.fignum_exists(fig.number):
        return

    # Play a game and get the result
    game = GameRunner()

    result = game.play_games(bot1, bot2, n_game=2)

    results["W"] += result["W"]
    results["L"] += result["L"]
    results["D"] += result["D"]

    game_count = results["W"] + results["L"] + results["D"]
    win_ratio = round((results["W"] + 0.5 * results["D"]) / game_count, 3)

    # Append the game number and result to the plot data
    xdata.append(game_count)
    ydata.append(win_ratio)

    # Update the line data
    line.set_data(xdata, ydata)

    # Update the line label with the last division result
    line.set_label(
        f'Average points: {win_ratio} ({results["W"]}W {results["D"]}D {results["L"]}L / {game_count} games)'
    )

    # Adjust the plot limits
    ax.set_xlim(0, game_count)
    ax.set_ylim(0, 1)

    # Update the legend
    ax.legend()

    return (line,)


def _versus_in_real_time(bot1, bot2):
    # Create a figure and an axis
    fig, ax = plt.subplots()

    # Add a horizontal line at Y=0.5
    ax.axhline(0.5, color="r", linestyle="--")

    # Initialize the line data
    xdata, ydata = [], []
    results = {"W": 0, "D": 0, "L": 0}
    (line,) = plt.plot([], [], "-")

    # Set the axis labels and the title
    ax.set_xlabel("Game count")
    ax.set_ylabel("P1 Win ratio")
    ax.set_title("{} vs {}".format(str(bot1), str(bot2)))

    # Update the legend
    ax.legend()

    # Create an animation
    ani = animation.FuncAnimation(
        fig,
        _update,
        fargs=[fig, ax, xdata, ydata, results, line, bot1, bot2],
        frames=None,
        blit=False,
    )

    plt.show()


if __name__ == "__main__":
    # Create an ArgumentParser
    parser = argparse.ArgumentParser(
        description="Play a game between two bots.",
        epilog="""You can use the following arguments :
                  - C++ executable name. Will be searched in the ./bin/ directory.
                        Example : "mm2" or "mm3".
                  - Stockfish algorithm: "sf" as prefix, and its ELO as suffixe.
                        Example: "sf1350".
                  - Random bot.
                        Example: "random".""",
    )

    # Add two positional arguments
    parser.add_argument("bot1_name", type=str, help="The name of the first bot.")
    parser.add_argument("bot2_name", type=str, help="The name of the second bot.")

    # Parse the arguments
    args = parser.parse_args()

    # Create the bots using the names provided as arguments
    if args.bot1_name == "random":
        bot1 = RandomBot()
    elif args.bot1_name[:2] == "sf":
        bot1 = StockfishBot(args.bot1_name[2:], 50)
    else:
        bot1 = CGBot("bins/" + args.bot1_name)

    if args.bot2_name == "random":
        bot2 = RandomBot()
    elif args.bot2_name[:2] == "sf":
        bot2 = StockfishBot(args.bot2_name[2:], 50)
    else:
        bot2 = CGBot("bins/" + args.bot2_name)

    # Set random seed from current time
    random.seed(int(time.time()))

    _versus_in_real_time(bot1, bot2)
