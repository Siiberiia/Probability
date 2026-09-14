#pragma once
#include <QWidget>
#include <QVector>
#include <QPointF>

// Виджет, отображающий положение точки и траекторию
class WalkWidget : public QWidget {
    Q_OBJECT
public:
    explicit WalkWidget(QWidget* parent = nullptr);

    void setRange(double minX, double maxX);
    void setHistory(const QVector<double>& positions);
    void setCurrent(double x);
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double minX = -20, maxX = 20;
    QVector<double> history;
    double currentX = 0;
};