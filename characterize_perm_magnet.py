from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

DATA = Path(__file__).parent / "data/perm.csv"

data = {}
curr_key = None
with open(DATA, 'r') as file:
    for line in file.readlines():
        if not line[0].isdigit():
            if curr_key:
                data[curr_key] = np.array(data[curr_key])
                plt.plot(data[curr_key][:,0], data[curr_key][:, 1])
                plt.title(curr_key.strip("metadata,"))
            data[line.strip("\n")] = []
            curr_key = line.strip("\n")
        else:
            data[curr_key].append([int(x) for x in line.strip("\n").split(",")])

data[curr_key] = np.array(data[curr_key])
    
plt.plot(data[curr_key][:,0], data[curr_key][:,1])
plt.title(curr_key.strip("metadata,"))
plt.savefig(Path(__file__).parent / f"imgs/perm_magnet.jpg")
