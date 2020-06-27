#include <QtWidgets>
#include "Menu.h"
#include "Calculator.h"
#include "Plotter.h"

double DistanceSquare(double x, double y);

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    auto st = new QStackedWidget;
    auto m = new Menu(st);
    st->addWidget(m);
    st->setCurrentWidget(m);
    st->show();
    return app.exec();
}