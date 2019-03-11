# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
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
fig, axes = plt.subplots(ncols=len(sizes),nrows=1, figsize=[12,3.5], sharex=True, sharey=True)

for s in range(len(sizes)):
    folder='comp8bit0to4/103wp'+str(sizes[s])+'m10/'
    mat = [[[] for y in range(16)] for x in range(16)]
    for nb in range(start+1,start+qt+1):
        try:
            with open(folder+filename+str(nb)) as f:
                print(nb)
                line = f.readlines()[-1]
                l = line.strip()
                l = line.split(' ')
            
                pos = []
                t = int(l[0])
        
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
                            x, y = convertWordToIdx(agent[2][0])
                            mat[y][x].append(len(clust)/sizes[s])
                         
                    pos = [p for p in pos if p not in clust]
                                                          
                        
        except IOError:
            print("No file:",folder+filename+str(nb))   

    for i in range(16):
        for j in range(16):
            if(len(mat[i][j])==0):
                mat[i][j] = 0
            else:
                mat[i][j] = mean(mat[i][j])
    npMat = np.matrix(mat)
    heatmap=axes[s].imshow(npMat, cmap='gray', interpolation='none', vmin=0,vmax=1)

    axes[s].set_title("N="+str(sizes[s]))
    axes[s].set_xlabel("a")
    axes[s].set_ylabel("b",rotation=0)
    axes[s].tick_params(axis='both',labelsize=9)
    axes[s].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.2f')%(x*0.25 + 0.25)))
    axes[s].yaxis.set_major_formatter(ticker.FuncFormatter(lambda y, pos: ('%.2f')%((15-y)*0.25 + 0.25)))
    fig.show()
    #heatmap = axes[s].pcolor(npMat)
    #axes[s].draw_artist(axes[s].patch)
    #axes[s].draw_artist(heatmap)
    #fig.canvas.blit(axes[s].bbox)
    #fig.canvas.flush_events()
    #t_end = time.time()
    #print("fps = {0}".format(999 if t_end == t_start else 1/(t_end-t_start)))
