#pragma once
#include <QWidget>
#include "model.h"
#include "simworker.h"

class QPlainTextEdit;
class QPushButton;
class QLineEdit;
class QLabel;
class QThread;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void onLoad();
    void onStart();
    void onStop();
    void onTick(int step, int h, int s, int r);
    void onFinished();

private:
    EpidemicModel model;
    SimWorker* worker = nullptr;
    QThread* thread = nullptr;

    QLabel* fileLabel = nullptr;
    QLineEdit* p1Edit = nullptr;
    QLineEdit* p2Edit = nullptr;
    QLineEdit* stepsEdit = nullptr;
    QLineEdit* delayEdit = nullptr;
    QPushButton* startBtn = nullptr;
    QPushButton* stopBtn = nullptr;
    QPlainTextEdit* log = nullptr;
};