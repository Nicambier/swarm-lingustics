# Libraries
import numpy as np
import math

directory='comp8bit0to4'
start=21
qt=30
time=30000

links = [103]
sizes = [25,50,100]
aRange = list(range(175,200,25))
bRange = list(range(400,425,25))
mRange = [10]

filename = 'out_'
newFilename = 'stat_'

dist = 2

def makeCluster(seed, pos):
    cluster = [seed]
    #print(pos)
    for p in range(len(pos)):
        d = abs(seed[0]-pos[p][0]) + abs(seed[1]-pos[p][1])#math.sqrt(math.pow(seed[0]-pos[p][0],2)+math.pow(seed[1]-pos[p][1],2))
        #print(d)
        if(d <= dist and pos[p] not in cluster):
            cluster.extend(makeCluster(pos[p],pos[:p]+pos[p+1:]))

    return cluster

def process1(link,s,a,b):
    folder = directory+'/'+str(link)+'p'+str(s)+'a'+str(a)+'b'+str(b)+'/'
    processFolder(folder,s)

def process2(link,s,m):
    folder = directory+'/'+str(link)+'wp'+str(s)+'m'+str(m)+'/'
    processFolder(folder,s)

def process3(link,s):
    folder = directory+'/'+str(link)+'p'+str(s)+'/'
    processFolder(folder,s)

def processFolder(folder,s):
    print(folder)
    for nb in range(start,start+qt):
        try:
            with open(folder+filename+str(nb)) as f:
                f2 = open(folder+newFilename+str(nb), "w")
                print(nb)
                lines = f.readlines()
                for line in lines:
                    l = line.strip()
                    l = line.split(' ')
                
                    pos = []
                    words = set()
                    t = int(l[0])
            
                    for i in range(1,len(l)):
                        members = l[i].split('?')
                        coord = members[0].split(',')
                        pos.append((float(coord[0]),float(coord[1])))
                        if(len(members)>1):
                            words |= set([int(i) for i in members[1].split(',')])
##                    if(0 in words):
##                        words.remove(0)

                    clusters = []
                    while(len(pos)>0):
                        clust = makeCluster(pos[0],pos[1:])
                        if(len(clust)>1):
                            clusters.append(clust)
                        pos = [p for p in pos if p not in clust]

                    lengths=[len(c) for c in clusters]
                    lengths.append(0)
                    print(t,len(clusters),len(words),round(np.mean(lengths),3),max(lengths)/s,s-sum(lengths),sep='\t',file=f2)
                f2.close()
                        
        except IOError:
            print("No file:",folder+filename+str(nb))    

def makeStats():
    for link in links:
        for s in sizes:
            if(link==103):
                for m in mRange:
                    process2(link,s,m)
            elif(link==104):
                process3(link,s)
            else:
                for a in aRange:
                    for b in bRange:
                        process1(link,s,a,b)
