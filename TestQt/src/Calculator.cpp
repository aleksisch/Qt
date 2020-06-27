#include "Calculator.h"

Calculator::Calculator(QWidget* ptr) : QWidget(ptr) {
    lcd = new QLCDNumber(12);
    const int bt_size = 4;
    const int lcd_row = 2;
    QChar Buttons[bt_size][bt_size] =  {{'6', '7', '8', '9'},
                                        {'2', '3', '4', '5'},
                                        {'0', '1', '+', '-'},
                                        {'*', '/', '.', '='}};
    auto grd = new QGridLayout;
    grd->addWidget(CreateButton("CE"), lcd_row - 1, bt_size);
    grd->addWidget(CreateButton("Exit"), lcd_row - 2, bt_size);
    grd->addWidget(lcd, 0,0, lcd_row - 1, bt_size);

    for (int i = 0; i < bt_size; i++) {
        for (int l = 0; l < bt_size; l++) {
            grd->addWidget(CreateButton(Buttons[i][l]), i + lcd_row, l);
        }
    }
    setLayout(grd);
}

QPushButton* Calculator::CreateButton(const QString& str) {
    QPushButton* btn = new QPushButton(str);
    btn->setAccessibleDescription("Test");
    btn->setMinimumSize(50, 50);
    connect(btn, SIGNAL(clicked()), SLOT(ButtonClicked()));
    return btn;
}

void Calculator::ButtonClicked() {
    QString cmd = (static_cast<QPushButton*>(sender()))->text();
    if (cmd == "CE") {
        screen_str.clear();
        stack.clear();
        lcd->display("0");
    } else if (cmd == "Exit") {
        emit ExitCalc();
    }
    if (cmd.contains(QRegExp("[0-9]"))) {
        screen_str += cmd;
        lcd->display(screen_str);
    } else if (cmd == "." || cmd == ",") {
        screen_str += cmd;
        lcd->display(screen_str);
    } else {
        if (stack.size() >= 2) {
            stack.push(screen_str);
            double res = calculate();
            stack.clear();
            stack.push(QString::number(res));
            screen_str.clear();
            lcd->display(stack.top());
            if (cmd != "=") {
                stack.push(cmd);
            }
        } else {
            stack.push(screen_str);
            stack.push(cmd);
            screen_str.clear();
            lcd->display(screen_str);
        }
    }
}

double Calculator::calculate() {
    double arg1 = stack.pop().toDouble();
    QString operation = stack.pop();
    double arg2 = stack.pop().toDouble();
    if (operation == "+") {
        return arg2 + arg1;
    } else if (operation == "-") {
        return arg2 - arg1;
    } else if (operation == "*") {
        return arg2 * arg1;
    } else if (operation == "/") {
        return arg2 / arg1;
    }
    return 0;
}

void Calculator::BackBtnAction() {
    emit ExitCalc();
}