#include <iostream>
#include "Plotter.h"
#include <chrono>

static double DistanceSquare(double x, double y) {
    return x * x + y * y;
}
static double DistanceSquare(std::pair<int, int> a) {
    return a.first*a.first + a.second*a.second;
}

Plotter::Plotter(QWidget *ptr, int number_balls, int ms_reset) : QOpenGLWidget(ptr), ms_reset(ms_reset), number_balls(number_balls) {
    tim = new QTimer();
    tim->setInterval(ms_reset);

    ButtonStyle = new QWidget(this);
    ButtonStyle->setStyleSheet("background-color:white");

    grd = new QGridLayout(ButtonStyle);
    auto add_btn = new QPushButton("Add v_balls");
    auto del_btn = new QPushButton("Del v_balls");
    fps = new QLabel("Fps:");
    energy = new QLabel("Energy:");
    auto BackBtn = new QPushButton("Exit");

    grd->addWidget(add_btn, 1, 0);
    grd->addWidget(del_btn, 1, 1);
    grd->addWidget(fps, 0, 0);
    grd->addWidget(energy, 0, 1);
    grd->addWidget(BackBtn, 2, 0);

    connect(BackBtn, SIGNAL(clicked()), SLOT(BackBtnAction()));
    connect(add_btn, SIGNAL(clicked()), SLOT(AddButton()));
    connect(del_btn, SIGNAL(clicked()), SLOT(DeleteButton()));
    connect(tim, SIGNAL(timeout()), this, SLOT(update()));
    tim->start();
}

void Plotter::initializeGL() {
    QOpenGLFunctions* pFunc = QOpenGLContext::currentContext()->functions();
    pFunc->glClearColor(0, 0, 0, 1) ;
}

void Plotter::resizeGL(int w, int h) {
    width = w;
    height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(0, w, h, 0, -1, 1);
    ButtonStyle->setGeometry(0, 0, grd->columnCount() * 200, grd->rowCount() * 30);
    v_balls.clear();
    for (int i = 0; i < number_balls; i++) {
        AddButton();
    }
}

void Plotter::paintGL() {
    UpdateLabel();
    ChangePower();
    CheckCollide();
    for (auto& i : v_balls) {
        draw(i, 0, 0, 0);    //clean
        i.Update();
        draw(i, 1, 1, 1);
    }
}

void Plotter::draw(Ball ball, double r, double g, double b, double step) {
    const double pi = acos(0) * 2;
    glBegin(GL_POLYGON);
        glColor3f(r, g, b);
        for (double i = step; i < 2 * pi; i += step) {
            double fAngle = 2 * pi / i;
            int x = ball.x + cos(fAngle) * ball.radius;
            int y = ball.y + sin(fAngle) * ball.radius;
            glVertex2f (x, y);
        }
    glEnd();
}

void Plotter::CheckCollide() {
    for (auto& b : v_balls) {
        if (b.x + b.speed_x >= width || b.x + b.speed_x <= 0) {
            b.speed_x = -b.speed_x;
        }
        if (b.y + b.speed_y >= height || b.y + b.speed_y <= 0) {
            b.speed_y = -b.speed_y;
        }
    }
    for (int i = 0; i < v_balls.size(); i++) {
        for (int l = i + 1; l < v_balls.size(); l++) {
            Ball& b1 = v_balls[i];
            Ball& b2 = v_balls[l];
            if (DistanceSquare(b1.x - b2.x, b1.y - b2.y) <= (b1.radius + b2.radius) * (b1.radius + b2.radius)) {
                std::pair<int, int> direction(b1.x - b2.x, b1.y - b2.y);
                if (DistanceSquare(direction) == 0)
                    break;
                double Proj_b1 = (b1.speed_x * direction.first + b1.speed_y * direction.second) / DistanceSquare(direction);
                double Proj_b2 = (b2.speed_x * direction.first + b2.speed_y * direction.second) / DistanceSquare(direction);
                std::pair<double, double> delta(direction.first * (Proj_b1 - Proj_b2), direction.second * (Proj_b1 - Proj_b2));
                b1.speed_x -= delta.first;
                b2.speed_x += delta.first;
                b1.speed_y -= delta.second;
                b2.speed_y += delta.second;
            }
        }
    }
}

void Plotter::ChangePower() {
    for (int i = 0; i < v_balls.size(); i++) {
        for (int j = i + 1; j < v_balls.size(); j++) {
            Ball& b1 = v_balls[i];
            Ball& b2 = v_balls[j];
            std::pair<int, int> direction(b2.x - b1.x, b2.y - b1.y);
            double f = 6.67 * b1.force * b2.force / DistanceSquare(direction);
            double sqrtDist = sqrt(DistanceSquare(direction));
            b1.speed_x += f * direction.first / b1.force / sqrtDist;
            b1.speed_y += f * direction.second / b1.force / sqrtDist;
            b2.speed_x -= f * direction.first / b2.force / sqrtDist;
            b2.speed_y -= f * direction.second / b2.force / sqrtDist;
        }
    }
}

void Plotter::AddButton() {
    v_balls.emplace_back(rand() % width, rand() % height, rand() % 10, rand() % 10, 50);
}

void Plotter::DeleteButton() {
    if (v_balls.empty())
        return;
    draw(v_balls.back(), 0, 0, 0);    //clean
    v_balls.pop_back();
}


void Plotter::UpdateLabel() {
    static auto last_time = std::chrono::system_clock::now();
    auto cur = std::chrono::system_clock::now();
    double delta = std::chrono::duration<double>(cur - last_time).count();
    last_time = cur;
    fps->setText("FPS: " + QString::number(1 / delta));
    double sum_energy = 0;

    for (auto& i : v_balls) {
        sum_energy += DistanceSquare(i.speed_x, i.speed_y) * i.force * 1 / 2;
    }
    for (int i = 0; i < v_balls.size(); i++) {
        for (int l = i + 1; l < v_balls.size(); l++) {
            Ball& b1 = v_balls[i];
            Ball& b2 = v_balls[l];
            sum_energy -= 6.67 * b1.force * b2.force / sqrt(DistanceSquare(b2.x - b1.x, b2.y - b1.y));
        }
    }
    energy->setText("Energy: " + QString::number(sum_energy));
}

void Plotter::BackBtnAction() {
    emit ExitPlotter();
}