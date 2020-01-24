#include "gridwindow.h"

GridWindow::GridWindow(int size_x, int size_y, World& world, int t): w(&world), time(t)
{
    this->size_x = size_x;
    this->size_y = size_y;

    startButton = new QPushButton("Push to start",this);
    connect(startButton , SIGNAL(clicked()), this, SLOT(onClicked()));
    startButton->move((cell_size*size_x)/2 - 50,0);

    grid = new QTableWidget(size_y,size_x,this);
    grid->setFixedSize(cell_size*size_y,cell_size*size_x);
    grid->horizontalHeader()->setVisible(false);
    grid->verticalHeader()->setVisible(false);
    grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    grid->move(0,25);

    for(int i=0; i<size_x; ++i) {
        grid->setColumnWidth(i,cell_size);
        for(int j=0; j<size_y; ++j) {
            grid->setRowHeight(j,cell_size);
            grid->setItem(j,i, new QTableWidgetItem(""));
            grid->item(j,i)->setFlags(Qt::NoItemFlags);
            cellActivation(i,j,65535);
        }
    }
    grid->setShowGrid(false);
    Display(w);
}

void GridWindow::cellActivation(int x, int y, uint32_t c){
    //grid->item(y,x)->setText("");
    grid->item(y,x)->setBackgroundColor(QColor(c>>16,(c>>8) & 255,c & 255));
    //grid->item(y,x)->setBackgroundColor(QColor(4*(c>>10),8 * ((c>>5) & 31),8 * (c & 31))); //16 bits
    //grid->item(y,x)->setTextColor(QColor(4*(c>>10),8 * ((c>>5) & 31),8 * (c & 31))); //8 bits
}

void GridWindow::Display(World* w) {
    for(int i=0; i<size_x; ++i) {
        for(int j=0; j<size_y; ++j) {
            cellActivation(i,j,w->CellColour(i,j));
        }
    }
}

void GridWindow::Play() {
    for(int t=0; t<time and !w->isFinished(); ++t) {
        w->Run();

        Display(w);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        //std::cout<<*w<<std::endl;
        //usleep(10000);
        update();
    }
}

void GridWindow::Save()
{
    int i = 1;
    for(int t=0; t<time and !w->isFinished(); ++t) {
        w->Run();

        if(t%10==0) {
            Display(w);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            //usleep(100000);
            update();
            QPixmap pixmap(grid->size());
            grid->render(&pixmap);
            std::string path = "../infection100/test-"+std::to_string(i)+".png";
            pixmap.save(QString(path.c_str()),"PNG",100);
            ++i;
        }
    }
}

void GridWindow::onClicked() {
    Play();
    //Save();
}
