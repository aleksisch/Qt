#include <inc/Plotter.h>
#include "Menu.h"
#include "Calculator.h"
#include <iostream>

Menu::Menu(QStackedWidget* st, QWidget* ptr) : QWidget(ptr), stack(st) {

    calc = new Calculator(this);
    plot = new Plotter(this);
    stack->addWidget(plot);
    stack->addWidget(calc);

    auto layout = new QHBoxLayout;
    auto CalcBtn = new QPushButton("Calculator", this);
    auto BallsBtn = new QPushButton("Balls", this);

    layout->addWidget(CalcBtn);
    layout->addWidget(BallsBtn);

    connect(CalcBtn, SIGNAL(clicked()), SLOT(ShowCalc()));
    connect(BallsBtn, SIGNAL(clicked()), SLOT(ShowBalls()));
    connect(plot, SIGNAL(ExitPlotter()), SLOT(MainWidget()));
    connect(calc, SIGNAL(ExitCalc()), SLOT(MainWidget()));

    setLayout(layout);
}


void Menu::ShowCalc() {
    stack->setCurrentWidget(calc);
}

void Menu::ShowBalls() {
    stack->setCurrentWidget(plot);
}

void Menu::MainWidget() {
    stack->setCurrentWidget(this);
}