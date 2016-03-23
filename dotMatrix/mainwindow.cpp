#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>

#include <wiringPiSPI.h>


#define CHANNEL 0

uint8_t buf[2];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uint8_t on[] = { 0xFF, 0xFF };
    uint8_t off[] = { 0xFF, 0x00 };
    uint8_t buf[2];

    if (wiringPiSPISetup(CHANNEL, 1000000) < 0) {
            fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
            exit(errno);
        }

//    //matrix init
//    uint8_t initSettings[] = { MAX7219_REG_SCANLIMIT, 0x07 , MAX7219_REG_DECODEMODE, 0x00 , MAX7219_REG_DISPLAYTEST, 0x00, MAX7219_REG_SHUTDOWN, 0x01};
//    memcpy(buf, initSettings ,8);
    //    wiringPiSPIDataRW(CHANNEL, buf, 8);

    //matrix init
    spi(MAX7219_REG_SHUTDOWN, 0);
    spi(MAX7219_REG_SCANLIMIT, 7);    // show all 8 digits
    spi(MAX7219_REG_DECODEMODE, 0);   // use matrix (not digits)
    spi(MAX7219_REG_DISPLAYTEST, 0);  // no display test
    spi(MAX7219_REG_SHUTDOWN, 1);

    clear();

    spi(8, binToDec("0101") );
    spi(7, binToDec("1011") );
    spi(6, binToDec("1101") );

    //    spi(0x0B,0x07);
//    spi(0x09,0x00);
//    spi(0x0C,0x00);
//    spi(0x0A,0xFF);
//    spi(0x0C,0x01);
//    spi(0xFF, 0xFF);
//    spi(0xFF, 0x00);

//    spi(1,0xFF);
//    spi(2,2);
//    spi(3,3);
//    spi(4,4);
//    spi(5,5);
//    spi(6,6);
//    spi(7,7);
//    spi(8,9);




//    for (;;) {
//            memcpy(buf, on, 2);
//            wiringPiSPIDataRW(CHANNEL, buf, 2);
//            sleep(1);
//            memcpy(buf, off, 2);
//            wiringPiSPIDataRW(CHANNEL, buf, 2);
//            sleep(1);
//        }

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
         tmpNumber.append(010);
         tmpNumber.append(110);
         tmpNumber.append(010);
         tmpNumber.append(010);
         tmpNumber.append(111);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //2:
         tmpNumber.append(010);
         tmpNumber.append(101);
         tmpNumber.append(001);
         tmpNumber.append(010);
         tmpNumber.append(111);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //3:
         tmpNumber.append(111);
         tmpNumber.append(001);
         tmpNumber.append(011);
         tmpNumber.append(001);
         tmpNumber.append(111);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //4:
         tmpNumber.append(101);
         tmpNumber.append(101);
         tmpNumber.append(111);
         tmpNumber.append(001);
         tmpNumber.append(001);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //5:
         tmpNumber.append(111);
         tmpNumber.append(100);
         tmpNumber.append(111);
         tmpNumber.append(001);
         tmpNumber.append(111);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //6:
         tmpNumber.append(111);
         tmpNumber.append(100);
         tmpNumber.append(111);
         tmpNumber.append(101);
         tmpNumber.append(111);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //7:
         tmpNumber.append(111);
         tmpNumber.append(001);
         tmpNumber.append(010);
         tmpNumber.append(100);
         tmpNumber.append(100);
         m_numberHash.insert(1, tmpNumber);
         tmpNumber.clear();

        //8:
         tmpNumber.append(111);
         tmpNumber.append(101);
         tmpNumber.append(111);
         tmpNumber.append(101);
         tmpNumber.append(111);
         m_numberHash.insert(8, tmpNumber);
         tmpNumber.clear();

        //9:
         tmpNumber.append(111);
         tmpNumber.append(101);
         tmpNumber.append(111);
         tmpNumber.append(001);
         tmpNumber.append(111);
         m_numberHash.insert(9, tmpNumber);
         tmpNumber.clear();

        //0:
         tmpNumber.append(111);
         tmpNumber.append(101);
         tmpNumber.append(101);
         tmpNumber.append(101);
         tmpNumber.append(111);
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
