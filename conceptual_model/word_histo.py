# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from scipy.stats import ranksums

directory='com_range1_8n/highDensity2/103wpSIZEm10'
time=300000
window=10000

start = 1
qt = 100

sizes=[25,50,100,200]

fig, axes = plt.subplots(ncols=len(sizes), nrows=3, figsize=[16,5],sharex=True)
for s in range(len(sizes)):
    words = []
    for iteration in range(start,start+qt):
        folder = directory.replace("SIZE",str(sizes[s]))
        filename = folder+"/out_"
        print(sizes[s],iteration)
        try:
            with open(filename+str(iteration)) as f:
                lines = f.readlines()
                for line in lines:
                    l = line.strip()
                    l = line.split(' ')
                
                    t = int(l[0])
                    if(t>=time-window):
                        words.append([])
                        for i in range(1,len(l)):
                            members = l[i].split('?')
                            for w in members[1].split(','):
                                words[-1].append(int(w))

        except IOError:
            print("No file:",filename+str(iteration))

    total_a = []
    total_b = []
    total_c = []
    #occurencies = []
    for timestep in words:
        for w in timestep:
            total_a.append((w>>8) & 255)
            total_b.append(w & 255)
            total_c.append((w>>16) & 255)
        #total.extend(timestep)
        #occurencies.extend(list(set(timestep)))
    y1,x1,z1=axes[0,s].hist(total_a, bins=255)
    y1,x1,z1=axes[1,s].hist(total_b, bins=255)
    y1,x1,z1=axes[2,s].hist(total_c, bins=255)

    

#plt.savefig(directory+'/word_histo_"'+str(iter)+'.png',additional_artists=art,bbox_inches='tight')
plt.show()

