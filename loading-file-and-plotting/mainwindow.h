#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <unordered_map>
#include <QMessageBox>

using namespace std;

struct Pair
{
    float amount = 0;
    int number = 0;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_runButton_clicked();

    void on_selectButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    unordered_map<string, Pair> hashTable[6];
};
#endif // MAINWINDOW_H
