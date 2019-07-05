#include "agentfactory.h"
#include "gridwindow.h"
#include "world.h"
#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

mutex m;
condition_variable cv;
std::atomic<int> thread_counter(0);

void run(int size_x, int size_y, int pop, AgentFactory* factory, int time, int j, atomic<int>& counter, string dir) {
    string file = dir+"out_"+to_string(j);
    World w(size_x, size_y, pop, factory);
    ofstream outfile (file);
    for(int t=0; t<time; ++t) {
        //if(t%500==0)
        //    outfile<<w<<endl;
        w.Run();
    }
    outfile<<w<<endl;
    outfile.close();
    lock_guard<mutex> lk(m);
    //cout<<"done: "<<dir<<" "<<j<<endl;
    --counter;
    cv.notify_all();
}

void waitAvailable() {
    unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return thread_counter < thread::hardware_concurrency();});
}

int main(int argc, char * argv[]) {
    string folder = "../com_range1/brute_force";
    int x[] = {25,35,50,71};
    int y[] = {25,35,50,71};

    int types[] = {TYPE_AGG_AGENT};
    int pop[] = {25};

    //double aParam[] = {2};
    //double bParam[] = {150};
    double aParam[32];
    double bParam[32];
    double cParam[8];
    for(int ab = 0; ab<32; ++ab) {aParam[ab]=0.25 + ab*0.25; bParam[ab]=0.25 + ab*0.25;}
    for(int c = 0; c<8; ++c) {cParam[c]=c*0.125;}

    double mParam[] = {0.01, 0.001};

    bool weak = true;
    string strength = "";

    int time = 100000;
    int start = 1;
    int runs = 20;

    vector<thread> threads;
    cout<<folder<<" "<<thread::hardware_concurrency()<<endl;
    for(unsigned int p=0; p<sizeof(pop) / sizeof(int); ++p) {
        for(unsigned int i=0; i<sizeof(types) / sizeof(int); ++i) {
            if(types[i]==TYPE_EE_AGG_AGENT) {
                for(unsigned int m=0; m<sizeof(mParam) / sizeof(double); ++m) {
                    AgentFactory* factory = new AgentFactory(types[i], mParam[m]);
                    if(weak) {strength = "w"; factory->SetWeak(weak);} else strength = "";
                    string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"m"+to_string((int) (mParam[m]*1000))+"/";
                    const int dir_err = system(("mkdir -p "+dir).c_str());
                    if (-1 == dir_err)
                    {
                        cerr<<"Error creating "<<dir<<endl;
                        exit(1);
                    }
                    for(int j=start; j<start+runs; ++j) {
                        waitAvailable();
                        ++thread_counter;
                        threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,j,ref(thread_counter),dir));
                        //run(x, y, pop[p], factory,time,runs,dir);
                    }
                }
            }
            else if(types[i]==TYPE_RANDOM_AGG_AGENT) {
                AgentFactory* factory = new AgentFactory(types[i]);
                strength = "";
                string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"/";
                const int dir_err = system(("mkdir -p "+dir).c_str());
                if (-1 == dir_err)
                {
                    cerr<<"Error creating "<<dir<<endl;
                    exit(1);
                }
                for(int j=start; j<start+runs; ++j) {
                    waitAvailable();
                    ++thread_counter;
                    threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,j,ref(thread_counter),dir));
                    //run(x, y, pop[p], factory,time,runs,dir);
                }
            }
            else {
                for(unsigned int a=0; a<sizeof(aParam) / sizeof(double); ++a) {
                    for(unsigned int b=0; b<sizeof(bParam) / sizeof(double); ++b) {
                        for(unsigned int c=0; c<sizeof(cParam) / sizeof(double); ++c) {
                            AgentFactory* factory = new AgentFactory(types[i], aParam[a], bParam[b]);
                            strength = "";
                            string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"a"+to_string((int) (aParam[a]*100))+"b"+to_string((int) (bParam[b]*100))+"c"+to_string((int) (cParam[c]*1000))+"/";
                            const int dir_err = system(("mkdir -p "+dir).c_str());
                            if (-1 == dir_err)
                            {
                                cerr<<"Error creating "<<dir<<endl;
                                exit(1);
                            }
                            for(int j=start; j<start+runs; ++j) {
                                waitAvailable();
                                ++thread_counter;
                                threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,j,ref(thread_counter),dir));
                                //run(x, y, pop[p], factory,time,runs,dir);
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto& th : threads) th.join();
    cout<<"done"<<endl;

    //AgentFactory* factory = new AgentFactory(TYPE_AGG_AGENT, 0.5, 1.25);
    /*AgentFactory* factory = new AgentFactory(TYPE_EE_AGG_AGENT, 0.01);
    factory->SetWeak(true);
    World w(70, 70, 200, factory);
    QApplication app(argc,argv);
    GridWindow window(w.GetSizeX(), w.GetSizeY(),w,1000000);
    window.show();
    //window.Play(w,100000);
    return app.exec();*/
}
