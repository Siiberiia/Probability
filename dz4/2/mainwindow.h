#pragma once
#include <QWidget>
#include <QVector>
#include <QPointF>
#include "model.h"

class QPlainTextEdit;
class QPushButton;
class QLineEdit;

// Простой виджет для рисования траекторий
class PlotWidget : public QWidget {
public:
    explicit PlotWidget(QWidget* parent = nullptr);
    void setTrajectories(const QVector<QVector<QPointF>>& t);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QVector<QPointF>> trajs;
};

class MainWindow : public QWidget {
public:
    MainWindow();

private:
    TrajectoryModel model;
    QLineEdit* fileEdit = nullptr;
    QPlainTextEdit* log = nullptr;
    PlotWidget* plot = nullptr;
    QPushButton* runBtn = nullptr;
    int K = 5;

    void onLoadConfig();
    void onRun();
};