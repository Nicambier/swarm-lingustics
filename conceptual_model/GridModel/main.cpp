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
        if(t%100==0)
            outfile<<w<<endl;
        w.Run();
    }
    outfile<<w<<endl;
    outfile.close();
    lock_guard<mutex> lk(m);
    cout<<"done: "<<dir<<" "<<j<<endl;
    --counter;
    cv.notify_all();
}

void show(World& w, int time) {
    GridWindow window(w.GetSizeX(), w.GetSizeY());
    window.Display(w);
    window.show();
    for(int t=0; t<time; ++t) {
        w.Run();

        window.Display(w);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        window.update();

    }
}

void waitAvailable() {
    unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return thread_counter < thread::hardware_concurrency();});
}

int main(int argc, char * argv[]) {
    string folder = "../comp8bit0to4";
    int x = 30;
    int y = 30;

    int types[] = {TYPE_EE_AGG_AGENT};
    int pop[] = {25,50,100};

    double aParam[1] = {1.75};
    double bParam[1] = {4};
    //for(int ab = 0; ab<25; ++ab) {aParam[ab]=ab*0.25; bParam[ab]=ab*0.25;}

    double mParam[] = {0.01};

    bool weak = true;
    string strength = "";

    int time = 30000;
    int start = 21;
    int runs = 30;

    vector<thread> threads;
    cout<<thread::hardware_concurrency()<<endl;
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
                    for(int j=start; j<=start+runs; ++j) {
                        waitAvailable();
                        ++thread_counter;
                        threads.push_back(thread(run, x, y, pop[p], factory,time,j,ref(thread_counter),dir));
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
                for(int j=start; j<=start+runs; ++j) {
                    waitAvailable();
                    ++thread_counter;
                    threads.push_back(thread(run, x, y, pop[p], factory,time,j,ref(thread_counter),dir));
                    //run(x, y, pop[p], factory,time,runs,dir);
                }
            }
            else {
                for(unsigned int a=0; a<sizeof(aParam) / sizeof(double); ++a) {
                    for(unsigned int b=0; b<sizeof(bParam) / sizeof(double); ++b) {
                        AgentFactory* factory = new AgentFactory(types[i], aParam[a], bParam[b]);
                        if(weak) {strength = "w"; factory->SetWeak(weak);} else strength = "";
                        string dir = folder+"/"+to_string(types[i])+strength+"p"+to_string(pop[p])+"a"+to_string((int) (aParam[a]*100))+"b"+to_string((int) (bParam[b]*100))+"/";
                        const int dir_err = system(("mkdir -p "+dir).c_str());
                        if (-1 == dir_err)
                        {
                            cerr<<"Error creating "<<dir<<endl;
                            exit(1);
                        }
                        for(int j=start; j<=start+runs; ++j) {
                            waitAvailable();
                            ++thread_counter;
                            threads.push_back(thread(run, x, y, pop[p], factory,time,j,ref(thread_counter),dir));
                            //run(x, y, pop[p], factory,time,runs,dir);
                        }
                    }
                }
            }
        }
    }

    for (auto& th : threads) th.join();
    cout<<"done"<<endl;

    /*QApplication app(argc,argv);
    AgentFactory* factory = new AgentFactory(TYPE_NG_AGG_AGENT, 1.75, 4);
    World w(x, y, pop, factory);
    show(w,30000);
    cout<<"done"<<endl;*/
}
