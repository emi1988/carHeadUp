#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpsmanager.h"

//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>

#include <wiringPiSPI.h>


#define CHANNEL 0

uint8_t buf[2];

void MainWindow::matrixInit()
{
    spi(MAX7219_REG_SHUTDOWN, 0);
    spi(MAX7219_REG_SCANLIMIT, 7);    // show all 8 digits
    spi(MAX7219_REG_DECODEMODE, 0);   // use matrix (not digits)
    spi(MAX7219_REG_DISPLAYTEST, 0);  // no display test
    spi(MAX7219_REG_SHUTDOWN, 1);
    spi(MAX7219_REG_INTENSITY, 15);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (wiringPiSPISetup(CHANNEL, 1000000) < 0) {
        fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
        exit(errno);
    }

    //init the font for displaying the numbers
    initFont();
    matrixInit();
    clear();

    gpsManager *myGpsManager= new gpsManager(1);

    connect(myGpsManager, SIGNAL(speedAviable(int)), this, SLOT(gpsSpeedReceived(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::spi(uint8_t reg, uint8_t val) {
    buf[0] = reg;
    buf[1] = val;
    wiringPiSPIDataRW(CHANNEL, buf, 2);
    //	usleep(10);
}

void MainWindow::clear()
{
    spi(1,0);
    spi(2,0);
    spi(3,0);
    spi(4,0);
    spi(5,0);
    spi(6,0);
    spi(7,0);
    spi(8,0);

}

int MainWindow::binToDec(QString bin)
{
    bool ok;
    return bin.toLongLong(&ok, 2);
}

void MainWindow::initFont()
{
    QStringList tmpNumber;

    //1:
         tmpNumber.append("010");
         tmpNumber.append("110");
         tmpNumber.append("010");
         tmpNumber.append("010");
         tmpNumber.append("111");
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //2:
         tmpNumber.append("010");
         tmpNumber.append("101");
         tmpNumber.append("001");
         tmpNumber.append("010");
         tmpNumber.append("111");
         m_numberHash.insert(2, tmpNumber);
         tmpNumber.clear();

        //3:
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("011");
         tmpNumber.append("001");
         tmpNumber.append("111");
         m_numberHash.insert(3, tmpNumber);
         tmpNumber.clear();

        //4:
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("001");
         m_numberHash.insert(4, tmpNumber);
         tmpNumber.clear();

        //5:
         tmpNumber.append("111");
         tmpNumber.append("100");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("111");
         m_numberHash.insert(5, tmpNumber);
         tmpNumber.clear();

        //6:
         tmpNumber.append("111");
         tmpNumber.append("100");
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         m_numberHash.insert(6, tmpNumber);
         tmpNumber.clear();

        //7:
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("010");
         tmpNumber.append("100");
         tmpNumber.append("100");
         m_numberHash.insert(7, tmpNumber);
         tmpNumber.clear();

        //8:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         m_numberHash.insert(8, tmpNumber);
         tmpNumber.clear();

        //9:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("111");
         tmpNumber.append("001");
         tmpNumber.append("111");
         m_numberHash.insert(9, tmpNumber);
         tmpNumber.clear();

        //0:
         tmpNumber.append("111");
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("101");
         tmpNumber.append("111");
         m_numberHash.insert(0, tmpNumber);
         tmpNumber.clear();

}


void MainWindow::on_lineEditValue_returnPressed()
{
    int reg = ui->lineEditReg->text().toInt();
    //int value = ui->lineEditValue->text().toInt();
    bool ok;
    int value = ui->lineEditValue->text().toLongLong(&ok, 2);
    spi(reg, value);

}

void MainWindow::on_pushButton_clicked()
{
    clear();
}

void MainWindow::displayNumber(int number)
{

    if((number>=10) & (number<100))
    {
         int number1 = QString::number(number).left(1).toInt();
         int number2 = QString::number(number).right(1).toInt();

         QStringList currentBinaryNumber1 = m_numberHash.value(number1);
         QStringList currentBinaryNumber2 = m_numberHash.value(number2);

         spi(8, binToDec(currentBinaryNumber1.at(0)+"0" +currentBinaryNumber2.at(0)));
         spi(7, binToDec(currentBinaryNumber1.at(1)+"0" +currentBinaryNumber2.at(1)));
         spi(6, binToDec(currentBinaryNumber1.at(2)+"0" +currentBinaryNumber2.at(2)));
         spi(5, binToDec(currentBinaryNumber1.at(3)+"0" +currentBinaryNumber2.at(3)));
         spi(4, binToDec(currentBinaryNumber1.at(4)+"0" +currentBinaryNumber2.at(4)));

    }
    else if ((number<10)& (number>=0))
    {
        QStringList currentBinaryNumber = m_numberHash.value(number);

        spi(8, binToDec(currentBinaryNumber.at(0)));
        spi(7, binToDec(currentBinaryNumber.at(1)));
        spi(6, binToDec(currentBinaryNumber.at(2)));
        spi(5, binToDec(currentBinaryNumber.at(3)));
        spi(4, binToDec(currentBinaryNumber.at(4)));
    }
    else if(number >=100)
    {
        //not implemented jet; waiting for second dot-matrix :)
    }
    else if(number <0)
    {
        //error occured, so clear display
        clear();
    }

}

void MainWindow::gpsSpeedReceived(int speed)
{

    displayNumber(speed);

}

void MainWindow::on_lineEditNumber_returnPressed()
{
    displayNumber(ui->lineEditNumber->text().toInt());
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    displayNumber(position);
    ui->lineEditNumber->setText(QString::number(position));
}
