import sys, os, re, subprocess, multiprocessing as mp, statistics as stat

# python3 benchmark.py [obj file] [queryType] [# of queries] [outfile (optional)]
try :
    outfile = sys.argv[4]
except :
    outfile = "benchmark.csv"

# multiprocessing function
def job(cmd) :
    process = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE)
    out = process.communicate()[0].decode("utf-8")  # get output from process, decode to string

    # return second and third numbers of stdout (preprocessing and benchmark times)
    return [float(re.findall("\d+.\d+", out)[1]), float(re.findall("\d+.\d+", out)[2])]

# fcpw processes
print("Processing FCPW commands...")
fcpw_cmd = "./fcpw.exe " + sys.argv[1] + " " + sys.argv[2] + " true 1729 " + sys.argv[3]

p = mp.Pool(mp.cpu_count())
fcpw_data = p.map(job, [fcpw_cmd for i in range(15)])
p.close()

# zeno processes
print("Processing ZENO commands...")
zeno_cmd = "./zeno.exe " + sys.argv[1] + " " + sys.argv[2] + " true 1729 " + sys.argv[3]

p = mp.Pool(mp.cpu_count())
zeno_data = p.map(job, [zeno_cmd for i in range(15)])
p.close()

# write stats to csv
print("Writing statistics to", outfile)

with open(outfile, "w") as csv :
    csv.write("# MEAN, MEDIAN, MIN, MAX, STDEV\n")    # comment clarifying csv entries

    def write_stats(data, comment) :
        csv.write(comment)
        csv.write(str(stat.mean(data)))
        csv.write(", " + str(stat.median(data)))
        csv.write(", " + str(min(data)))
        csv.write(", " + str(max(data)))
        csv.write(", " + str(stat.stdev(data)) + "\n")

    write_stats(fcpw_data[:][0], "\n# FCPW Preprocessing Times\n")
    write_stats(fcpw_data[:][1], "\n# FCPW Benchmark Times\n")
    write_stats(zeno_data[:][0], "\n# ZENO Preprocessing Times\n")
    write_stats(zeno_data[:][1], "\n# ZENO Benchmark Times\n")
