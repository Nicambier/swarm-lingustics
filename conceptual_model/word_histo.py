# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from scipy.stats import ranksums
from mpl_toolkits import mplot3d
from stats import makeCluster

def most_frequent(List): 
    dict = {} 
    count, itm = 0, '' 
    for item in reversed(List): 
        dict[item] = dict.get(item, 0) + 1
        if dict[item] >= count : 
            count, itm = dict[item], item 
    return(itm, count) 

directory='com_range1_8n/highDensity2/103wpSIZEm10'
time=300000
window=300000

start = 1
qt = 100
nbWords = 15

sizes=[25,50,100,200]

fig = plt.figure(figsize=(17,8))
for s in range(len(sizes)):
    a = []
    b = []
    c = []
    wordMap = {}
    for iteration in range(start,start+qt):
        folder = directory.replace("SIZE",str(sizes[s]))
        filename = folder+"/out_"
        print(sizes[s],iteration)
        try:
            with open(filename+str(iteration)) as f:
                lines = f.readlines()
                for line in lines:
                    words = []
                    l = line.strip()
                    l = line.split(' ')
                
                    t = int(l[0])
                    if(t>=time-window):
                        pos = []
                        for i in range(1,len(l)):
                            members = l[i].split('?')
                            tempWords = [int(w) for w in members[1].split(',')]
                            words.extend(tempWords)
                            coord = members[0].split(',')
                            pos.append((float(coord[0]),float(coord[1])))
                        w,count = most_frequent(words)
                        if(count/sizes[s]>=0.95):
                            len_clusters = []
                            while(len(pos)>0):
                                clust = makeCluster(pos[0],pos[1:])
                                if(len(clust)>1):
                                    len_clusters.append(len(clust))
                                pos = [p for p in pos if p not in clust]
                            if(max(len_clusters)/sizes[s] >= 0.95):
                                if(w in wordMap):
                                    wordMap[w] +=1                                        
                                else:
                                    wordMap[w]=1
                            

        except IOError:
            print("No file:",filename+str(iteration))

    sorted_WM = [(k,v) for k, v in sorted(wordMap.items(), key=lambda item: item[1], reverse=True)]
    print(len(sorted_WM))
    for i in range(min(len(sorted_WM),nbWords)):
        w = sorted_WM[i][0]
        a.append(0.02*((w >> 8)&255))
        b.append(0.02*(w & 255))
        c.append(0.0039*((w >> 16)&255))
        print(w,round(a[-1],2),round(b[-1],2),round(c[-1],2),sorted_WM[i][1])
    
    ax = fig.add_subplot(2,4,s+1,projection='3d')
    cols = np.array([[255, 0, 0]]+[[0, 0, 255]]*(nbWords-1))
    ax.scatter(a,b,c, c=cols/255.0, depthshade=False)
    ax.set_xlim([0,5])
    ax.set_ylim([0,5])
    ax.set_zlim([0,1])
    ax.set_xlabel('a')
    ax.set_ylabel('b')
    ax.set_zlabel('c')
    ax.view_init(azim=0, elev=60)
    ax.set_title('N='+str(sizes[s]))

    sorted_vals = [v for (k,v) in sorted_WM] 
    ax2 = fig.add_subplot(2,4,4+s+1)
    ax2.plot(range(len(sorted_vals)),sorted_vals)
    ax2.set_xlabel("word rank")
    ax2.set_ylabel("frequency")

    

#plt.savefig(directory+'/word_histo_"'+str(iter)+'.png',additional_artists=art,bbox_inches='tight')
plt.show()

