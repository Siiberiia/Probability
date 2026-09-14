#include "simworker.h"

void SimWorker::run() {
    stopFlag = false;

    for (int step = 1; step <= maxSteps; step++) {
        if (stopFlag) break;
        model->step();
        emit tick(step,
                  model->count(HEALTHY),
                  model->count(SICK),
                  model->count(RECOVERED));
        QThread::msleep(delayMs);
        if (!model->hasSick()) break;
    }
    emit finished();
}

void SimWorker::requestStop() {
    stopFlag = true;
}