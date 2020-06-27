#pragma once

#include <QtWidgets>
#include "Calculator.h"
#include "Plotter.h"

class Menu : public QWidget  {
    Q_OBJECT
    QStackedWidget* stack;
    Calculator* calc;
    Plotter* plot;
public:
    Menu(QStackedWidget* st, QWidget* ptr = nullptr);

public slots:
    void MainWidget();
    void ShowCalc();
    void ShowBalls();
};