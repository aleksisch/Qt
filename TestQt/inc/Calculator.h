#pragma once
#include <QtWidgets>

class Calculator : public QWidget {
    Q_OBJECT
    QLCDNumber* lcd;
    QStack<QString> stack;
    QString screen_str;
    double calculate();
public:
    Calculator(QWidget* ptr = nullptr);
    QPushButton* CreateButton(const QString& str);
signals:
    void ExitCalc();
public slots:
    void BackBtnAction();
    void ButtonClicked();
};