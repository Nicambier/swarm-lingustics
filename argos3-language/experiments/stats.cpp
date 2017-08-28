#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <utility>
#include <list>
#include <math.h> 

using namespace std;

int clusterSize(list< pair<float,float> >::iterator seed, list< pair<float,float> >& pos) {
    pair<float,float> s = *seed;
    pos.erase(seed);
    int size = 1;
    if(pos.size() > 0) {
        bool hasNeighbours = true;
        while(hasNeighbours) {
            hasNeighbours = false;
            for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
                if(sqrt(pow((it->first - s.first),2)+pow((it->second - s.second),2)) < 1) {
                    hasNeighbours = true;
                    size += clusterSize(it,pos);
                    break;
                }                
            }
        }
    }
    return size;
}

int countClusters(list< pair<float,float> > pos) {
    int c = 0;
    pair<float,float> seed;
    while(pos.size() > 0) {
        int size = clusterSize(pos.begin(),pos);
        if(size >= 3) {
            ++c;
            cout<<size<<" ";
        }
    }
    cout<<endl;
            
    return c;
}

double std2D(list< pair<float,float> > pos) {
    pair<float, float> accPos = make_pair(0,0);
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        accPos.first += it->first;
        accPos.second += it->second;
    }
    pair<float, float> meanPos = make_pair(accPos.first/pos.size(),accPos.second/pos.size());
    
    double accStd = 0;
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        accStd += pow((it->first - meanPos.first),2)+pow((it->second - meanPos.second),2);
    }
    
    return accStd/(pos.size()*4*pow(0.14,2));
}
    
    
double parse(string path) {
    // Open the file for text read
    ifstream file;
    file.open(path.c_str(), ifstream::in);
    if(file.fail()) {
        cerr << "Error opening file " << path << endl;
    }
   
    string l,line;
    int i = 1;
    set<int> lexicon;
    list< pair<float,float> > positions;
    list<short int> words;
    while (getline(file, l))
    {
        line = l;
    }
    
    lexicon.clear();
    positions.clear();
    int pos = line.find_first_of("(),");
    while(pos != string::npos) {
        line.erase(pos,1);
        pos = line.find_first_of("(),",pos);
    }
    
    pos = line.find("N");
    while(pos != string::npos) {
        line.replace(pos,3,"0");
        pos = line.find("N");
    }
    
    float a, b;
    short int c;
    
    istringstream iss(line);
    iss >> i;
    while(iss >> a >> b >> c) {
        positions.push_back(make_pair(a,b));
        words.push_back(c);
    }
    
    double stdAcc = 0;
    /*int currentW;
    bool newCluster = true;
    list< pair<float,float> > posByWord;
    while(newCluster) {
        posByWord.clear();
        newCluster = false;
        list<short int>::iterator w = words.begin();
        for (list< pair<float,float> >::iterator it=positions.begin(); it != positions.end(); ++it) {
            c = *w;
            if(lexicon.count(c)==0 and !newCluster) {
                newCluster = true;
                lexicon.insert(c);
                currentW = c;
            }
            if(newCluster and currentW == c)
                posByWord.push_back(make_pair(a,b));
            positions.push_back(make_pair(a,b));
            ++w;
        }
        if(newCluster)
            stdAcc += std2D(posByWord);
    }*/
    
    cout << i << ": " << countClusters(positions) << " "<< endl;//lexicon.size() << " " << stdAcc/lexicon.size() << endl;
    file.close();
    return stdAcc/lexicon.size();
    
}

int main() {
    string path;
    
    float clusters[6][10];
    
    int sizes[2] = {20,100};
    int algos[2] = {1,2};
    int probas[3] = {14,17,20};
    //int i = 2;
    double stdAcc;
    for(int s=0; s<2; ++s) {
        for(int a=0; a<2; ++a) {
            for(int p=0; p<3; ++p) {
                stdAcc = 0;
                cout << "s" << to_string(sizes[s]) << "_a" << to_string(algos[a]) << "_p" << to_string(probas[p]) <<": "<<endl;
                for(int i=1; i<= 20; ++i) {
                    cout<<i<<": "<<endl;
                    path = "results/" + to_string(i)+"/s"+to_string(sizes[s])+"_a"+to_string(algos[a])+"_p"+to_string(probas[p])+".txt";
                    stdAcc += parse(path);
                }
                cout <<"Standard deviation: "<<stdAcc/20<<endl<<endl<<endl;
            }
        }
    }
}