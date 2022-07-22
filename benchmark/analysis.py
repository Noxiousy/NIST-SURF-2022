import os, sys, numpy as np, subprocess, math, matplotlib.pyplot as plt

# python3 [.obj files] [queryType]
obj_files = os.listdir(sys.argv[1])
obj_files = sorted(obj_files, key = lambda x : os.stat(sys.argv[1] + x).st_size)

prep_means = [[], []]
prep_stdevs = [[], []]
query_means = [[], []]
query_stdevs = [[], []]

for obj_file in obj_files :
    print("Fetching", obj_file, "benchmarks...")
    subprocess.call("python3 benchmark.py ../obj_files/" + obj_file + " " + sys.argv[2] + " 10000000", shell = True, stdout = subprocess.PIPE)

    # get benchmarks from subprocess
    with open("outfiles/benchmark.csv", "r") as csv :
        lines = csv.readlines()
        fcpw_prep = lines[3].split(", "); zeno_prep = lines[9].split(", ")
        fcpw_query = lines[6].split(", "); zeno_query = lines[12].split(", ")

        prep_means[0].append(float(fcpw_prep[0]))
        prep_means[1].append(float(zeno_prep[0]))
        prep_stdevs[0].append(float(fcpw_prep[4]) / math.sqrt(15))
        prep_stdevs[1].append(float(zeno_prep[4]) / math.sqrt(15))

        query_means[0].append(float(fcpw_query[0]))
        query_means[1].append(float(zeno_query[0]))
        query_stdevs[0].append(float(fcpw_query[4]) / math.sqrt(15))
        query_stdevs[1].append(float(zeno_query[4]) / math.sqrt(15))

# double bar plot for average preprocessing times
x = np.arange(len(obj_files))
width = 0.35

fig, ax = plt.subplots()
ax.bar(x - width / 2, prep_means[0], width, yerr = prep_stdevs[0], label = "FCPW")
ax.bar(x + width / 2, prep_means[1], width, yerr = prep_stdevs[1], label = "Zeno")

ax.set_ylim(bottom = 0)
ax.set_ylabel("Time (seconds)")
ax.set_xlabel(".obj files")
ax.set_title("Average FCPW & Zeno Preprocessing Times")
ax.set_xticks(x, obj_files)
ax.legend()

fig.tight_layout()
plt.savefig("preprocessing.png")

# double bar plot for average query times
fig, ax = plt.subplots()
ax.bar(x - width / 2, query_means[0], width, yerr = query_stdevs[0], label = "FCPW")
ax.bar(x + width / 2, query_means[1], width, yerr = query_stdevs[1], label = "Zeno")

ax.set_ylim(bottom = 0)
ax.set_ylabel("Time (seconds)")
ax.set_xlabel("Number of Vertices per .obj file")
ax.set_title("Average Time for 10M Closest Point Queries")
ax.set_xticks(x, obj_files)
ax.legend()

fig.tight_layout()
plt.savefig("query.png")

# bar plot for ratio query times (zeno to fcpw)
ratio = [i / j for (i, j) in zip(query_means[1], query_means[0])]

fig, ax = plt.subplots()
ax.bar(x, ratio, width)

ax.set_ylim(bottom = 0)
ax.set_ylabel("Ratio (Zeno to FCPW)")
ax.set_xlabel(".obj files")
ax.set_title("Ratio of Average Query Times")
ax.set_xticks(x, obj_files)

fig.tight_layout()
plt.savefig("ratio.png")
