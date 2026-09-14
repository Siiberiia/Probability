#pragma once
#include <QObject>
#include <QThread>
#include <atomic>
#include "model.h"

// Поток, в котором прогоняются эксперименты
class SimWorker : public QObject {
    Q_OBJECT
public:
    RumourModel* model = nullptr;
    int K = 100000;      // число экспериментов
    int showLimit = 20;  // сколько первых экспериментов показать

public slots:
    void run();
    void requestStop();

    signals:
        void experimentDone(int index, int outcome);
    void progress(int done, int total);
    void finished(int countReturn, int countRepeat, int countOk);

private:
    std::atomic<bool> stopFlag{false};
};