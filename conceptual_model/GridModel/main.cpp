#include "agentfactory.h"
#include "gridwindow.h"
#include "world.h"
#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>
#include <mutex>
#include <string.h>
#include <condition_variable>
#include <vector>
#include <cstdlib>

using namespace std;

mutex m;
condition_variable cv;
std::atomic<int> thread_counter(0);

void run(int size_x, int size_y, int pop, AgentFactory factory, int time, int start, int iter, atomic<int>& counter, string dir) {
    for(int j=start; j<start+iter; ++j)
    {
        string file = dir+"out_"+to_string(j);
        World w(size_x, size_y, pop, &factory);
        ofstream outfile (file);
        for(int t=0; t<time; ++t) {
            if(t%200==0)
                outfile<<w<<endl;
            w.Run();
        }
        outfile<<w<<endl;
        outfile.close();
    }
    /*lock_guard<mutex> lk(m);
    //cout<<"done: "<<dir<<" "<<j<<endl;
    --counter;
    cv.notify_all();*/
}

void waitAvailable() {
    unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return thread_counter < 6;});
}

int AutoDisplay(int argc, char * argv[], int type_agent, double p1=0, double p2=0, double p3=0) {
    AgentFactory* factory = new AgentFactory(type_agent, p1, p2, p3);
    //AgentFactory* factory = new AgentFactory(type_agent, 0);
    factory->SetWeak(true);
    World w(25, 25, 25, factory);
    QApplication app(argc,argv);
    GridWindow window(w.GetSizeX(), w.GetSizeY(),w,200000);
    window.show();
    delete factory;
    return app.exec();
}

void AutoRun() {
    string folder = "../com_range1_8n/highDensity";
    int x[] = {17,24,35,49};
    int y[] = {17,24,35,49};

    int types[] = {TYPE_EE_AGG_AGENT};
    int pop[] = {25,50,100,200};

    double aParam[] = {3.75};
    double bParam[] = {0.75};
    double cParam[] = {0.875};
    /*double aParam[32];
    double bParam[32];
    double cParam[8];
    for(int ab = 0; ab<32; ++ab) {aParam[ab]=0.25 + ab*0.25; bParam[ab]=0.25 + ab*0.25;}
    for(int c = 0; c<8; ++c) {cParam[c]=c*0.125;}*/

    double mParam[] = {0,0.01,0.001};

    bool weak = true;
    string strength = "";

    int time = 300000;
    int start = 1;
    int runs = 20;

    vector<thread> threads;
    cout<<folder<<" "<<thread::hardware_concurrency()<<endl;
    for(unsigned int p=0; p<sizeof(pop) / sizeof(int); ++p) {
        for(unsigned int i=0; i<sizeof(types) / sizeof(int); ++i) {
            if(types[i]==TYPE_EE_AGG_AGENT) {
                for(unsigned int m=0; m<sizeof(mParam) / sizeof(double); ++m) {
                    double tempMparam = mParam[m];
                    if(mParam[m]==0)
                        tempMparam = 0;
                    AgentFactory factory(types[i], tempMparam);
                    if(weak) {strength = "w"; factory.SetWeak(weak);} else strength = "";
                    string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"m"+to_string((int) (mParam[m]*1000))+"/";
                    const int dir_err = system(("mkdir -p "+dir).c_str());
                    if (-1 == dir_err)
                    {
                        cerr<<"Error creating "<<dir<<endl;
                        exit(1);
                    }
                    //waitAvailable();
                    //++thread_counter;
                    //threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir));
                    run(x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir);
                }
            }
            else if(types[i]==TYPE_RANDOM_AGG_AGENT) {
                AgentFactory factory(types[i]);
                strength = "";
                string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"/";
                const int dir_err = system(("mkdir -p "+dir).c_str());
                if (-1 == dir_err)
                {
                    cerr<<"Error creating "<<dir<<endl;
                    exit(1);
                }
                //waitAvailable();
                //++thread_counter;
                //threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir));
                run(x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir);
            }
            else {
                for(unsigned int a=0; a<sizeof(aParam) / sizeof(double); ++a) {
                    for(unsigned int b=0; b<sizeof(bParam) / sizeof(double); ++b) {
                        for(unsigned int c=0; c<sizeof(cParam) / sizeof(double); ++c) {
                            AgentFactory factory(types[i], aParam[a], bParam[b], cParam[c]);
                            strength = "";
                            string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"a"+to_string((int) (aParam[a]*100))+"b"+to_string((int) (bParam[b]*100))+"c"+to_string((int) (cParam[c]*1000))+"/";
                            const int dir_err = system(("mkdir -p "+dir).c_str());
                            if (-1 == dir_err)
                            {
                                cerr<<"Error creating "<<dir<<endl;
                                exit(1);
                            }
                            //waitAvailable();
                            //++thread_counter;
                            //threads.push_back(thread(run, x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir));
                            run(x[p], y[p], pop[p], factory,time,start,runs,ref(thread_counter),dir);
                        }
                    }
                }
            }
        }
    }
    //for (auto& th : threads) th.join();
    cout<<"done"<<endl;
}

void configuredRun(int argc, char * argv[]) {
    int time = 0;
    int size = 0;
    int pop = 0;
    int type_agent = 0;
    double p[3];
    unsigned int seed = 0;

    for(int i=1;i<argc;++i) {
        if(strcmp(argv[i],"-t")==0)
            time = atoi(argv[i+1]);
        else if(strcmp(argv[i],"-s")==0)
            size = atoi(argv[i+1]);
        else if(strcmp(argv[i],"-p")==0)
            pop = atoi(argv[i+1]);
        else if(strcmp(argv[i],"--type")==0)
            type_agent = atoi(argv[i+1]);
        else if(strcmp(argv[i],"-a")==0)
            p[0] = atoi(argv[i+1])*0.25;
        else if(strcmp(argv[i],"-b")==0)
            p[1] = atoi(argv[i+1])*0.25;
        else if(strcmp(argv[i],"-c")==0)
            p[2] = atoi(argv[i+1])*0.125;
        else if(strcmp(argv[i],"--seed")==0)
            seed = atoi(argv[i+1]);
    }

    bool weak = false;
    AgentFactory factory(type_agent, p[0], p[1], p[2]);
    factory.SetWeak(weak);
    World w(size, size, pop, &factory, seed);
    for(int t=0; t<time; ++t) {
        w.Run();
    }
    cout<<(1-w.Evaluate())<<endl; //irace minimises output
}

int main(int argc, char * argv[]) {
    //configuredRun(argc, argv);
    AutoRun();
    //return AutoDisplay(argc, argv, TYPE_EE_AGG_AGENT, 0.001);
    //return AutoDisplay(argc, argv, TYPE_AGG_AGENT, 1.75,4,0.375);
}
