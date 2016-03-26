#ifndef GPSMANAGER_H
#define GPSMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QDateTime>
#include "defsGps.h"

class gpsManager :public QObject
{
    Q_OBJECT

public:
    gpsManager( int secondsBetweenData = 1);

private:
    QSerialPort *m_serialPort;
    stSerialPortSettings m_currentPortSettings;

    QString m_receiveBuffer;
    //stGPSdata m_currentGPSdata;
    int m_secondsBetweenData;
    quint64 m_lastSendetTime;

    bool getSerialPortSettings();
    bool openSerialPort();

signals:

    void  speedAviable(int speedKmh);

    private slots:
    void handleError(QSerialPort::SerialPortError error);
    void serialDataReceived();
};

#endif // GPSMANAGER_H
