#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QThread>
#include <QColor>
#include <QPushButton>

#include "world.h"

class GridWindow : public QWidget
{
    Q_OBJECT

private:
    QTableWidget* grid;
    QPushButton* startButton;
    World* w;
    int cell_size = 10;
    int size_x, size_y;
    int time;

    void cellActivation(int x, int y, uint16_t);
    void Display(World* w);
    void Play();
public:
    GridWindow(int size_x, int size_y, World& world, int t=0);

public slots :
    void onClicked();

};

#endif // GRIDWINDOW_H
