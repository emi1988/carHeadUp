#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myDefs.h"


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <QHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void matrixInit();
private slots:
    void on_lineEditValue_returnPressed();
    void on_pushButton_clicked();

    void displayNumber(int number);

    void on_lineEditNumber_returnPressed();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    void spi(uint8_t reg, uint8_t val);
    void clear();
    int binToDec(QString bin);
    void initFont();


    QHash<int, QStringList> m_numberHash;
};

#endif // MAINWINDOW_H
