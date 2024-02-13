import argparse
import random
import time

import matplotlib.pyplot as plt
import numpy as np
from bots_interfaces.AbstractBot import AbstractBot
from bots_interfaces.CGBot import CGBot
from bots_interfaces.RandomBot import RandomBot
from bots_interfaces.StockfishBot import StockfishBot
from matplotlib import animation
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from tools.GameRunner import GameRunner

plt.switch_backend("TkAgg")


def _draw_prediction(x_range: np.ndarray, y_pred: np.ndarray) -> float:
    # Define the tolerance level for closeness to 0.5
    tolerance = 1e-2

    # Find the indices where y_pred is close to 0.5
    close_to_half_indices = np.isclose(y_pred, 0.5, atol=tolerance)

    # If no indices are found, return None
    if not np.any(close_to_half_indices):
        print(
            f"No intersection with 0.5 found within {len(y_pred)} values, with tolerance={tolerance}."
        )
        return None

    # Find the maximum x-value where y_pred is close to 0.5
    estimated_elo = int(x_range[close_to_half_indices].max().round(0))

    return estimated_elo


# def _draw_linear_regression(
#     x_dots: np.ndarray, y_dots: np.ndarray, x_range: np.ndarray
# ):

#     # Créer un modèle de régression linéaire et ajuster aux données transformées
#     model = LinearRegression()
#     model.fit(x_dots, y_dots)

#     # Prédire le winrate en fonction du ELO (Linear)
#     y_pred_linear = model.predict(x_range)

#     estimated_elo = _draw_prediction(x_range, y_pred_linear, "blue")
#     print("_draw_linear_regression - Estimated ELO:", estimated_elo)


def _draw_polynomial_regression(
    x_dots: np.ndarray,
    y_dots: np.ndarray,
    x_range: np.ndarray,
    degree: int,
) -> np.ndarray:

    # Créer des caractéristiques polynomiales jusqu'à un certain degré
    poly_features = PolynomialFeatures(degree=degree)
    x_poly = poly_features.fit_transform(x_dots)

    # Créer un modèle de régression poly et ajuster aux données transformées
    model_poly = LinearRegression()
    model_poly.fit(x_poly, y_dots)

    # Prédire le winrate en fonction du ELO (Polynome degres 2)
    x_range_poly = poly_features.transform(x_range)
    y_pred_poly = model_poly.predict(x_range_poly)

    # estimated_elo = _draw_prediction(x_range, y_pred_poly, color)
    # print("_draw_polynomial_regression - Estimated ELO:", estimated_elo)

    return y_pred_poly


def _challenge_all_stockfish(
    frame: int,
    fig,
    ax,
    scatter,
    lines: list,
    prediction_lines: list,
    elo_list: list[int],
    game_results: dict[int, float],
    p1: AbstractBot,
):
    # Prevent multiple frame 0
    if frame == 0:
        return

    runner = GameRunner()
    p2 = StockfishBot(0, time_per_turn=p1.time_per_turn)

    elo_winrates = []
    for elo in elo_list:

        p2.set_elo_to(elo)
        results = runner.play_games(p1, p2)

        # Adjust the winrate
        game_results[elo]["W"] += results["W"]
        game_results[elo]["L"] += results["L"]
        game_results[elo]["D"] += results["D"]

        if game_results[elo]["W"] + game_results[elo]["L"] == 0:
            elo_winrates.append(0.5)
        else:
            elo_winrates.append(
                game_results[elo]["W"]
                / (game_results[elo]["W"] + game_results[elo]["L"])
            )

    # Create the dataset
    x_dots = np.array(elo_list).reshape(-1, 1)
    y_dots = np.array(elo_winrates)

    # Update the scatter plot
    scatter.set_offsets(np.column_stack((x_dots, y_dots)))
    scatter.set_label(f"{frame} games against each Stockfish")

    x_range = np.linspace(min(elo_list), max(elo_list), 1000).reshape(-1, 1)
    for i, (line, pred_line) in enumerate(zip(lines, prediction_lines)):

        # Fit linear regressions models & predict an ELO
        y_pred = _draw_polynomial_regression(x_dots, y_dots, x_range, degree=i + 1)
        estimated_elo = _draw_prediction(x_range, y_pred)

        # Update curve and prediction line
        line.set_data(x_range, y_pred)
        if estimated_elo is not None:
            pred_line.set_data([estimated_elo, estimated_elo], [0, 0.5])
            pred_line.set_label(f"Estimated ELO: {estimated_elo} (Degree {i + 1})")
        else:
            pred_line.set_label("_nolegend_")

    # Update the legend
    ax.legend()

    # Redraw the plot
    fig.canvas.draw()

    print(f"Frame {frame} end - ELO winrates: {elo_winrates}")


def _elo_rating_ml(player: AbstractBot, elo_list: list):

    # Create a figure and an axis
    fig, ax = plt.subplots()

    scatter = ax.scatter([], [])

    lines = []
    prediction_lines = []
    for color in ["blue", "red", "orange", "purple", "green", "black"]:
        (line,) = ax.plot([], [], color=color)
        lines.append(line)

        (line,) = ax.plot([], [], color=color, linestyle="--")
        prediction_lines.append(line)

    # Add a horizontal line at Y=0.5
    ax.axhline(0.5, color="red", linestyle="--")

    # Update the legend
    ax.set_xlabel("ELO")
    ax.set_ylabel("Win rate")
    ax.set_title(f"ELO rating algorithm on P1={str(p1)}")
    ax.legend()

    # Adjust the plot limits
    ax.set_xlim(min(elo_list), max(elo_list))
    ax.set_ylim(0, 1)

    game_results = {elo: {"W": 0, "D": 0, "L": 0} for elo in elo_list}

    # Create an animation
    ani = animation.FuncAnimation(
        fig,
        _challenge_all_stockfish,
        fargs=[
            fig,
            ax,
            scatter,
            lines,
            prediction_lines,
            elo_list,
            game_results,
            player,
        ],
        frames=None,
        blit=False,
    )

    # Afficher le graphique
    plt.show()


if __name__ == "__main__":
    # Create an ArgumentParser
    parser = argparse.ArgumentParser(
        description="Calculate the ELO rating of a bot.",
        epilog="""You can use the following arguments :
                  - C++ executable name. Will be searched in the ./bin/ directory.
                        Example : "mm2" or "mm3".
                  - Stockfish algorithm: "sf" as prefix, and its ELO as suffixe.
                        Example: "sf1350".
                  - Random bot.
                        Example: "random".""",
    )

    # Add a positional argument for the bot name
    parser.add_argument("bot_name", type=str, help="The name of the bot.")

    # Parse the arguments
    args = parser.parse_args()

    # Create the bot using the name provided as an argument
    if args.bot_name[:2] == "sf":
        p1 = StockfishBot(int(args.bot_name[2:]), time_per_turn=None)
    elif args.bot_name == "random":
        p1 = RandomBot()
    else:
        p1 = CGBot(args.bot_name)

    # Set random seed from current time
    random.seed(int(time.time()))

    stockfish_elos = range(1000, 1700, 300)
    _elo_rating_ml(p1, stockfish_elos)

    # Stub data
    # p1 = StockfishBot(1350)
    # x_dots = np.array([1000, 1100, 1200, 1300, 1400, 1500, 1600]).reshape(-1, 1)
    # y_dots = np.array([0.18, 0.27, 0.31, 0.42, 0.54, 0.50, 0.55])
    # _elo_rating_ml(p1, x_dots, y_dots)
