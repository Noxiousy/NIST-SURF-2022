import sys, subprocess, re

fcpw_results = []
zeno_results = []

args = " " + sys.argv[1] + " " + sys.argv[2] + " " + sys.argv[3] + " " + sys.argv[4] + " true"

# fcpw results
subprocess.call("./fcpw.exe" + args, shell = True, stdout = subprocess.PIPE)

with open("results.txt", "r") as infile :
    for line in infile.readlines()[1:] :
        result = re.findall("\((.+?)\)", line)[-1]
        fcpw_results.append(result)

# zeno results
subprocess.call("./zeno.exe" + args, shell = True, stdout = subprocess.PIPE)

with open("results.txt", "r") as infile :
    for line in infile.readlines()[1:] :
        result = re.findall("\((.+?)\)", line)[-1]
        zeno_results.append(result)

for i in range(int(sys.argv[3])) :
    fcpw_results[i] = round(float(fcpw_results[i]), 4)
    zeno_results[i] = round(float(zeno_results[i]), 4)
    print(fcpw_results[i], zeno_results[i], "!!" if fcpw_results[i] == zeno_results[i] else "")
