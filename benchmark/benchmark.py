import sys, os, re, subprocess, multiprocessing as mp, statistics as stat

# python3 benchmark.py [obj file] [queryType] [# of queries] [outfile (optional)]
try :
    outfile = "outfiles/" + sys.argv[4]
except :
    outfile = "outfiles/benchmark.csv"

# multiprocessing function
def job(cmd) :
    process = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE)
    out = process.communicate()[0].decode("utf-8")  # get output from process, decode to string

    results = re.findall("\d+.\d+", out)
    triangle_count, vertex_count = re.findall("\d+", out)[-2], re.findall("\d+", out)[-1]

    return [float(results[0]), float(results[1]), int(triangle_count), int(vertex_count)]

# fcpw processes
print("Processing FCPW commands...")
fcpw_cmd = "./fcpw.exe " + sys.argv[1] + " " + sys.argv[2] + " 1729 " + sys.argv[3] + "false"

p = mp.Pool(mp.cpu_count())
fcpw_data = p.map(job, [fcpw_cmd for i in range(15)])
p.close()

# zeno processes
print("Processing ZENO commands...")
zeno_cmd = "./zeno.exe " + sys.argv[1] + " " + sys.argv[2] + " 1729 " + sys.argv[3] + "false"

p = mp.Pool(mp.cpu_count())
zeno_data = p.map(job, [zeno_cmd for i in range(15)])
p.close()

# write stats to csv
print("Writing statistics to", outfile)

with open(outfile, "w") as csv :
    csv.write("# MEAN, MEDIAN, MIN, MAX, STDEV\n")    # comment clarifying csv entries

    def write_stats(data, comment) :
        csv.write(comment)
        csv.write(str(f'{stat.mean(data):.8f}'))
        csv.write(", " + str(f'{stat.median(data):.8f}'))
        csv.write(", " + str(f'{min(data):.8f}'))
        csv.write(", " + str(f'{max(data):.8f}'))
        csv.write(", " + str(f'{stat.stdev(data):.8f}') + "\n")

    write_stats([i[0] for i in fcpw_data], "\n# FCPW Preprocessing Times\n")
    write_stats([i[1] for i in fcpw_data], "\n# FCPW Benchmark Times\n")
    write_stats([i[0] for i in zeno_data], "\n# ZENO Preprocessing Times\n")
    write_stats([i[1] for i in zeno_data], "\n# ZENO Benchmark Times\n")

    # adding vertex count to csv
    csv.write("\n# Vertices in this .obj file\n" + str(fcpw_data[0][3]) + "\n")
    csv.write("\n# Triangles in this model\n" + str(fcpw_data[0][2]) + "\n")
