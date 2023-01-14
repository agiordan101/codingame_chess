import os
import subprocess as sp

# cpp_compilation = sp.check_output(['g++', 'src/main.cpp', '-o', '../mychessbot.out'], stdin=r, timeout=problem.timelimit)
os.system("g++ src/main.cpp -o mychessbot_unittest.out")

# process = sp.Popen(['ls'], shell=True, stdout=sp.PIPE, stdin=sp.PIPE)
process = sp.Popen(['./mychessbot_unittest.out'], shell=True, stderr=sp.PIPE, stdout=sp.PIPE, stdin=sp.PIPE)

chess_inputs = [
    "2",
    "crazyHouse 0",
    "maxMoves 125",
]
stdin_data = bytes('\n'.join(chess_inputs), 'UTF-8')

process.stdin.write(stdin_data)
# print(stdin_data, file=)
# process.stdin.flush()

# print(process.stdout.read())
# print(process.stdout.close())

stdout, stderr = process.communicate(stdin_data)
print(stdout, stderr)

exit()


"""
p = Popen(["python", "-u", "1st.py"], stdin=PIPE, stdout=PIPE, bufsize=1)
print p.stdout.readline(), # read the first line
for i in range(10): # repeat several times to show that it works
    print >>p.stdin, i # write input
    p.stdin.flush() # not necessary in this case
    print p.stdout.readline(), # read output

print p.communicate("n\n")[0], # signal the child to exit,
                               # read the rest of the output, 
                               # wait for the child to exit"""