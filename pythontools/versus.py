from asyncio import sleep
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

from bots_interfaces.AbstractBot import AbstractBot
from bots_interfaces.CGBot import CGBot
from bots_interfaces.RandomBot import RandomBot
from bots_interfaces.StockfishBot import StockfishBot
from GameRunner import GameRunner


# Update function
def update(frame, fig, ax, xdata, ydata, results, line, bot1, bot2):

    # Check if the figure has been closed
    if not plt.fignum_exists(fig.number):
        return

    # Play a game and get the result
    game = GameRunner()
    result = game.play_game(bot1, bot2)

    if result.winner is True:
        results["W"] += 1
    elif result.winner is False:
        results["L"] += 1
    else:
        results["D"] += 1

    win_ratio = round(results["W"] / (results["W"] + results["L"]), 3)

    # Append the game number and result to the plot data
    xdata.append(frame)
    ydata.append(win_ratio)

    # Update the line data
    line.set_data(xdata, ydata)

    # Update the line label with the last division result
    line.set_label(f'Win ratio: {win_ratio} (Draws: {results["D"]}/{frame})')

    # Adjust the plot limits
    ax.set_xlim(0, frame)
    ax.set_ylim(0, 1)

    # Update the legend
    ax.legend()

    return line,

def versus_in_real_time():
    # Create a figure and an axis
    fig, ax = plt.subplots()

    # Add a horizontal line at Y=0.5
    ax.axhline(0.5, color='r', linestyle='--')

    # Initialize the line data
    xdata, ydata = [], []
    results = {
        "W": 0,
        "D": 0,
        "L": 0
    }
    line, = plt.plot([], [], '-')

    # Create the bots
    bot1 = CGBot("mm2")
    # bot1 = StockfishBot(1100, time_per_turn=100)

    bot2 = CGBot("mm3")
    # bot2 = StockfishBot(1200, time_per_turn=100)

    # Set the axis labels and the title
    ax.set_xlabel('Game count')
    ax.set_ylabel('P1 Win ratio')
    ax.set_title('{} vs {}'.format(str(bot1), str(bot2)))

    # Create an animation
    ani = animation.FuncAnimation(fig, update, fargs=[fig, ax, xdata, ydata, results, line, bot1, bot2], frames=None, blit=False)

    plt.show()


if __name__ == "__main__":
    versus_in_real_time()
