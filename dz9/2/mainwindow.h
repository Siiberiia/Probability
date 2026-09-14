#pragma once
#include <QWidget>
#include <QTimer>
#include <QVector>
#include "distribution.h"
#include "walkwidget.h"

class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QLabel;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void onLoadFile();
    void onStart();
    void onStop();
    void onTick();

private:
    DiscreteRV rv;

    double startX = 0;
    int n = 10;

    double currentX = 0;
    int stepsDone = 0;

    // Текущая анимация: за 1 секунду точка должна сдвинуться на delta
    double delta = 0;
    double animProgress = 0;

    QTimer* timer = nullptr;   // 1000 / 60 fps
    QVector<double> history;

    // Накопитель финальных положений (для итогового распределения)
    QVector<double> finals;

    WalkWidget* canvas = nullptr;
    QLineEdit* startEdit = nullptr;
    QLineEdit* nEdit = nullptr;
    QLabel* distLabel = nullptr;
    QPushButton* loadBtn = nullptr;
    QPushButton* startBtn = nullptr;
    QPushButton* stopBtn = nullptr;
    QPlainTextEdit* log = nullptr;

    void showFinalDistribution();
};