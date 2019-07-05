# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from scipy.stats import ranksums

directory='old_stuff/lowConstDensity'
start=1
qt=20
time=30000

boxplot = False

#nValues=[0.001]
sizes=[25,50,100]

fig, axes = plt.subplots(ncols=3, nrows=2, figsize=[12,5],sharex=True)
#axes[2].set_ylabel('#Words')
axes[1][0].set_xlabel('Time-steps')
axes[1][1].set_xlabel('Time-steps')
axes[1][2].set_xlabel('Time-steps')

axes[1][0].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.dk')%(x*1e-3)))
axes[1][1].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.dk')%(x*1e-3)))
axes[1][2].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.dk')%(x*1e-3)))

#col=["blue","cyan","red","pink","grey","green","orange"]

if(boxplot):
    col=["blue","red","green"]
    lab=["Evolving (weak) m=0.01","Evolving 16 bits (weak) m=0.01","Optimal (for N=25)"]
    folders = ['../test_NGturns/103wpSIZEm10/','103wpSIZEm10_16bit/','101pSIZEopti25/']
else:
    col=["red","grey","green","orange"]
    lab=["Evolving 16 bits (weak) m=0.01","Random","Optimal","Optimal (for N=25)"]
    folders = ['103wpSIZEm10_16bit/','104pSIZE/','101pSIZEopti/','101pSIZEopti25/']

dummyLines = [plt.Line2D([],[],color="black",linestyle="-"),plt.Line2D([],[],color="black",linestyle=":")]

def drawFolder(folder,i,size,color):
    x = list(range(0,time+100,200))
    y1 = [[]]*len(x)
    y2 = [[]]*len(x)
    y3 = [[]]*len(x)
    w = [[]]*len(x)
    
    for nb in range(start,start+qt):
        try:
            with open(folder+'stat_'+str(nb)) as f:
                lines = f.readlines()
                c = 0
                for line in lines:
                    arr = np.fromstring(line, dtype=float, sep='\t')
                    tempX = arr[0]
                    #print(tempX%1000==0)
                    if(tempX<=time and tempX%200 == 0):
                        w[c] = w[c]+[arr[2]]
                        y1[c] = y1[c]+[arr[1]]
                        y2[c] = y2[c]+[arr[4]]
                        y3[c] = y3[c]+[arr[5]]
                        c+=1
                        
                        
        except Exception as e:
            print(e," ",folder+'stat_'+str(nb))

    a = []
    b = []
    b_err = []
    b_med = []
    b_Q1 = []
    b_Q3 = []
    c = []
    wa = []
    #print(y2[3])

    for z in range(len(y1)):
        a.append(np.mean(y1[z]))
        b.append(np.mean(y2[z]))
        b_err.append(np.std(y2[z]))
        b_med.append(np.median(y2[z]))
        b_Q1.append(np.percentile(y2[z],25))
        b_Q3.append(np.percentile(y2[z],75))
        c.append(np.mean(y3[z]))
        wa.append(np.mean(w[z]))

    x=np.array(sorted(x))
    a=np.array(a)
    b=np.array(b)
    b_err=np.array(b_err)
    b_med = np.array(b_med)
    b_Q1 = np.array(b_Q1)
    b_Q3 = np.array(b_Q3)
    c=np.array(c)
    wa=np.array(wa)

    axes[0][i].set_title("N="+str(sizes[s]))
    if(not boxplot):
        axes[0][i].plot(x,b,color=color,linestyle="-")
    else:
        axes[0][i].plot(x,b_med,color=color,linestyle="-")
        axes[0][i].fill_between(x, b_Q1, b_Q3, alpha=0.2, facecolor=color)
    
    axes[1][i].plot(x,a,color=color,linestyle="-")
    axes[1][i].plot(x,c,color=color,linestyle=":")
    #axes[1][i].plot(x,wa,color=color,linestyle="--")
    axes[0][i].set_ylim([0,1])
    axes[1][i].set_ylim([0,size/2])

    return np.array(y2[-1])

final_arrays = {}
for f in range(len(folders)):
    final_arrays[folders[f]] = []
    for s in range(len(sizes)):
        folder = directory+'/'+folders[f].replace('SIZE',str(sizes[s]))
        final_arrays[folders[f]].append(drawFolder(folder,s,sizes[s],col[f]))

print("Wilcoxon rank-sum test (",folders[0],">","101pSIZEopti25/ )")
for s in range(len(sizes)):
    print("N=",sizes[s],":",ranksums(final_arrays[folders[0]][s], final_arrays['101pSIZEopti25/'][s]))
    #print(final_arrays['103wpSIZEm10/'][s], final_arrays['101pSIZEopti25/'][s],sep='\n')

axes[0][0].set_ylabel('Cluster metric')
axes[1][0].set_ylabel('Quantity')
axes[1][2].legend(dummyLines,["Clusters","Free Agents"],loc='lower left', bbox_to_anchor=(0.55, 0.75), ncol=1, fancybox=True, shadow=True, fontsize=10)
lgd = axes[1][1].legend(axes[0][0].get_lines(),lab,loc='lower right', bbox_to_anchor=(1.6, -0.5), ncol=3, fancybox=True, shadow=True, fontsize=12)
art=[lgd]
##if(boxplot):
##    plt.savefig(directory+'/timeEvol_boxplot.png',additional_artists=art,bbox_inches='tight')
##else:
##    plt.savefig(directory+'/timeEvol.png',additional_artists=art,bbox_inches='tight')


plt.show()

