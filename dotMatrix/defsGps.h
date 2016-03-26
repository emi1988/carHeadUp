#ifndef DEFSGPS_H
#define DEFSGPS_H

#include <QString>
#include <QtSerialPort/QSerialPort>

struct stSerialPortSettings
{
    QString portName;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

struct stGPSdata
{
    QString timeStampGPS;
    QString longitude; //längengrad
    QString longitudeAlignment; //E(ast) or W(est)
    QString latitude; //breitengrad
    QString latitudeAlignment; // N(orth) or S(outh)
    QString satelliteAmount;
    QString horizontalPrecision;
    QString altitude;

    QString longitudeDecimal;
    QString latitudeDecimal;
    QString timeStampRapi;
};


#endif // DEFSGPS_H
