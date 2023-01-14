import os
import subprocess as sp

# cpp_compilation = sp.check_output(['g++', 'src/main.cpp', '-o', '../mychessbot.out'], stdin=r, timeout=problem.timelimit)
os.system("g++ src/main.cpp -o mychessbot_unittest.out")

# process = sp.Popen(['ls'], shell=True, stdout=sp.PIPE, stdin=sp.PIPE)
process = sp.Popen(['./mychessbot_unittest.out'], shell=True, stdout=sp.PIPE, stdin=sp.PIPE)

chess_inputs = [
    "2",
    "crazyHouse 0",
    "maxMoves 125",
]
stdin = bytes('\n'.join(chess_inputs), 'UTF-8')

stdout, stderr = process.communicate(input=stdin)

print(f"stdout: {stdout}")
print(f"stderr: {stderr}")
