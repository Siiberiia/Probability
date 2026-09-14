#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QThread>
#include <QGroupBox>
#include <QFormLayout>

MainWindow::MainWindow() {
    setWindowTitle("Rumour spread simulation");
    resize(900, 600);

    // --- Управление ---
    nEdit = new QLineEdit("10");
    rEdit = new QLineEdit("5");
    NEdit = new QLineEdit("3");
    KEdit = new QLineEdit("100000");

    modeBox = new QComboBox;
    modeBox->addItem("One person at a time");
    modeBox->addItem("Group of N people");

    startBtn = new QPushButton("Start");
    stopBtn  = new QPushButton("Stop");
    stopBtn->setEnabled(false);

    auto* form = new QFormLayout;
    form->addRow("n (people-1):", nEdit);
    form->addRow("r (transfers):", rEdit);
    form->addRow("N (group size):", NEdit);
    form->addRow("K (experiments):", KEdit);
    form->addRow("Mode:", modeBox);

    auto* ctrlBox = new QGroupBox("Control");
    auto* ctrlLayout = new QVBoxLayout;
    ctrlLayout->addLayout(form);
    auto* btnRow = new QHBoxLayout;
    btnRow->addWidget(startBtn);
    btnRow->addWidget(stopBtn);
    ctrlLayout->addLayout(btnRow);
    ctrlBox->setLayout(ctrlLayout);

    // --- Вывод ---
    log = new QPlainTextEdit;
    log->setReadOnly(true);

    auto* layout = new QHBoxLayout(this);
    layout->addWidget(ctrlBox, 1);
    layout->addWidget(log, 3);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(stopBtn,  &QPushButton::clicked, this, &MainWindow::onStop);
}

void MainWindow::onStart() {
    model.n = nEdit->text().toInt();
    model.r = rEdit->text().toInt();
    model.N = NEdit->text().toInt();
    model.mode = (modeBox->currentIndex() == 0) ? ONE : GROUP;

    if (model.n < 1 || model.r < 1 || model.N < 1) {
        QMessageBox::warning(this, "Error", "n, r, N must be >= 1");
        return;
    }

    int K = KEdit->text().toInt();
    if (K < 1) {
        QMessageBox::warning(this, "Error", "K must be >= 1");
        return;
    }

    worker = new SimWorker;
    worker->model = &model;
    worker->K = K;
    worker->showLimit = 20;

    thread = new QThread(this);
    worker->moveToThread(thread);

    connect(thread, &QThread::started,  worker, &SimWorker::run);
    connect(worker, &SimWorker::experimentDone, this, &MainWindow::onExperimentDone);
    connect(worker, &SimWorker::progress,       this, &MainWindow::onProgress);
    connect(worker, &SimWorker::finished,       this, &MainWindow::onFinished);
    connect(worker, &SimWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);

    log->appendPlainText("=== Simulation started ===");
    log->appendPlainText(QString("n=%1 r=%2 N=%3 K=%4 mode=%5")
                             .arg(model.n).arg(model.r).arg(model.N).arg(K)
                             .arg(modeBox->currentText()));

    thread->start();
}

void MainWindow::onStop() {
    if (worker) worker->requestStop();
    stopBtn->setEnabled(false);
}

void MainWindow::onExperimentDone(int index, int outcome) {
    log->appendPlainText(
        QString("Experiment %1: %2")
            .arg(index).arg(QString::fromStdString(
                RumourModel::outcomeName((Outcome)outcome))));
}

void MainWindow::onProgress(int done, int total) {
    log->appendPlainText(QString("... %1 / %2").arg(done).arg(total));
}

void MainWindow::onFinished(int cntReturn, int cntRepeat, int cntOk) {
    int total = cntReturn + cntRepeat + cntOk;
    if (total == 0) total = 1;

    log->appendPlainText("=== Simulation finished ===");
    log->appendPlainText(QString("Total experiments: %1").arg(total));

    auto show = [&](const QString& name, int c) {
        log->appendPlainText(
            QString("  %1: count=%2  emp P=%3")
                .arg(name).arg(c)
                .arg(c / (double)total, 0, 'f', 6));
    };

    show("Returned to first", cntReturn);
    show("Repeated to someone", cntRepeat);
    show("Ok (r transfers done)", cntOk);

    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    worker = nullptr;
    thread = nullptr;
}