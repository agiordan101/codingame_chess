import numpy as np
import matplotlib.pyplot as plt

from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from bots_interfaces.AbstractBot import AbstractBot
from bots_interfaces.CGBot import CGBot
from bots_interfaces.RandomBot import RandomBot
from bots_interfaces.StockfishBot import StockfishBot
from GameRunner import GameRunner


plt.switch_backend('TkAgg')

def draw_prediction(x_range: np.ndarray, y_pred: np.ndarray, color: str) -> float:

    # Find the indices where y_pred_poly crosses the 0.5 line
    precision = 1e-3
    intersec_indices = []
    while len(intersec_indices) == 0:
        intersec_indices = np.where(np.isclose(y_pred, 0.5, atol=precision))

        if len(intersec_indices) == 0:
            print(f"No intersection with 0.5 found within {len(y_pred)} values, with precision={precision}.")
        precision *= 10

    if x_range[intersec_indices].size == 0:
            raise Exception(f"No X found in {y_pred}")
    estimated_elo = int(x_range[intersec_indices].mean().round(0))

    plt.plot([min(x_range.flatten()), estimated_elo], [0.5, 0.5], color='green', linestyle='--')
    plt.plot([estimated_elo, estimated_elo], [0, 0.5], color=color, linestyle='--', label=f'Estimated ELO={estimated_elo}')
    
    return estimated_elo


def draw_linear_regression(x_dots: np.ndarray, y_dots: np.ndarray, x_range: np.ndarray):

    # Créer un modèle de régression linéaire et ajuster aux données transformées
    model = LinearRegression()
    model.fit(x_dots, y_dots)

    # Prédire le winrate en fonction du ELO (Linear)
    y_pred_linear = model.predict(x_range)

    plt.plot(x_range, y_pred_linear, color='blue', label='P1 win probability linear')

    estimated_elo = draw_prediction(x_range, y_pred_linear, 'blue')
    print("draw_linear_regression - Estimated ELO:", estimated_elo)


def draw_polynomial_regression(x_dots: np.ndarray, y_dots: np.ndarray, x_range: np.ndarray, degree: int = 2, color: str = 'red'):

    # Créer des caractéristiques polynomiales jusqu'à un certain degré
    poly_features = PolynomialFeatures(degree=degree)
    x_poly = poly_features.fit_transform(x_dots)

    # Créer un modèle de régression poly et ajuster aux données transformées
    model_poly = LinearRegression()
    model_poly.fit(x_poly, y_dots)

    # Prédire le winrate en fonction du ELO (Polynome degres 2)
    x_range_poly = poly_features.transform(x_range)
    y_pred_poly = model_poly.predict(x_range_poly)

    plt.plot(x_range, y_pred_poly, color=color, label='P1 win probability degres {}'.format(degree))

    estimated_elo = draw_prediction(x_range, y_pred_poly, color)
    print("draw_polynomial_regression - Estimated ELO:", estimated_elo)


def estimate_elo_rating(p1: AbstractBot, x_dots: np.ndarray, y_dots: np.ndarray, batch_size: int):

    x_range = np.linspace(min(x_dots), max(x_dots), 1000).reshape(-1, 1)

    draw_linear_regression(x_dots, y_dots, x_range)
    draw_polynomial_regression(x_dots, y_dots, x_range)
    draw_polynomial_regression(x_dots, y_dots, x_range, degree=3, color='orange')
    draw_polynomial_regression(x_dots, y_dots, x_range, degree=4, color='purple')

    # Tracer le nuage de points
    plt.scatter(x_dots, y_dots, label=f'{len(x_dots)} games against Stockfish ({batch_size}/vs)')

    # Ajouter des étiquettes et un titre
    plt.xlabel('ELO')
    plt.ylabel('Win rate')
    plt.title(f'ELO rating algorithm on P1={str(p1)}')
    plt.legend()

    # Afficher le graphique
    plt.show()


def elo_rating_ml(player: AbstractBot, elo_list: list, batch_size: int):

    runner = GameRunner()
    p2 = StockfishBot(1350, time_per_turn=player.time_per_turn)

    game_elos = []
    game_results = []
    for elo in elo_list:

        # results = runner.play_games(p1, p2, batch_size, winrate=True)
        p2.set_elo_to(elo)
        for i in range(2):
            results = runner.play_games(p1, p2, batch_size, winrate=True)
            game_elos.append(elo)
            game_results.append(results)

    x_dots = np.array(game_elos).reshape(-1, 1)
    y_dots = np.array(game_results)

    estimate_elo_rating(p1, x_dots, y_dots, batch_size)


if __name__ == "__main__":

    p1 = StockfishBot(1350, time_per_turn=100)
    stockfish_elos = range(600, 1800, 50)
    batch_size = 20

    elo_rating_ml(p1, stockfish_elos, batch_size)

    # Stub data
    # p1 = StockfishBot(1350)
    # x_dots = np.array([1000, 1100, 1200, 1300, 1400, 1500, 1600]).reshape(-1, 1)
    # y_dots = np.array([0.18, 0.27, 0.31, 0.42, 0.54, 0.50, 0.55])
    # elo_rating_ml(p1, x_dots, y_dots)
