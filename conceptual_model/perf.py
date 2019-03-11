# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

directory='comp8bit0to4'
start=1
qt=50
time=30000

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
col=["red","pink","grey","green","orange"]
lab=["Evolving (weak) m=0.01","Evolving (weak) m=0.001","Random","Optimal"]
folders = ['103wpSIZEm10/','103wpSIZEm1/','104pSIZE/','101pSIZEopti/']
#0=no_NG 2=EE_NG 3=Opti
dummyLines = [plt.Line2D([],[],color="black",linestyle="-"),plt.Line2D([],[],color="black",linestyle=":")]

def drawFolder(folder,i,size,color):
    x = list(range(0,time+100,100))
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
                    if(tempX<=time):
                        w[c] = w[c]+[arr[2]]
                        y1[c] = y1[c]+[arr[1]]
                        y2[c] = y2[c]+[arr[4]]
                        y3[c] = y3[c]+[arr[5]]
                        c+=1
                        
                        
        except Exception as e:
            print(e," ",folder+'stat_'+str(nb))

    a = []
    b = []
    c = []
    wa = []
    #print(y2[3])
    for z in range(len(y1)):
        a.append(np.mean(y1[z]))
        b.append(np.mean(y2[z]))
        c.append(np.mean(y3[z]))
        wa.append(np.mean(w[z]))

    x=np.array(sorted(x))
    a=np.array(a)
    b=np.array(b)
    c=np.array(c)
    wa=np.array(wa)

    axes[0][i].set_title("N="+str(sizes[s]))
    axes[0][i].plot(x,b,color=color,linestyle="-")
    axes[1][i].plot(x,a,color=color,linestyle="-")
    axes[1][i].plot(x,c,color=color,linestyle=":")
    #axes[1][i].plot(x,wa,color=color,linestyle="--")
    axes[0][i].set_ylim([0,1])
    axes[1][i].set_ylim([0,size/2])
    
for f in range(len(folders)):
    for s in range(len(sizes)):
        folder = directory+'/'+folders[f].replace('SIZE',str(sizes[s]))
        drawFolder(folder,s,sizes[s],col[f])

axes[0][0].set_ylabel('Cluster metric')
axes[1][0].set_ylabel('Quantity')
axes[1][2].legend(dummyLines,["Clusters","Free Agents"],loc='lower left', bbox_to_anchor=(0.55, 0.75), ncol=1, fancybox=True, shadow=True, fontsize=10)
lgd = axes[1][1].legend(axes[0][0].get_lines(),lab,loc='lower right', bbox_to_anchor=(1.6, -0.5), ncol=3, fancybox=True, shadow=True, fontsize=12)
art=[lgd]
plt.savefig('timeEvol.png',additional_artists=art,bbox_inches='tight')
plt.show()
