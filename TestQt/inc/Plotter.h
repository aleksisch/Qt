#pragma once

#include <QtOpenGL>
#include <QTimer>
#include <QTime>
#include <math.h>
#include <iostream>

struct Ball {
    Ball(int x = 50, int y = 100, double speed_x = 10, double speed_y = 10, int radius = 50, double force = 5000)
        : x(x), y(y), speed_x(speed_x), speed_y(speed_y), radius(radius), force(force) {}
    void Update() {
        x += speed_x;
        y += speed_y;
    }
    friend std::ostream& operator << (std::ostream& cout, const Ball& b) {
        cout << "x, y: " << b.x << " " << b.y << " speed x, y: " << b.speed_x << " " << b.speed_y << std::endl;
        return cout;
    }
    int x;
    int y;
    int radius;
    double speed_x;
    double speed_y;
    double force;
};

class Plotter : public QOpenGLWidget {
    Q_OBJECT
    int width;
    int height;
    int ms_reset;
    int number_balls;
    QGridLayout* grd;
    QWidget* ButtonStyle;
    QLabel* fps;
    QLabel* energy;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QTimer* tim;
    std::vector<Ball> v_balls;
    void CheckCollide();
    void ChangePower();
    void UpdateLabel();
public:
    Plotter(QWidget* ptr = nullptr, int number_balls = 2, int ms_reset = 10);
    void draw(Ball ball, double r, double g, double b, double step = 0.01);
signals:
    void ExitPlotter();
public slots:
    void AddButton();
    void DeleteButton();
    void BackBtnAction();
};