#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <deque>
using namespace std;

// ---------- PlotWidget ----------

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(500, 400);
}

void PlotWidget::setTrajectories(const QVector<QVector<QPointF>>& t) {
    trajs = t;
    update();
}

void PlotWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
    if (trajs.isEmpty()) return;

    double minX = 1e18, maxX = -1e18, minY = 1e18, maxY = -1e18;
    for (const auto& tr : trajs)
        for (const auto& pt : tr) {
            minX = min(minX, pt.x());
            maxX = max(maxX, pt.x());
            minY = min(minY, pt.y());
            maxY = max(maxY, pt.y());
        }

    if (maxX - minX < 1e-9) maxX = minX + 1;
    if (maxY - minY < 1e-9) { minY -= 1; maxY += 1; }

    int W = width(), H = height(), margin = 40;

    auto toScreen = [&](double x, double y) {
        double sx = margin + (x - minX) / (maxX - minX) * (W - 2 * margin);
        double sy = H - margin - (y - minY) / (maxY - minY) * (H - 2 * margin);
        return QPointF(sx, sy);
    };

    p.setPen(Qt::gray);
    QPointF originX = toScreen(minX, 0);
    QPointF originY = toScreen(0, minY);
    p.drawLine(QPointF(margin, originX.y()), QPointF(W - margin, originX.y()));
    p.drawLine(QPointF(originY.x(), margin), QPointF(originY.x(), H - margin));

    QVector<QColor> colors = {Qt::blue, Qt::red, Qt::darkGreen, Qt::magenta, Qt::darkCyan};
    for (int k = 0; k < trajs.size(); k++) {
        p.setPen(colors[k % colors.size()]);
        p.setBrush(colors[k % colors.size()]);
        const auto& tr = trajs[k];
        for (int i = 1; i < tr.size(); i++)
            p.drawLine(toScreen(tr[i-1].x(), tr[i-1].y()),
                       toScreen(tr[i].x(), tr[i].y()));
        for (const auto& pt : tr)
            p.drawEllipse(toScreen(pt.x(), pt.y()), 1.5, 1.5);
    }
}

// ---------- MainWindow ----------

MainWindow::MainWindow() {
    setWindowTitle("Trajectory simulation");
    resize(1000, 700);

    fileEdit = new QLineEdit("config.json");
    auto* loadBtn = new QPushButton("Load config");
    runBtn = new QPushButton("Run");

    auto* top = new QHBoxLayout;
    top->addWidget(new QLabel("Config:"));
    top->addWidget(fileEdit, 1);
    top->addWidget(loadBtn);
    top->addWidget(runBtn);

    log = new QPlainTextEdit;
    log->setReadOnly(true);
    log->setMaximumHeight(200);

    plot = new PlotWidget;

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(top);
    layout->addWidget(plot, 1);
    layout->addWidget(new QLabel("Log (last K runs):"));
    layout->addWidget(log);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadConfig);
    connect(runBtn,  &QPushButton::clicked, this, &MainWindow::onRun);
}

void MainWindow::onLoadConfig() {
    QString fn = QFileDialog::getOpenFileName(this, "Open JSON config",
                                              fileEdit->text(),
                                              "JSON files (*.json)");
    if (fn.isEmpty()) return;
    fileEdit->setText(fn);

    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) {
        QMessageBox::warning(this, "Error", "Bad JSON");
        return;
    }
    QJsonObject o = doc.object();

    model.h = o.value("h").toDouble(1.0);
    model.n = o.value("n").toInt(100);
    model.Y = o.value("Y").toDouble(5.0);
    model.l = o.value("l").toDouble(0.0);
    K       = o.value("K").toInt(5);

    // Какое распределение использовать
    QString useName = o.value("use").toString("uniform");

    // Все наборы параметров лежат в объекте "distributions"
    QJsonObject dists = o.value("distributions").toObject();
    if (!dists.contains(useName)) {
        QMessageBox::warning(this, "Error",
            QString("No such distribution in config: %1").arg(useName));
        return;
    }
    QJsonObject params = dists.value(useName).toObject()
                             .value("params").toObject();

    if (useName == "uniform") {
        model.dist.kind = DistKind::Uniform;
        model.dist.a = params.value("a").toInt(-1);
        model.dist.b = params.value("b").toInt(2);
    } else if (useName == "binomial") {
        model.dist.kind = DistKind::Binomial;
        model.dist.nBin = params.value("n").toInt(5);
        model.dist.p    = params.value("p").toDouble(0.5);
    } else if (useName == "geometric") {
        model.dist.kind = DistKind::Geometric;
        model.dist.p      = params.value("p").toDouble(0.3);
        model.dist.maxVal = params.value("max").toInt(10);
    } else if (useName == "triangular") {
        model.dist.kind = DistKind::Triangular;
        model.dist.a = params.value("a").toInt(-2);
        model.dist.b = params.value("b").toInt(3);
        model.dist.c = params.value("c").toInt(0);
    } else {
        QMessageBox::warning(this, "Error", "Unknown distribution");
        return;
    }

    log->appendPlainText(QString("Config loaded: h=%1 n=%2 Y=%3 l=%4 K=%5 use=%6")
                             .arg(model.h).arg(model.n).arg(model.Y).arg(model.l)
                             .arg(K).arg(useName));
}

void MainWindow::onRun() {
    if (model.n <= 0) {
        QMessageBox::warning(this, "Error", "Load config first");
        return;
    }

    int totalRuns = 20;
    int crossed = 0;

    QVector<QVector<QPointF>> drawTrajs;
    deque<QString> lastLogs;

    for (int t = 0; t < totalRuns; t++) {
        Trajectory tr = model.run();

        if (tr.crossedAxis || tr.touchedZero)
            crossed++;

        if (t >= totalRuns - K) {
            QVector<QPointF> pts;
            for (const auto& p : tr.points)
                pts.append(QPointF(p.x, p.y));
            drawTrajs.append(pts);
        }

        QString line = QString("Run %1: crossed=%2 touchedZero=%3")
                           .arg(t + 1)
                           .arg(tr.crossedAxis ? "yes" : "no")
                           .arg(tr.touchedZero ? "yes" : "no");
        lastLogs.push_back(line);
        if ((int)lastLogs.size() > K) lastLogs.pop_front();
    }

    double p = crossed / (double)totalRuns;

    log->appendPlainText("=== Run results ===");
    log->appendPlainText(QString("Total runs: %1").arg(totalRuns));
    log->appendPlainText(QString("Crossings:  %1").arg(crossed));
    log->appendPlainText(QString("Empirical P(cross axis): %1").arg(p, 0, 'f', 4));
    log->appendPlainText("--- Last K runs ---");
    for (const auto& s : lastLogs)
        log->appendPlainText(s);

    plot->setTrajectories(drawTrajs);
}