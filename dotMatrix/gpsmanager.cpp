#include "gpsmanager.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDir>
#include <QTimer>



gpsManager::gpsManager(int secondsBetweenData)
{
    m_secondsBetweenData = secondsBetweenData;

    m_lastSendetTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

    m_serialPort = new QSerialPort(this);

      //connect to gps-receiver
      connectToGpsReceiver();

}

void gpsManager::connectToGpsReceiver()
{
    if(getSerialPortSettings() == true)
    {
        //gps receiver found, so open serial port
        bool connected = openSerialPort();
        if(connected == true)
        {
            connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReceived()));
            connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
        }
        else
        {
            //try to reconnect in 0,8 seconds
             QTimer::singleShot(800, this, SLOT(reConnectToGpsReceiver()));
        }

    }
    else
    {
        //try to reconnect in 0,8 seconds
         QTimer::singleShot(800, this, SLOT(reConnectToGpsReceiver()));
    }
}

bool gpsManager::getSerialPortSettings()
{
    //go through all aviable serial ports
        bool foundGPSreceiver = false;
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            //the number 8963 (in hex) is the ID from my GPS-receiver
            if(info.productIdentifier() == 8963 )
            {
                m_currentPortSettings.portName = info.portName();
                foundGPSreceiver = true;
                qDebug() <<"GPS-receiver found on port:" << info.portName();
                qDebug() << "description: " << info.description();
                qDebug() << "manufacturer: " << info.manufacturer();

                break;
            }
        }

    if(foundGPSreceiver == true)
    {
        //set all other settings

        m_currentPortSettings.baudRate = QSerialPort::Baud4800;
        m_currentPortSettings.dataBits = QSerialPort::Data8;
        m_currentPortSettings.flowControl = QSerialPort::NoFlowControl;
        m_currentPortSettings.parity = QSerialPort::NoParity;
        m_currentPortSettings.stopBits = QSerialPort::OneStop;

    }
    else
    {
        qDebug() << "GPS-receiver not found ): ";
    }

        return foundGPSreceiver;

}

bool gpsManager::openSerialPort()
{
    //first set the current settings
    m_serialPort->setPortName(m_currentPortSettings.portName);
    m_serialPort->setBaudRate(m_currentPortSettings.baudRate);
    m_serialPort->setDataBits(m_currentPortSettings.dataBits);
    m_serialPort->setFlowControl(m_currentPortSettings.flowControl);
    m_serialPort->setParity(m_currentPortSettings.parity);
    m_serialPort->setStopBits(m_currentPortSettings.stopBits);


     if (m_serialPort->open(QIODevice::ReadWrite))
     {
         qDebug()<<"serial port successfully opened";
         return true;
     }
     else
     {
         qDebug()<<"could not open serial port ): ";

         return false;
     }

}

void gpsManager::handleError(QSerialPort::SerialPortError error)
{
    qDebug() << "QSerialPort Error Number" << error;

    //disconnect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    //delete m_serialPort;
    m_serialPort = new QSerialPort(this);
    //connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));

    emit speedAviable(-1);

    connectToGpsReceiver();
}

void gpsManager::serialDataReceived()
{
    QString currentReceivedData = QString(m_serialPort->readAll());

    //add the buffered data from the last receive run
    currentReceivedData.prepend(m_receiveBuffer);

    //search for the start-character "$"
    QStringList currentGPSsentences = currentReceivedData.split("$");
    //save the last incomplete part in the buffer
    m_receiveBuffer.clear();
    m_receiveBuffer.append(currentGPSsentences.last());
    //remove the last incomplete sentence from the buffer
    currentGPSsentences.removeLast();

    //go through all sentences

    foreach (QString currentSentence, currentGPSsentences)
    {
        QStringList singleSentenceData = currentSentence.split(",");

        //just use the GPGGA-Information and check if it's already received completely
        if((singleSentenceData.at(0).compare("GPRMC")== 0) & (singleSentenceData.count() == 13))
        {

            //just save/send the gps-data if the m_msecondsBetweenWebSend is over
            quint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

            if(currentTime > (m_lastSendetTime + m_secondsBetweenData) )
            {
                m_lastSendetTime = currentTime;

                QStringList singleSentenceData = currentSentence.split(",");
                qDebug() << "GPGGA-Data: " << singleSentenceData;


                qDebug() << "RapiTimestamp: " << currentTime;

                //GPGGA-Data:  ("GPGGA", "213225.936", "", "", "", "", "0", "00", "", "", "M", "0.0", "M", "", "0000*5F ")
                //$GPRMC,191410,A,4735.5634,N,00739.3538,E,0.0,0.0,181102,0.4,E,A*19

                //check if received data is empty because of no gps-signal
                QString testData = singleSentenceData.at(3);
                if(testData.compare("") == 0)
                {
                    qDebug() << "gps-data is empty";

                }
                else
                {
                    //gps-data is not empty -> process it

                    float speedKmh = singleSentenceData.at(7).toFloat()*1.852;

                    qDebug() << "speed (km//h): " << speedKmh;

                    emit speedAviable((int) speedKmh);


                }
            }
        }
    }

    //qDebug() << currentGPSsentences;

}

void gpsManager::reConnectToGpsReceiver()
{
    connectToGpsReceiver();
}
