# Libraries
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stat

# Create a figure with 9 plot areas
fig, axes = plt.subplots(ncols=3, nrows=2, sharex='all', sharey='all')
fig.patch.set_facecolor('white')
color='dodgerblue'
sty="--"
width=2

axes[0,0].set_title('Aggregates (no NG)')
axes[0,1].set_title('Aggregates (NG)')
axes[0,2].set_title('Words (NG)')

nbExp = 20
folder = "symmetric"

sizes=[20,100]
x = np.array([0,0.5,1,1.5,2,2.5])

for k in range(3):
    col=1
    if k==0:
        algo='101'
    else:
        algo='102'
        if(k>1):
            col=2
            
    for i in range(len(sizes)):
        y = []
        e = []
        for a in range(len(x)):
            spe = 0
            tempArr = []
            tempClustArr = []
            title = algo+'p'+str(sizes[i])+'a'+str(int(x[a]*100))+'b15000'
            for nb in range(0,nbExp):
                f = open(folder+'/'+title+'/stat_'+str(nb+1))
                l = f.readlines()[-1]
                arr = np.fromstring(l, dtype=float, sep=' ')
                t = arr[0]
                if(col!=1 or arr[5]<=0.1):
                    tempArr.append(arr[col])
                else:#if the total # of agents in clusters is < 90%, they are not recognised as clusters
                    tempArr.append(0)

            y.append(np.mean(tempArr))
            e.append(np.std(tempArr))
            print(title,'2nd moment:', arr[6])
##            if(k==2):
##                print("Wilcoxon",title,":",stat.wilcoxon(tempClustArr,tempArr))
##            if(k<2):
##                print("Spread",title,":",np.mean(tempDensArr))
        
        y=np.array(y)
        e=np.array(e)

        axes[i,k].errorbar(x, y, e, color=color, linestyle=sty, dashes=(5,2), marker='o', markeredgecolor=color, linewidth=width, capsize=0)
        axes[i,k].set_ylim([-0.5,4])
        axes[i,k].set_xlim([-0.25,2.75])
        axes[i,k].set_xticks(x)
        axes[i,k].tick_params(labelcolor='black', top='on', bottom='on', left='on', right='on')
        if(i==len(sizes)-1):
            axes[i,k].set_xlabel('a')
        if(k==0):
            axes[i,k].set_ylabel("Quantity ("+str(sizes[i])+' agents)')


plt.show()
