#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QComboBox>

#include <algorithm>

#include <QDebug>

using namespace std;

typedef pair<string, Pair> value_type;

bool cmp(value_type p1, value_type p2){
    return p1.second.amount / p1.second.number > p2.second.amount / p2.second.number;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox->addItem("PM25");
    ui->comboBox->addItem("PM10");
    ui->comboBox->addItem("NO2");
    ui->comboBox->addItem("CO");
    ui->comboBox->addItem("O3");
    ui->comboBox->addItem("SO2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runButton_clicked()
{
    int index = ui->comboBox->currentIndex();
    //save the hashtable into a vector to sort
    vector<value_type> resultVector;
    for(auto iter = hashTable[index].begin(); iter != hashTable[index].end(); ++iter){
        if(iter->second.number != 0)
            resultVector.push_back(*iter);
    }
    sort(resultVector.begin(), resultVector.end(), cmp);

    QBarSeries *series = new QBarSeries();
    for(int i = 0; i < 5; i++) {
        QString label = QString::fromStdString(resultVector[i].first);
        QBarSet *set0 = new QBarSet(label);
        *set0 << (resultVector[i].second.amount / resultVector[i].second.number);
        series->append(set0);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Top 5 stations with most selected pollutant");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->graphicsView->setChart(chart);
}


void MainWindow::on_selectButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    QFile inputFile(filename);
    if(!inputFile.open(QIODevice::ReadOnly)) return;
    QTextStream inputTextStream(&inputFile);

    // load the file and save it into a hashtable
    QStringList rows = inputTextStream.readAll().split("\n");
    for(int pollu = 0; pollu < 6; ++pollu){
        for(int i = 1; i < rows.count() - 1; ++i){
            QStringList row = rows[i].split(",");
            string key = row.at(0).toStdString();
            QString value = row.at(pollu + 2);
            if(value.toStdString() == "NULL"){
                hashTable[pollu][key].number++;
            } else {
                hashTable[pollu][key].amount += value.toFloat();
                hashTable[pollu][key].number++;
            }
        }
    }
    inputFile.close();
    QMessageBox::information(this, "Information", "Succeed loading data!");
    ui->clearButton->setEnabled(true);
    ui->selectButton->setEnabled(false);
    ui->runButton->setEnabled(true);
}


void MainWindow::on_clearButton_clicked()
{
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Question", "Are you sure to clear?");
    if(answer == QMessageBox::No) return;
    for(int pollu = 0; pollu < 6; ++pollu){
        hashTable[pollu].clear();
    }
    ui->clearButton->setEnabled(false);
    ui->selectButton->setEnabled(true);
    ui->runButton->setEnabled(false);
}

