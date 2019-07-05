# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from mpl_toolkits.mplot3d import Axes3D
import time
from collections import Counter
from statistics import mean
from stats import makeCluster

def convertWordToParam(word):
    a = 0.25 + 0.25*(word//16)
    b = 0.25 + 0.25*(word%16)
    return a,b

def convertParamToWord(a,b):
    return ((a-0.25)*4)*16 + ((b-0.25)*4)

def convertWordToIdx(word):
    i = word//16
    j = 15-(word%16)
    return i,j

def convertIdxToWord(i,j):
    return i*16 + j

filename="out_"
start=0
qt=50
maxTime=30000
sizes = [25,50,100]
fig = plt.figure(figsize=(20, 5))

grid_place=1
for size in sizes:
    folder='comp8bit0to4/103wp'+str(size)+'m10/'
    a_list = []
    b_list = []
    perf_list = []
    time_list = []
    for nb in range(start+1,start+qt+1):
        try:
            with open(folder+filename+str(nb)) as f:
                print(nb)
                lines = f.readlines()
                for line in lines:
                    l = line.strip()
                    l = l.split(' ')
                
                    pos = []
                    t = int(l[0])
                    if(t%2000==0):
                        for i in range(1,len(l)):
                            members = l[i].split('?')
                            coord = members[0].split(',')
                            pos.append((float(coord[0]),float(coord[1]),[int(i) for i in members[1].split(',')]))
                            
                        clusters = []
                        while(len(pos)>0):
                            clust = makeCluster(pos[0],pos[1:])
                            if(len(clust)>1):
                                words = set()
                                for agent in clust:
                                    words |= set(agent[2])
                                    #print(nb,words, len(clust))
                                for w in words:
                                    a, b = convertWordToParam(agent[2][0])
                                    a_list.append(a)
                                    b_list.append(b)
                                    perf_list.append(len(clust)/size)
                                    time_list.append(t)
                                    
                            #delete agents in the cluster from list of free agents
                            pos = [p for p in pos if p not in clust]
                                                          
                        
        except IOError:
            print("No file:",folder+filename+str(nb))   

    #data_points = [(x, y, z) for x, y, z in zip(time_list, a_list, b_list)]

    ax = fig.add_subplot(1,3,grid_place, projection='3d')
    t = fig.suptitle('Time-evolution of word selection | cluster metrics', fontsize=14)

    color = [100*x for x in perf_list]
    size = [100 if(t==30000) else 10 for t in time_list]

    ax.scatter(a_list, time_list, b_list, alpha=0.4, s=size, c=color, cmap='jet')

    ax.set_xlabel('a')
    ax.set_ylabel('Time-steps')
    ax.invert_yaxis()
    ax.set_zlabel('b')
    grid_place+=1
    
plt.show()
