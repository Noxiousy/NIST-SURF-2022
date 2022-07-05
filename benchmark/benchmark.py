import sys, os, subprocess

# python3 benchmark.py [obj file] [# of queries] [outfile (optional)]
try :
    outfile = sys.argv[3]
except :
    outfile = "benchmark.csv"
finally :
    print("Writing to " + outfile)

# write to benchmark.csv
with open(outfile, "w") as csv :
    commands = ["./fcpw.exe " + sys.argv[1] + " contains true 1729 " + sys.argv[2],
                "./fcpw.exe " + sys.argv[1] + " closest_point true 1729 " + sys.argv[2],
                "./zeno.exe " + sys.argv[1] + " contains true 1729 " + sys.argv[2],
                "./zeno.exe " + sys.argv[1] + " closest_point true 1729 " + sys.argv[2]]

    processes = subprocess.Popen(shell = True, stdout = subprocess.PIPE)
    processes.communicate(commands)
