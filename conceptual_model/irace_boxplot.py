# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import random as rand
from scipy.stats import ranksums

def set_box_color(bp, color):
    plt.setp(bp['boxes'], color=color)
    plt.setp(bp['whiskers'], color=color)
    plt.setp(bp['caps'], color=color)
    plt.setp(bp['medians'], color=color)

folder = 'com_range1_8n/'
directory=folder+'iraceOpti/'
start=1
qt=10

sizes = [25,50,100,200]
dens = ["high","low"]
dens_labels = ["High density","Low density"]

col = ['#F5793A','#A95AA1']

perf = {}

for s in sizes:
    for d in dens:
        optiParam = []
        for i in range(start,start+qt):
            if(i==start):
                perf[(s,d)] = []
            final = 0
            try:
                counterToBest = -1
                bestParam = (None,None,None)
                f = open(directory+"out"+str(s)+"_"+d+str(i))
                for line in f.readlines():
                    if(counterToBest>=0):
                        counterToBest += 1
                        if(counterToBest == 3):
                            tempLine = line.split()
                            counterToBest = -1
                            bestParam = (tempLine[2],tempLine[3],tempLine[4])
                    spl = line.split(':')  
                    if("Best-so-far configuration" in spl[0]):
                        final = 1-float(spl[2])
                        counterToBest = 0
                perf[(s,d)].append(final)
                optiParam.append(bestParam)
                #print(s,d,perf[(s,d)][-1],i,optiParam[-1])
            except Exception as e:
                print(e," ",s,d,i,bestParam)
        #print(optiParam)
        bestIdx = perf[(s,d)].index(max(perf[(s,d)]))
        print(s,d,perf[(s,d)][bestIdx],optiParam[bestIdx])

for k in range(len(sizes)):
    pos = (k*3)+1
    bp1 = plt.boxplot(perf[(sizes[k],dens[0])],positions = [pos], widths = 0.6,whis=20)
    bp2 = plt.boxplot(perf[(sizes[k],dens[1])],positions = [pos+1], widths = 0.6,whis=20)
    set_box_color(bp1, col[0])
    set_box_color(bp2, col[1])
    #plt.scatter(pos, perf[(sizes[k],dens[0])][0], color=col[0])
    #plt.scatter(pos+1, perf[(sizes[k],dens[1])][0], color=col[1])

plt.axes().set_xlabel("N")
plt.axes().set_ylabel("Cluster metric")

plt.axes().set_xticklabels(sizes)
plt.axes().set_xticks([1.5+ (3*x) for x in range(len(sizes))])
plt.xlim(0,3*len(sizes))
plt.ylim(0.5,1.01)

hB, = plt.plot([1,1],'-',color = col[0])
hR, = plt.plot([1,1],'-',color = col[1])
plt.legend([hB, hR],dens_labels)
hB.set_visible(False)
hR.set_visible(False)

#plt.show()
plt.savefig(folder+'irace_compare.png')
