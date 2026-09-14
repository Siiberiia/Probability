#include "walkwidget.h"
#include <QPainter>
using namespace std;

WalkWidget::WalkWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(600, 200);
}

void WalkWidget::setRange(double mn, double mx) {
    minX = mn;
    maxX = mx;
    update();
}

void WalkWidget::setHistory(const QVector<double>& h) {
    history = h;
    update();
}

void WalkWidget::setCurrent(double x) {
    currentX = x;
    update();
}

void WalkWidget::clear() {
    history.clear();
    currentX = 0;
    update();
}

void WalkWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), Qt::white);

    if (maxX - minX < 1e-9) return;

    int W = width(), H = height();
    int midY = H / 2;
    int margin = 40;

    auto toScreen = [&](double x) {
        double sx = margin + (x - minX) / (maxX - minX) * (W - 2 * margin);
        return sx;
    };

    // Ось
    p.setPen(Qt::gray);
    p.drawLine(margin, midY, W - margin, midY);

    // Отметки
    p.setPen(Qt::black);
    for (double x = ceil(minX); x <= maxX; x += 1.0) {
        double sx = toScreen(x);
        p.drawLine(QPointF(sx, midY - 3), QPointF(sx, midY + 3));
    }

    // История траектории
    p.setPen(Qt::blue);
    for (int i = 1; i < history.size(); i++) {
        p.drawLine(QPointF(toScreen(history[i-1]), midY),
                   QPointF(toScreen(history[i]),   midY));
    }

    // Текущее положение
    p.setBrush(Qt::red);
    p.setPen(Qt::red);
    double cx = toScreen(currentX);
    p.drawEllipse(QPointF(cx, midY), 6, 6);
}