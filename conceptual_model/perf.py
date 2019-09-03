# Libraries
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from scipy.stats import ranksums

directory='com_range1_8n'
start=1
qt=100
time=300000

boxplot = True

#nValues=[0.001]
sizes=[25,50,100,200]

fig, axes = plt.subplots(ncols=len(sizes), nrows=2, figsize=[16,5],sharex=True)
#axes[2].set_ylabel('#Words')
for i in range(len(sizes)):
    axes[1][i].set_xlabel('Time-steps')
    axes[1][0].xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: ('%.dk')%(x*1e-3)))

#col=["blue","cyan","red","pink","grey","green","orange"]

if(boxplot):
    col=["green","blue","red"]
    ls=["-","-","-","--"]
    lab=["CE m=0.01","CE m=0.001","Fixed optimal"]
    folders = ['highDensity2/103wpSIZEm10/','highDensity2/103wpSIZEm1/','optiHigh2/101pSIZEopti/']
else:
    col=["green","blue","red","orange"]
    ls=["-","-","-","--"]
    lab=["m=0","m=10^(-2)","m=10^(-10)"]
    folders = ['highDensity/103wpSIZEm0/','highDensity/103wpSIZEm2/','highDensity/103wpSIZEm10/']

dummyLines = [plt.Line2D([],[],color="black",linestyle="-"),plt.Line2D([],[],color="black",linestyle=":")]

def drawFolder(folder,i,size,color,ls):
    x = list(range(0,time+1,200))
    y1 = [[]]*len(x)
    y2 = [[]]*len(x)
    y3 = [[]]*len(x)
    w = [[]]*len(x)
    print(folder)
    for nb in range(start,start+qt):
        try:
            with open(folder+'stat_'+str(nb)) as f:
                lines = f.readlines()
                c = 0
                for line in lines:
                    arr = np.fromstring(line, dtype=float, sep='\t')
                    tempX = arr[0]
                    #print(tempX%1000==0)
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
    b_err = []
    b_med = []
    b_Q1 = []
    b_Q3 = []
    c = []
    wa = []

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
        axes[0][i].plot(x,b,color=color,linestyle=ls)
    else:
        axes[0][i].plot(x,b_med,color=color,linestyle=ls)
        axes[0][i].fill_between(x, b_Q1, b_Q3, alpha=0.2, facecolor=color)

    axes[1][i].plot(x,a,color=color,linestyle="-")
    axes[1][i].plot(x,c,color=color,linestyle=":")
    #axes[1][i].plot(x,wa,color=color,linestyle="-")
    axes[0][i].set_ylim([0,1.1])
    axes[1][i].set_ylim([0,size/4])
    
    return np.array(y2[-1])

final_arrays = {}
for f in range(len(folders)):
    final_arrays[folders[f]] = []
    for s in range(len(sizes)):
        folder = directory+'/'+folders[f].replace('SIZE',str(sizes[s]))
        final_arrays[folders[f]].append(drawFolder(folder,s,sizes[s],col[f],ls[f]))

#print("Wilcoxon rank-sum test (",lab[0],">",lab[2])
print("\\begin{tabular}{|c|c|cc|}")
print("\hline")
print("m & N & statistic & p-value\\\\")
print("\hline")
print("\multirow{4}{*}{0.01}")
for s in range(len(sizes)):
    stati, pval = ranksums(final_arrays[folders[0]][s], final_arrays[folders[2]][s])
    print("&",sizes[s],"&",stati,"&",pval,"\\\\")

print("\hline")
print("\multirow{4}{*}{0.001}")
for s in range(len(sizes)):
    stati, pval = ranksums(final_arrays[folders[1]][s], final_arrays[folders[2]][s])
    print("&",sizes[s],"&",stati,"&",pval,"\\\\")
print("\hline")
print("\end{tabular}")

axes[0][0].set_ylabel('Cluster metric')
axes[1][0].set_ylabel('Lexicon size')
axes[1][len(sizes)-1].legend(dummyLines,["Clusters","Free Agents"],loc='lower left', bbox_to_anchor=(0.55, 0.75), ncol=1, fancybox=True, shadow=True, fontsize=10)
lgd = axes[1][1].legend(axes[0][0].get_lines(),lab,loc='lower right', bbox_to_anchor=(2.3, -0.5), ncol=4, fancybox=True, shadow=True, fontsize=12)
art=[lgd]
if(boxplot):
    plt.savefig(directory+'/timeEvol_boxplot.png',additional_artists=art,bbox_inches='tight')
else:
    plt.savefig(directory+'/timeEvol.png',additional_artists=art,bbox_inches='tight')


#plt.show()

