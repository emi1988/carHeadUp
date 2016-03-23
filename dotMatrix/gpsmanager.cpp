#include "gpsmanager.h"

gpsManager::gpsManager()
{

      m_serialPort = new QSerialPort(this);

      //connect to gps-receiver
      if(getSerialPortSettings() == true)
      {
          //gps receiver found, so open serial port
          openSerialPort();
          connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReceived()));
          connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));

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

                ui->textEditOutput->append("GPS-receiver found on port:" + info.portName());
                ui->textEditOutput->append("description: " + info.description());
                ui->textEditOutput->append("manufacturer: " + info.manufacturer());

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
        ui->textEditOutput->append( "GPS-receiver not found ): ");
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
         ui->textEditOutput->append("serial port successfully opened");
         return true;
     }
     else
     {
         qDebug()<<"could not open serial port ): ";
         ui->textEditOutput->append("could not open serial port ): ");

         return false;
     }

}

void gpsManager::handleError(QSerialPort::SerialPortError error)
{
    qDebug() << "QSerialPort Error Number" << error;
}

void gpsManager::serialDataReceived()
{
    //TODO: copy from gspCar-Project and modify to get the speed
}
