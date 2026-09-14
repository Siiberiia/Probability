#pragma once
#include <QWidget>
#include "model.h"
#include "simworker.h"

class QPlainTextEdit;
class QPushButton;
class QLineEdit;
class QComboBox;
class QThread;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void onStart();
    void onStop();
    void onExperimentDone(int index, int outcome);
    void onProgress(int done, int total);
    void onFinished(int cntReturn, int cntRepeat, int cntOk);

private:
    RumourModel model;
    SimWorker* worker = nullptr;
    QThread* thread = nullptr;

    QLineEdit* nEdit = nullptr;
    QLineEdit* rEdit = nullptr;
    QLineEdit* NEdit = nullptr;
    QLineEdit* KEdit = nullptr;
    QComboBox* modeBox = nullptr;
    QPushButton* startBtn = nullptr;
    QPushButton* stopBtn = nullptr;
    QPlainTextEdit* log = nullptr;
};