# Libraries
import numpy as np
import math

start=1
qt=100
#time=300000

links = [101]
sizes = [25,50,100,200]
#aRange = list(range(0,625,25))
#bRange = list(range(0,625,25))
aRange = [200]
bRange = [15000]
mRange = [1,10]

filename = 'out_'
newFilename = 'stat_'

dist = 1

def makeCluster(seed, pos, manhattan):
    cluster = [seed]
    #print(pos)
    for p in range(len(pos)):
        if(manhattan):
            d = abs(seed[0]-pos[p][0]) + abs(seed[1]-pos[p][1])#math.sqrt(math.pow(seed[0]-pos[p][0],2)+math.pow(seed[1]-pos[p][1],2))
        else:
            d = max(abs(seed[0]-pos[p][0]),abs(seed[1]-pos[p][1]))
        #print(d)
        if(d <= dist and pos[p] not in cluster):
            cluster.extend(makeCluster(pos[p],pos[:p]+pos[p+1:],manhattan))

    return cluster

def variation(cluster):
    meanpos = [0.0,0.0]
    for pos in cluster:
        meanpos[0]+=pos[0]
        meanpos[1]+=pos[1]
    meanpos[0]/=len(cluster)
    meanpos[1]/=len(cluster)
    var = 0
    for pos in cluster:
        var += pow(pos[0]-meanpos[0],2) + pow(pos[1]-meanpos[1],2)
    #we don't need to divide by 4r**2 as that's 1 here
    return var/len(cluster)

def process1(directory,link,s,a,b,c):
    folder = directory+'/'+str(link)+'p'+str(s)+'a'+str(a)+'b'+str(b)+'c'+str(c)+'/'
    processFolder(folder,s)

def process2(directory,link,s,m):
    folder = directory+'/'+str(link)+'wp'+str(s)+'m'+str(m)+'/'
    processFolder(folder,s)

def process3(directory,link,s):
    folder = directory+'/'+str(link)+'p'+str(s)+'/'
    processFolder(folder,s)

def processOpti(directory,link,s):
    folder = directory+'/'+str(link)+'p'+str(s)+'opti/'
    processFolder(folder,s)

def processOpti25(directory,link,s):
    folder = directory+'/'+str(link)+'p'+str(s)+'opti25/'
    processFolder(folder,s)

def processFolder(folder,s):
    print(folder)
    tot = 0
    for nb in range(start,start+qt):
        try:
            f = open(folder+filename+str(nb))
            lines = f.readlines()
            line = lines[-1]
            #l = line.strip()
            l = line.split(' ')

            #MANHATTAN DIST
            pos = []
            words = set()
            t = int(l[0])
            for i in range(1,len(l)):
                members = l[i].split('?')
                coord = members[0].split(',')
                pos.append((float(coord[0]),float(coord[1])))

            clusters = []
            while(len(pos)>0):
                clust = makeCluster(pos[0],pos[1:],True)
                if(len(clust)>1):
                    clusters.append(clust)
                pos = [p for p in pos if p not in clust]

            #CHEBYSHEV DIST
            pos2 = []
            words = set()
            t = int(l[0])
            for i in range(1,len(l)):
                members = l[i].split('?')
                coord = members[0].split(',')
                pos2.append((float(coord[0]),float(coord[1])))

            clusters2 = []
            while(len(pos2)>0):
                clust = makeCluster(pos2[0],pos2[1:],False)
                if(len(clust)>1):
                    clusters2.append(clust)
                pos2 = [p for p in pos2 if p not in clust]

            lengths=[len(c) for c in clusters]
            lengths.append(0)

            lengths2=[len(c) for c in clusters2]
            lengths2.append(0)
            tot += max(lengths) - max(lengths2)
            f.close()
                        
        except IOError:
            print("No file:",folder+filename+str(nb))
    print(tot/100)

def makeStats(directory):
    for link in links:
        for s in sizes:
            if(link==103):
                for m in mRange:
                    process2(directory,link,s,m)
            elif(link==104):
                process3(directory,link,s)
            else:
                for a in aRange:
                    for b in bRange:
                        process1(directory,link,s,a,b)

#Don't forget to comment that bit after use
# or it will activate each time stats is imported (e.g. word_scatter or word_map)
#dirs = ["highConstDensity2"]
#for d in dirs:
#    makeStats(d)
