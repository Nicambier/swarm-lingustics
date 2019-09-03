# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from scipy.stats import ranksums

fol='com_range1_8n'
directory=fol+'/highDensity/103wpSIZEm10/'
start=1
qt=20
time=300000

sizes=[25,50,100,200]

fig, axes = plt.subplots(ncols=len(sizes), nrows=3, figsize=[16,7.5],sharex=True)
#axes[2].set_ylabel('#Words')
for i in range(len(sizes)):
    axes[1][i].set_xlabel('Time-steps')
    axes[1][0].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.dk')%(x*1e-3)))

col=["green","blue","red"]
lab=["a","b","c"]

def drawFolder(folder,s,size):
    x = list(range(0,time+100,200))
    a = [[] for i in range(len(x))]
    b = [[] for i in range(len(x))]
    c = [[] for i in range(len(x))]
    print(folder)
    for nb in range(start,start+qt):
        try:
            with open(folder+'out_'+str(nb)) as f:
                lines = f.readlines()
                count = 0
                for line in lines:
                    l = line.strip()
                    l = l.split(' ')
                    for i in range(1,len(l)):
                        members = l[i].split('?')
                        if(len(members)>1 and members[1].strip()!=''):
                            for w in members[1].split(','):
                                word = int(w)
                                a[count].append(0.02*((word>>8) & 255))
                                b[count].append(0.02*(word & 255))
                                c[count].append(0.0039*((word>>16) & 255))
                    count+=1
                        
                        
        except Exception as e:
            print(e," ",folder+'stat_'+str(nb))

    a_mean = []
    b_mean = []
    c_mean = []

    a_std = []
    b_std = []
    c_std = []

    for z in range(len(a)):
        a_mean.append(np.mean(a[z]))
        b_mean.append(np.mean(b[z]))
        c_mean.append(np.mean(c[z]))

        a_std.append(np.std(a[z]))
        b_std.append(np.std(b[z]))
        c_std.append(np.std(c[z]))
        
        
    a_mean=np.array(a_mean)
    b_mean=np.array(b_mean)
    c_mean=np.array(c_mean)

    a_std=np.array(a_std)
    b_std=np.array(b_std)
    c_std=np.array(c_std)
    
    axes[0][s].set_title("N="+str(sizes[s]))
    temp = [a_mean,b_mean,c_mean]
    temp_std = [a_std,b_std,c_std]
    print(len(x), len(a), len(b), len(c))
    for j in range(3):
        axes[j][s].plot(x,temp[j],color=col[j],linestyle="-")
        axes[j][s].fill_between(x, [temp[j][val]-temp_std[j][val] for val in range(len(temp[j]))], [temp[j][val]+temp_std[j][val] for val in range(len(temp[j]))], alpha=0.2, facecolor=col[j])

    axes[0][s].set_ylim([0,5.2])
    axes[1][s].set_ylim([0,5.2])
    axes[2][s].set_ylim([0,1])
    

for s in range(len(sizes)):
    folder = directory.replace('SIZE',str(sizes[s]))
    drawFolder(folder,s,sizes[s])

axes[0][0].set_ylabel('a')
axes[1][0].set_ylabel('b')
axes[2][0].set_ylabel('c')
##lines = []
##lines.extend(axes[0][0].get_lines())
##lines.extend(axes[1][0].get_lines())
##lines.extend(axes[2][0].get_lines())
##lgd = axes[2][0].legend(lines,lab,loc='lower right', bbox_to_anchor=(2.7, -0.5), ncol=4, fancybox=True, shadow=True, fontsize=12)
plt.savefig(fol+'/wordEvol.png')


#plt.show()

