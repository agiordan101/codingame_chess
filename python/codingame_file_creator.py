import os
import re

# Example usage
input_filenames = [
    "srcs/chessengine/ChessEngine.hpp",
    "srcs/chessengine/Move.hpp",
    "srcs/chessengine/Board.hpp",
    "srcs/heuristics/AbstractHeuristic.hpp",
    "srcs/heuristics/PiecesHeuristic.hpp",
    "srcs/agents/AbstractAgent.hpp",
    "srcs/agents/MinMaxAlphaBetaAgent.hpp",
    "srcs/players/AbstractPlayer.hpp",
    "srcs/players/BotPlayer.hpp",
    "srcs/gameengine/GameEngine.hpp",
    "srcs/gameengine/GameEngine.cpp",
    "srcs/chessengine/Board.cpp",
    "srcs/chessengine/Move.cpp",
    "srcs/agents/MinMaxAlphaBetaAgent.cpp",
    "srcs/heuristics/PiecesHeuristic.cpp",
    "srcs/players/BotPlayer.cpp",
    "mains/main.cpp",
]


def merge_files(file_name, output_fd):
    regexes = [
        re.compile(r'#include "([^"]+)"'),
    ]

    with open(file_name, "r") as infile:
        output_fd.write("\n\n/*\n\tContent of '" + file_name + "'\n*/\n\n")

        for line in infile:
            # Skip the #include directive
            if not any(regex.match(line) for regex in regexes):

                # Remove comments
                if (
                    "//" in line
                    and "// clang-format on" not in line
                    and "// clang-format off" not in line
                ):
                    line = line.split("//")[0]

                output_fd.write(line)


if __name__ == "__main__":

    output_filename = "mains/maincg.cpp"
    with open(output_filename, "w") as outfile:

        # Add compilation flags for CG compilation
        outfile.write(
            """
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
        """
        )

        # Copy all source code
        for file_name in input_filenames:
            if os.path.exists(file_name):
                merge_files(file_name, outfile)
                # print(f"{file_name} merged.")
            else:
                print(f"Warning: {file_name} does not exist and will be skipped.")

    print(f"Source code merged successfully into {output_filename}")
