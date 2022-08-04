import os, sys, pandas as pd, numpy as np, subprocess, math, matplotlib.pyplot as plt

# python3 [.obj files] [queryType]
df = pd.DataFrame({".obj files" : os.listdir(sys.argv[1])})

df["FCPW Preprocessing"] = np.nan
df["FCPW Query Mean"] = np.nan
df["FCPW Query Stdev"] = np.nan
df["Zeno Preprocessing"] = np.nan
df["Zeno Query Mean"] = np.nan
df["Zeno Query Stdev"] = np.nan
df["Triangle Count"] = np.nan
df["Vertex Count"] = np.nan

df.set_index(".obj files", inplace = True)
df.index.names = [None]

n = math.sqrt(15) # for computing avg stdev

for obj_file in df.index :
    print("Fetching", obj_file, "benchmarks...")
    subprocess.call("python3 benchmark.py ../obj_files/" + obj_file + " " + sys.argv[2] + " 10000000", shell = True, stdout = subprocess.PIPE)

    # get benchmarks from subprocess
    with open("outfiles/benchmark.csv", "r") as csv :
        lines = csv.readlines()
        fcpw_prep = lines[3].split(", "); zeno_prep = lines[9].split(", ")
        fcpw_query = lines[6].split(", "); zeno_query = lines[12].split(", ")

        df.loc[obj_file] = [float(fcpw_prep[0]), float(fcpw_query[0]), float(fcpw_query[4]) / n,
                        float(zeno_prep[0]), float(zeno_query[0]), float(zeno_query[4]) / n,
                        int(lines[18]), int(lines[15])]

# sort rows by triangle count
df.sort_values(by = ["Vertex Count"], inplace = True)

# double bar plot for average preprocessing times
ax = df.plot(y = ["FCPW Preprocessing", "Zeno Preprocessing"], label = ["FCPW", "Zeno"], kind = "bar")
ax.set_ylabel("Time (seconds)")
ax.set_xlabel(".obj files")
ax.tick_params("x", rotation = 60)
ax.set_title("Average FCPW & Zeno Preprocessing Times")
ax.get_figure().tight_layout()
plt.savefig("preprocessing.png")

# double bar plot for average query times
ax = df.plot(y = ["FCPW Query Mean", "Zeno Query Mean"], label = ["FCPW", "Zeno"], yerr = [df["FCPW Query Stdev"], df["Zeno Query Stdev"]], kind = "bar")
ax.set_ylabel("Time (seconds)")
ax.set_xlabel(".obj file")
ax.tick_params("x", rotation = 60)
ax.set_title("Average Time for 10M Closest Point Queries")
ax.get_figure().tight_layout()
plt.savefig("query.png")

# ratio (Zeno to FCPW)
x = np.arange(len(df.index))
ratio = [row["Zeno Query Mean"] / row["FCPW Query Mean"] for _, row in df.iterrows()]

fig, ax = plt.subplots()
ax.bar(x, ratio, width = 0.5)
ax.set_ylim(bottom = 0)
ax.set_ylabel("Ratio (Zeno to FCPW)")
ax.set_xlabel(".obj files")
ax.tick_params("x", rotation = 60)
ax.set_title("Speed Up of Average Query Times")
ax.set_xticks(x, df.index)

fig.tight_layout()
plt.savefig("ratio.png")

# save data
with open("data.txt", "w") as txt :
    txt.write("Preprocessing:\n")
    txt.write(str(df["FCPW Preprocessing"]) + "\n")
    txt.write(str(df["Zeno Preprocessing"]) + "\n")

    txt.write("\nQueries:\n")
    txt.write(str(df["FCPW Query Mean"]) + "\n")
    txt.write(str(df["FCPW Query Stdev"]) + "\n")
    txt.write(str(df["Zeno Query Mean"]) + "\n")
    txt.write(str(df["Zeno Query Stdev"]) + "\n") 
    
    txt.write("\n.obj files:\n")
    for i, row in df.iterrows() :
        txt.write(": Vertices (" + str(row["Vertex Count"]) + "), Triangles (" + str(row["Triangle Count"]) + ")\n")
