# Libraries
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from collections import Counter
from mpl_toolkits.mplot3d import Axes3D

directory='highConstDensity2'
start=1
qt=20
time=30000
sizes=[25,50,100]
fig = plt.figure(figsize=plt.figaspect(1.0/len(sizes)))
i=0
aParam = range(0,625,25)
bParam = range(0,625,25)
aLength=len(aParam)
bLength=len(bParam)
dim=(bLength,aLength)
for s in sizes:
    mat = np.zeros(dim)
    aKey = 0
    for a in aParam:
        bKey = bLength - 1
        for b in bParam:
            temp = 0
            
            folder = directory+"/101p"+str(s)+"a"+str(a)+"b"+str(b)
            try:
                for nb in range(start,start+qt):
                    with open(folder+'/stat_'+str(nb)) as f:
                        lines = f.readlines()
                        line = lines[-1]
                        arr = np.fromstring(line, dtype=float, sep=' ')
                        tempX = arr[0]
                        if(tempX==time):
                            temp += arr[4]/qt
            except:
                print("No file: "+folder+'/stat_'+str(nb))
            mat[bKey,aKey] = temp
            bKey -= 1
        aKey += 1

    npMat = np.matrix(mat)
    ax = fig.add_subplot(1, len(sizes), i+1)#, projection='3d')
    #(x, y) = np.meshgrid(np.arange(npMat.shape[0]), np.arange(npMat.shape[1]))
    #cb = ax.plot_surface(x,y, npMat, cmap=plt.cm.coolwarm,vmin=0.075, vmax=0.941)
    orig_cmap = mpl.cm.gray
    cb=ax.imshow(npMat, cmap='gray', interpolation='none',vmin=0,vmax=1)
    ax.set_title("N="+str(s))
    ax.set_xlabel("a")
    ax.set_ylabel("b",rotation=0)

    #ax.xaxis.set_ticks(np.arange(min(aParam)/100.0, max(aParam)/100.0+0.25, 0.25))
    ax.tick_params(axis='both',labelsize=9)
    ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.0f')%(x*0.25)))
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda y, pos: ('%.0f')%((bLength-1-y)*0.25)))
    ax.xaxis.set_major_locator(ticker.MultipleLocator(4))
    ax.yaxis.set_major_locator(ticker.MultipleLocator(4))
    #ax.invert_xaxis()
    #ax.invert_yaxis()
    i+=1
    
    maxi = 0
    maxPara = (0,0)
    mini = 1
    minPara = (0,0)
    bKey = bLength-1
    for b in bParam:
        #print(max(mat[bKey]))
        aKey = 0
        for a in aParam:
            if(mat[bKey,aKey]>maxi):
                maxi=mat[bKey,aKey]
                maxPara=(a,b)
            if(mat[bKey,aKey]<mini):
                mini=mat[bKey,aKey]
                minPara=(a,b)
            aKey+=1
        bKey-=1
    print("max N="+str(s),maxi,maxPara)
    print("min N="+str(s),mini,minPara)

cbar_ax = fig.add_axes([0.92, 0.1, 0.02, 0.8])
fig.colorbar(cb,cax=cbar_ax)
plt.savefig(directory+'/brute_force.png',bbox_inches='tight')
plt.show()

