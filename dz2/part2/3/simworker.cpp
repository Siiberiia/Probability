#include "simworker.h"

void SimWorker::run() {
    stopFlag = false;

    int cntReturn = 0, cntRepeat = 0, cntOk = 0;

    for (int t = 0; t < K; t++) {
        if (stopFlag) break;

        Outcome o = model->run();

        if (o == RETURN_TO_FIRST) cntReturn++;
        else if (o == REPEAT)     cntRepeat++;
        else                      cntOk++;

        // Показываются только первые showLimit экспериментов
        if (t < showLimit)
            emit experimentDone(t + 1, (int)o);

        // Прогресс раз в 1000 экспериментов
        if ((t + 1) % 1000 == 0)
            emit progress(t + 1, K);
    }

    emit finished(cntReturn, cntRepeat, cntOk);
}

void SimWorker::requestStop() {
    stopFlag = true;
}