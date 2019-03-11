#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QThread>
#include <QColor>

#include "world.h"

class GridWindow : public QWidget
{
private:
    QTableWidget* grid;
    int cell_size = 10;
    int size_x, size_y;

    void cellActivation(int x, int y, uint16_t);
public:
    GridWindow(int size_x, int size_y);
    void Display(World& w);

};

#endif // GRIDWINDOW_H
