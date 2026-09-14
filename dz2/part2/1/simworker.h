#pragma once
#include <QObject>
#include <QThread>
#include <atomic>
#include "model.h"

// Поток, в котором крутится моделирование
class SimWorker : public QObject {
    Q_OBJECT
public:
    EpidemicModel* model = nullptr;
    int maxSteps = 1000;
    int delayMs = 50;

public slots:
    void run();
    void requestStop();

    signals:
        void tick(int step, int healthy, int sick, int recovered);
    void finished();

private:
    std::atomic<bool> stopFlag{false};
};