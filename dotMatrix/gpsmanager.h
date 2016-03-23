#ifndef GPSMANAGER_H
#define GPSMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QNetworkReply>
#include <QDateTime>

class gpsManager
{
public:
    gpsManager();

private:
    QSerialPort *m_serialPort;
    stSerialPortSettings m_currentPortSettings;

    QString m_receiveBuffer;
    stGPSdata m_currentGPSdata;

    bool getSerialPortSettings();
    bool openSerialPort();

signals:

    void  speedAviable(float speedKmh);

    private slots:
    void handleError(QSerialPort::SerialPortError error);
    void serialDataReceived();
};

#endif // GPSMANAGER_H
