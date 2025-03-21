from bots_interfaces.CGBot import CGBot
from bots_interfaces.RandomBot import RandomBot
from tools.GameRunner import GameRunner

if __name__ == "__main__":

    runner = GameRunner()
    bot1 = CGBot("BbMmPv-2")
    # bot2 = CGBot("mm2")
    bot2 = RandomBot()

    results = runner.play_games(bot1, bot2, 10, winrate=True)
    print(results)
