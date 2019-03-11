#include "gridwindow.h"

GridWindow::GridWindow(int size_x, int size_y)
{
    this->size_x = size_x;
    this->size_y = size_y;
    grid = new QTableWidget(size_y,size_x);
    grid->setFixedSize(cell_size*size_y,cell_size*size_x);
    grid->horizontalHeader()->setVisible(false);
    grid->verticalHeader()->setVisible(false);
    grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    grid->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i=0; i<size_x; ++i) {
        grid->setColumnWidth(i,cell_size);
        for(int j=0; j<size_y; ++j) {
            grid->setRowHeight(j,cell_size);
            grid->setItem(j,i, new QTableWidgetItem(""));
            grid->item(j,i)->setFlags(Qt::NoItemFlags);
            cellActivation(i,j,255);
        }
    }

    QHBoxLayout l;
    l.addWidget(grid);
    setLayout(&l);
}

void GridWindow::cellActivation(int x, int y, uint16_t c){
    grid->item(y,x)->setText("");
    grid->item(y,x)->setBackgroundColor(QColor(36*(c>>5),36*((c>>3) & 7),85 * (c & 3)));
    grid->item(y,x)->setTextColor(QColor(36*(c>>5),36*((c>>3) & 7),85 * (c & 3)));
}

void GridWindow::Display(World& w) {
    for(int i=0; i<size_x; ++i) {
        for(int j=0; j<size_y; ++j) {
            cellActivation(i,j,w.CellColour(i,j));
        }
    }
}
