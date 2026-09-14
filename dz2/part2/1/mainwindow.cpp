#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QGroupBox>
#include <QFormLayout>

using namespace std;

MainWindow::MainWindow() {
    setWindowTitle("Epidemic simulation");
    resize(800, 600);

    // --- Управление ---
    fileLabel = new QLabel("No file loaded");
    auto* loadBtn = new QPushButton("Load data...");

    p1Edit    = new QLineEdit("0.3");
    p2Edit    = new QLineEdit("0.1");
    stepsEdit = new QLineEdit("1000");
    delayEdit = new QLineEdit("50");

    startBtn = new QPushButton("Start");
    stopBtn  = new QPushButton("Stop");
    stopBtn->setEnabled(false);

    auto* form = new QFormLayout;
    form->addRow("p1 (infect):",  p1Edit);
    form->addRow("p2 (recover):", p2Edit);
    form->addRow("max steps:",    stepsEdit);
    form->addRow("delay (ms):",   delayEdit);

    auto* ctrlBox = new QGroupBox("Control");
    auto* ctrlLayout = new QVBoxLayout;
    ctrlLayout->addWidget(fileLabel);
    ctrlLayout->addWidget(loadBtn);
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
    layout->addWidget(log, 2);

    // --- Сигналы ---
    connect(loadBtn,  &QPushButton::clicked, this, &MainWindow::onLoad);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(stopBtn,  &QPushButton::clicked, this, &MainWindow::onStop);
}

void MainWindow::onLoad() {
    QString fn = QFileDialog::getOpenFileName(this, "Open data file");
    if (fn.isEmpty()) return;
    if (!model.load(fn.toStdString())) {
        QMessageBox::warning(this, "Error", "Cannot load file");
        return;
    }
    fileLabel->setText("Loaded: " + fn);
    log->appendPlainText(QString("Loaded N=%1").arg(model.N));
}

void MainWindow::onStart() {
    if (model.N == 0) {
        QMessageBox::warning(this, "Error", "Load data first");
        return;
    }

    model.p1 = p1Edit->text().toDouble();
    model.p2 = p2Edit->text().toDouble();
    model.reset();

    worker = new SimWorker;
    worker->model    = &model;
    worker->maxSteps = stepsEdit->text().toInt();
    worker->delayMs  = delayEdit->text().toInt();

    thread = new QThread(this);
    worker->moveToThread(thread);

    connect(thread, &QThread::started,  worker, &SimWorker::run);
    connect(worker, &SimWorker::tick,   this,   &MainWindow::onTick);
    connect(worker, &SimWorker::finished, this, &MainWindow::onFinished);
    connect(worker, &SimWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    log->appendPlainText("--- Simulation started ---");

    thread->start();
}

void MainWindow::onStop() {
    if (worker) worker->requestStop();
    stopBtn->setEnabled(false);
}

void MainWindow::onTick(int step, int h, int s, int r) {
    log->appendPlainText(
        QString("Step %1: healthy=%2 sick=%3 recovered=%4")
            .arg(step).arg(h).arg(s).arg(r));
}

void MainWindow::onFinished() {
    log->appendPlainText("--- Simulation finished ---");

    auto print = [&](const QString& title, const vector<int>& v) {
        QString s = title + ": ";
        for (int x : v) s += QString::number(x) + " ";
        log->appendPlainText(s);
    };

    print("Never got sick", model.neverSick());
    print("Recovered", model.recovered());
    print("Recovered whose neighbours did not recover",
          model.recoveredWithNotRecoveredNeighbours());
    print("Healthy with all sick neighbours",
          model.healthyWithAllSickNeighbours());

    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    worker = nullptr;
    thread = nullptr;
}