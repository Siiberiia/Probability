#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMap>

using namespace std;

MainWindow::MainWindow() {
    setWindowTitle("Random walk");
    resize(900, 600);

    startEdit = new QLineEdit("0");
    nEdit     = new QLineEdit("10");
    distLabel = new QLabel("No file loaded");
    loadBtn   = new QPushButton("Load DVR file...");
    startBtn  = new QPushButton("Start");
    stopBtn   = new QPushButton("Stop");
    stopBtn->setEnabled(false);

    auto* form = new QFormLayout;
    form->addRow("Start x:", startEdit);
    form->addRow("Steps n:", nEdit);
    form->addRow("DVR:", distLabel);

    auto* ctrlBox = new QGroupBox("Control");
    auto* ctrlLayout = new QVBoxLayout;
    ctrlLayout->addLayout(form);
    ctrlLayout->addWidget(loadBtn);
    auto* btnRow = new QHBoxLayout;
    btnRow->addWidget(startBtn);
    btnRow->addWidget(stopBtn);
    ctrlLayout->addLayout(btnRow);
    ctrlBox->setLayout(ctrlLayout);

    canvas = new WalkWidget;

    log = new QPlainTextEdit;
    log->setReadOnly(true);
    log->setMaximumHeight(180);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(ctrlBox);
    layout->addWidget(canvas, 1);
    layout->addWidget(new QLabel("Log / final distribution:"));
    layout->addWidget(log);

    timer = new QTimer(this);
    timer->setInterval(16);   // ~60 fps

    connect(loadBtn,  &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(stopBtn,  &QPushButton::clicked, this, &MainWindow::onStop);
    connect(timer,    &QTimer::timeout,     this, &MainWindow::onTick);
}

void MainWindow::onLoadFile() {
    QString fn = QFileDialog::getOpenFileName(this, "Open DVR file");
    if (fn.isEmpty()) return;

    string err;
    if (!rv.load(fn.toStdString(), err)) {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
        return;
    }
    distLabel->setText("Loaded: " + fn);

    log->appendPlainText("DVR loaded. Values and probabilities:");
    for (size_t i = 0; i < rv.values.size(); i++)
        log->appendPlainText(QString("  v=%1  p=%2")
                                 .arg(rv.values[i]).arg(rv.probs[i]));
}

void MainWindow::onStart() {
    if (rv.values.empty()) {
        QMessageBox::warning(this, "Error", "Load DVR file first");
        return;
    }

    startX = startEdit->text().toDouble();
    n      = nEdit->text().toInt();
    if (n < 1) {
        QMessageBox::warning(this, "Error", "n must be >= 1");
        return;
    }

    currentX = startX;
    stepsDone = 0;
    delta = 0;
    animProgress = 0;
    history.clear();
    history.push_back(currentX);

    canvas->clear();
    canvas->setCurrent(currentX);

    // Оценка диапазона для графика
    double maxAbs = 0;
    for (double v : rv.values)
        maxAbs = max(maxAbs, fabs(v));
    double range = maxAbs * n + fabs(startX) + 2;
    canvas->setRange(startX - range, startX + range);

    log->appendPlainText("--- Simulation started ---");
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);

    // Первый шаг запускается сразу
    delta = rv.sample();
    animProgress = 0;
    timer->start();
}

void MainWindow::onStop() {
    timer->stop();
    log->appendPlainText("--- Stopped by user ---");
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}

void MainWindow::onTick() {
    // За 1 секунду точка проходит |delta| (по модулю)
    // Скорость пропорциональна |delta|, поэтому за тик проходим долю:
    animProgress += timer->interval() / 1000.0;

    if (animProgress >= 1.0) {
        // Шаг завершён
        currentX += delta;
        stepsDone++;
        history.push_back(currentX);
        canvas->setHistory(history);
        canvas->setCurrent(currentX);

        log->appendPlainText(QString("Step %1: s=%2, x=%3")
                                 .arg(stepsDone).arg(delta).arg(currentX));

        if (stepsDone >= n) {
            // Все шаги выполнены
            timer->stop();
            finals.push_back(currentX);
            showFinalDistribution();

            log->appendPlainText("--- Finished ---");
            startBtn->setEnabled(true);
            stopBtn->setEnabled(false);
            return;
        }

        // Следующий шаг
        delta = rv.sample();
        animProgress = 0;
    } else {
        // Промежуточное положение
        double x = currentX + delta * animProgress;
        canvas->setCurrent(x);
    }
}

void MainWindow::showFinalDistribution() {
    // Считается, сколько раз точка попала в каждое финальное положение
    QMap<double, int> cnt;
    for (double x : finals) cnt[x]++;

    log->appendPlainText("Final position distribution (over "
                         + QString::number(finals.size()) + " runs):");
    for (auto it = cnt.begin(); it != cnt.end(); ++it) {
        double p = it.value() / (double)finals.size();
        log->appendPlainText(QString("  x=%1  count=%2  P=%3")
                                 .arg(it.key()).arg(it.value()).arg(p, 0, 'f', 4));
    }
}