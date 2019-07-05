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
    Display(w);
    //show();
}

void GridWindow::cellActivation(int x, int y, uint16_t c){
    //grid->item(y,x)->setText("");
    grid->item(y,x)->setBackgroundColor(QColor(4*(c>>10),8 * ((c>>5) & 31),8 * (c & 31)));
    //grid->item(y,x)->setTextColor(QColor(4*(c>>10),8 * ((c>>5) & 31),8 * (c & 31)));
}

void GridWindow::Display(World* w) {
    for(int i=0; i<size_x; ++i) {
        for(int j=0; j<size_y; ++j) {
            cellActivation(i,j,w->CellColour(i,j));
        }
    }
}

void GridWindow::Play() {
    for(int t=0; t<time; ++t) {
        w->Run();

        Display(w);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        update();
    }
}

void GridWindow::onClicked() {
    Play();
}
