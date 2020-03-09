import numpy as np
from os import listdir
from os.path import isfile, join

def parse_dir(dirname):
  db = dict()
  for filename in sorted(listdir(dirname)):
    fullname = join(dirname, filename)
    if not isfile(fullname):
      continue
    for line in open(fullname).readlines()[1:]:
      tokens = map(lambda x: x.strip(), line.split(", "))
      key = (int(tokens[0]), int(tokens[1]), int(tokens[2]))
      val = float(tokens[3])
      if key not in db:
        db[key] = list()
      db[key].append(val)
  return db

db_a = parse_dir("/tmp/sgemm_v0")
db_b = parse_dir("/tmp/sgemm_v1")
print("%4s, %4s, %4s, %15s, %15s, %15s, %15s, %15s" % ("m", "n", "k", "mkl_gflops", "mkl_std", "dnnl_gflops", "dnnl_std", "dnnl_%speedup"))
for key in sorted(db_a.keys()):
  a_list = db_a[key]
  b_list = db_b[key]
  a = np.median(a_list)
  b = np.median(b_list)
  a_std = np.std(a_list)
  b_std = np.std(b_list)
  speedup = (b - a) / a * 100;
  print("%4d, %4d, %4d, %15.4f, %15.4f, %15.4f, %15.4f, %15.4f" % (key[0], key[1], key[2], a, a_std, b, b_std, speedup))
