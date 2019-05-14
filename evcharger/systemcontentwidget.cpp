/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName:
* Author: Sunny.Guo
* Version: 1.0
* Date:
* Description:
*
* History:
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle
*
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#include "systemcontentwidget.h"
#include "mxde.h"
#include "constant.h"
#include "mxapplication.h"
#include <QZXing.h>

#include <QObject>
#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSignalMapper>
#include <QDirIterator>
#include <QStringList>
#include <QPalette>
#define TTY_RS485_MODE			1
#define TTY_RS232_MODE			0

const QString SystemContentWidget::cmsCode = "20190508";

SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
{
    openFlag = false;

    m_width = width;
    if(m_width <=0){
        m_width = DEFAULT_SCREEN_WIDTH;
    }
    m_height = height;
    if(m_height <= 0){
        m_height = DEFAULT_SCREEN_HEIGHT;
    }

    this->setFixedSize(m_width, m_height);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);
}

SystemContentWidget::~SystemContentWidget()
{
    qDebug() << "SystemContentWidget ..." << m_serial_fd;
    m_mxde->callCloseSerialPort(m_serial_fd);

}

void SystemContentWidget::SerialPortInit()
{
    m_serial_fd = 0;
    QString str = m_mxde->callGetSerialList();
    if((str!=NULL) &&(str.length() < 2)){
        return;
    }

    QStringList list = str.split("\n");
    m_serial_num = list.count()-1;
    qDebug() << "m_serial_num: " << m_serial_num ;


    for(int i=0;i<m_serial_num;i++)
    {


       m_serial_name[i] = strdup(list.at(i).toLocal8Bit().data());

       qDebug("%s",m_serial_name[i]);
    }
}

void SystemContentWidget::initUI()
{
    vLayoutList *vllist = nullptr;
    layoutIdx = layoutBeingCharging;

    /* idle screen title */
    mIdleTitleWidget = new QWidget();
    mIdleTitleLayout = new QGridLayout(mIdleTitleWidget);
    mIdleTitleLabel = new QLabel(mIdleTitleWidget);
    mIdleTitleLabel->setText(tr("Scaning Barcode to Charge"));
    mIdleTitleLabel->setAlignment(Qt::AlignCenter);
    mIdleTitleLabel->setStyleSheet("font-size : 36px");
    mIdleTitleLayout->addWidget(mIdleTitleLabel);
    mIdleTitleWidget->setLayout(mIdleTitleLayout);
    vllist = new struct vLayoutList;
    vllist->w = mIdleTitleWidget;
    vllist->p = 1;
    mWidgetList[layoutIdle].append(vllist);
    vllist = nullptr;

    /* idle screen barcode */
    mIdleBarcodeWidget = new QWidget();
    mIdleBarcodelayout = new QGridLayout(mIdleBarcodeWidget);
    mIdleBarcodeLabel = new QLabel(mIdleBarcodeWidget);
    QImage barcode = QZXing::encodeData(cmsCode, QZXing::EncoderFormat_QR_CODE,
                                    QSize(260, 260), QZXing::EncodeErrorCorrectionLevel_H);
    mIdleBarcodeLabel->setPixmap(QPixmap::fromImage(barcode));
    mIdleBarcodeLabel->setAlignment(Qt::AlignCenter);
    mIdleBarcodelayout->addWidget(mIdleBarcodeLabel);
    mIdleBarcodeWidget->setLayout(mIdleBarcodelayout);
    vllist = new struct vLayoutList;
    vllist->w = mIdleBarcodeWidget;
    vllist->p = 6;
    mWidgetList[layoutIdle].append(vllist);
    vllist = nullptr;

    /* start charging screen title */
    mStartChargingTitleWidget = new QWidget();
    mStartChargingTitleLayout = new QGridLayout(mStartChargingTitleWidget);
    mStartChargingTitleLabel = new QLabel(mStartChargingTitleWidget);
    mStartChargingTitleLabel->setText(tr("Starting Charging"));
    mStartChargingTitleLabel->setAlignment(Qt::AlignCenter);
    mStartChargingTitleLabel->setStyleSheet("font-size : 36px");
    mStartChargingTitleLayout->addWidget(mStartChargingTitleLabel);
    mStartChargingTitleWidget->setLayout(mStartChargingTitleLayout);
    vllist = new struct vLayoutList;
    vllist->w = mStartChargingTitleWidget;
    vllist->p = 1;
    mWidgetList[layoutStartCharging].append(vllist);
    vllist = nullptr;

    /* start charging screen movie */
    mStartChargingMovieWidget = new QWidget();
    mStartChargingMovielayout = new QGridLayout(mStartChargingMovieWidget);
    mStartChargingMovieLabel = new QLabel(mStartChargingMovieWidget);
    QMovie *movie = new QMovie(":/res/images/evcharger/startcharging.gif");
    mStartChargingMovieLabel->setMovie(movie);
    movie->start();
    mStartChargingMovieLabel->setAlignment(Qt::AlignCenter);
    mStartChargingMovielayout->addWidget(mStartChargingMovieLabel);
    mStartChargingMovieWidget->setLayout(mStartChargingMovielayout);
    vllist = new struct vLayoutList;
    vllist->w = mStartChargingMovieWidget;
    vllist->p = 6;
    mWidgetList[layoutStartCharging].append(vllist);
    vllist = nullptr;

    /* being charging screen title */
    mBeingChargingTitleWidget = new QWidget();
    mBeingChargingTitleLayout = new QGridLayout(mBeingChargingTitleWidget);
    mBeingChargingTitleLabel = new QLabel(mBeingChargingTitleWidget);
    mBeingChargingTitleLabel->setText(tr("Being Charging"));
    mBeingChargingTitleLabel->setAlignment(Qt::AlignCenter);
    mBeingChargingTitleLabel->setStyleSheet("font-size : 36px");
    mBeingChargingTitleLayout->addWidget(mBeingChargingTitleLabel);
    mBeingChargingTitleWidget->setLayout(mBeingChargingTitleLayout);
    vllist = new struct vLayoutList;
    vllist->w = mBeingChargingTitleWidget;
    vllist->p = 1;
    mWidgetList[layoutBeingCharging].append(vllist);
    vllist = nullptr;

    /* being charging screen data */
    mBeingChargingDataWidget = new QWidget();
    mBeingChargingDatalayout = new QGridLayout(mBeingChargingDataWidget);

    mTimeElapsedLabel = new QLabel(mBeingChargingDataWidget);
    mTimeElapsedLabel->setText(tr("Time Elapsed(M):"));
    mTimeElapsedLabel->setAlignment(Qt::AlignRight);
    mTimeElapsedValue = new QLineEdit(mBeingChargingDataWidget);
    mTimeElapsedValue->setText(tr("10"));
    mTimeElapsedValue->setFocusPolicy(Qt::NoFocus);

    mMaxCellLabel = new QLabel(mBeingChargingDataWidget);
    mMaxCellLabel->setText(tr("Max Cell Voltage(V):"));
    mMaxCellLabel->setAlignment(Qt::AlignRight);
    mMaxCellValue = new QLineEdit(mBeingChargingDataWidget);
    mMaxCellValue->setText(tr("12.1"));
    mMaxCellValue->setFocusPolicy(Qt::NoFocus);

    mPowerConsumptionLabel = new QLabel(mBeingChargingDataWidget);
    mPowerConsumptionLabel->setText(tr("Power Consumption(kWH):"));
    mPowerConsumptionLabel->setAlignment(Qt::AlignRight);
    mPowerConsumptionValue = new QLineEdit(mBeingChargingDataWidget);
    mPowerConsumptionValue->setText(tr("2.1"));
    mPowerConsumptionValue->setFocusPolicy(Qt::NoFocus);

    mRemainingTimeLabel = new QLabel(mBeingChargingDataWidget);
    mRemainingTimeLabel->setText(tr("Remaining Time(M):"));
    mRemainingTimeLabel->setAlignment(Qt::AlignRight);
    mRemainingTimeValue = new QLineEdit(mBeingChargingDataWidget);
    mRemainingTimeValue->setText(tr("35"));
    mRemainingTimeValue->setFocusPolicy(Qt::NoFocus);

    mChargingVoltageLabel = new QLabel(mBeingChargingDataWidget);
    mChargingVoltageLabel->setText(tr("Charging Voltage(V):"));
    mChargingVoltageLabel->setAlignment(Qt::AlignRight);
    mChargingVoltageValue = new QLineEdit(mBeingChargingDataWidget);
    mChargingVoltageValue->setText(tr("469.1"));
    mChargingVoltageValue->setFocusPolicy(Qt::NoFocus);

    mMaxBattTemperatureLabel = new QLabel(mBeingChargingDataWidget);
    mMaxBattTemperatureLabel->setText(tr("Max Battery Temperature(℃):"));
    mMaxBattTemperatureLabel->setAlignment(Qt::AlignRight);
    mMaxBattTemperatureValue = new QLineEdit(mBeingChargingDataWidget);
    mMaxBattTemperatureValue->setText(tr("36"));
    mMaxBattTemperatureValue->setFocusPolicy(Qt::NoFocus);

    mChargingCurrentLabel = new QLabel(mBeingChargingDataWidget);
    mChargingCurrentLabel->setText(tr("Charging Current(A):"));
    mChargingCurrentLabel->setAlignment(Qt::AlignRight);
    mChargingCurrentValue = new QLineEdit(mBeingChargingDataWidget);
    mChargingCurrentValue->setText(tr("92.3"));
    mChargingCurrentValue->setFocusPolicy(Qt::NoFocus);

    mMinBattTemperatureLabel = new QLabel(mBeingChargingDataWidget);
    mMinBattTemperatureLabel->setText(tr("Min Battery Temperature(℃):"));
    mMinBattTemperatureLabel->setAlignment(Qt::AlignRight);
    mMinBattTemperatureValue = new QLineEdit(mBeingChargingDataWidget);
    mMinBattTemperatureValue->setText(tr("36"));
    mMinBattTemperatureValue->setFocusPolicy(Qt::NoFocus);

    mCurrentSocLabel = new QLabel(mBeingChargingDataWidget);
    mCurrentSocLabel->setText(tr("Current SOC:"));
    mCurrentSocLabel->setAlignment(Qt::AlignRight);
    mCurrentSocValue = new QLineEdit(mBeingChargingDataWidget);
    mCurrentSocValue->setText(tr("68%"));
    mCurrentSocValue->setFocusPolicy(Qt::NoFocus);

    mBatteryTypeLabel = new QLabel(mBeingChargingDataWidget);
    mBatteryTypeLabel->setText(tr("Battery Type:"));
    mBatteryTypeLabel->setAlignment(Qt::AlignRight);
    mBatteryTypeValue = new QLineEdit(mBeingChargingDataWidget);
    mBatteryTypeValue->setText(tr("Lithium iron phosphate"));
    mBatteryTypeValue->setFocusPolicy(Qt::NoFocus);

    /* row 1 */
    mBeingChargingDatalayout->addWidget(mTimeElapsedLabel, 0, 0, 3, 1);
    mBeingChargingDatalayout->addWidget(mTimeElapsedValue, 0, 3, 1, 1);
    mBeingChargingDatalayout->addWidget(mMaxCellLabel, 0, 4, 3, 1);
    mBeingChargingDatalayout->addWidget(mMaxCellValue, 0, 7, 1, 1);

    /* row 2 */
    mBeingChargingDatalayout->addWidget(mPowerConsumptionLabel, 1, 0, 3, 1);
    mBeingChargingDatalayout->addWidget(mPowerConsumptionValue, 1, 3, 1, 1);
    mBeingChargingDatalayout->addWidget(mRemainingTimeLabel, 1, 4, 3, 1);
    mBeingChargingDatalayout->addWidget(mRemainingTimeValue, 1, 7, 1, 1);

    /* row 3 */
    mBeingChargingDatalayout->addWidget(mChargingVoltageLabel, 2, 0, 3, 1);
    mBeingChargingDatalayout->addWidget(mChargingVoltageValue, 2, 3, 1, 1);
    mBeingChargingDatalayout->addWidget(mMaxBattTemperatureLabel, 2, 4, 3, 1);
    mBeingChargingDatalayout->addWidget(mMaxBattTemperatureValue, 2, 7, 1, 1);

    /* row 4 */
    mBeingChargingDatalayout->addWidget(mChargingCurrentLabel, 3, 0, 3, 1);
    mBeingChargingDatalayout->addWidget(mChargingCurrentValue, 3, 3, 1, 1);
    mBeingChargingDatalayout->addWidget(mMinBattTemperatureLabel, 3, 4, 3, 1);
    mBeingChargingDatalayout->addWidget(mMinBattTemperatureValue, 3, 7, 1, 1);

    /* row 5 */
    mBeingChargingDatalayout->addWidget(mCurrentSocLabel, 4, 0, 3, 1);
    mBeingChargingDatalayout->addWidget(mCurrentSocValue, 4, 3, 1, 1);
    mBeingChargingDatalayout->addWidget(mBatteryTypeLabel, 4, 4, 3, 1);
    mBeingChargingDatalayout->addWidget(mBatteryTypeValue, 4, 7, 1, 1);

    mBeingChargingDataWidget->setLayout(mBeingChargingDatalayout);
    vllist = new struct vLayoutList;
    vllist->w = mBeingChargingDataWidget;
    vllist->p = 5;
    mWidgetList[layoutBeingCharging].append(vllist);
    vllist = nullptr;

    /* being charging screen end button */
    mBeingChargingEndWidget = new QWidget();
    mBeingChargingEndLayout = new QGridLayout(mBeingChargingEndWidget);
    mBeingChargingEndBtn = new QPushButton(tr("Ending Charging"), mBeingChargingEndWidget);
    mBeingChargingEndBtn->setFocusPolicy(Qt::NoFocus);
    mBeingChargingEndLayout->addWidget(mBeingChargingEndBtn,0,0,Qt::AlignHCenter);
    mBeingChargingEndWidget->setLayout(mBeingChargingEndLayout);
    vllist = new struct vLayoutList;
    vllist->w = mBeingChargingEndWidget;
    vllist->p = 1;
    mWidgetList[layoutBeingCharging].append(vllist);
    vllist = nullptr;

    /* stop charging screen title */
    mStopChargingTitleWidget = new QWidget();
    mStopChargingTitleLayout = new QGridLayout(mStopChargingTitleWidget);
    mStopChargingTitleLabel = new QLabel(mStopChargingTitleWidget);
    mStopChargingTitleLabel->setText(tr("Stopping Charging"));
    mStopChargingTitleLabel->setAlignment(Qt::AlignCenter);
    mStopChargingTitleLabel->setStyleSheet("font-size : 36px");
    mStopChargingTitleLayout->addWidget(mStopChargingTitleLabel);
    mStopChargingTitleWidget->setLayout(mStopChargingTitleLayout);
    vllist = new struct vLayoutList;
    vllist->w = mStopChargingTitleWidget;
    vllist->p = 1;
    mWidgetList[layoutStopCharging].append(vllist);
    vllist = nullptr;

    /* stop charging screen image */
    mStopChargingImageWidget = new QWidget();
    mStopChargingImagelayout = new QGridLayout(mStopChargingImageWidget);
    mStopChargingImageLabel = new QLabel(mStopChargingImageWidget);
    QPixmap label_pixmap(":/res/images/evcharger/stopcharging.png");
    mStopChargingImageLabel->setPixmap(label_pixmap);
    mStopChargingImageLabel->setAlignment(Qt::AlignCenter);
    mStopChargingImagelayout->addWidget(mStopChargingImageLabel);
    mStopChargingImageWidget->setLayout(mStopChargingImagelayout);
    vllist = new struct vLayoutList;
    vllist->w = mStopChargingImageWidget;
    vllist->p = 6;
    mWidgetList[layoutStopCharging].append(vllist);
    vllist = nullptr;

    mainLayout = new QVBoxLayout;
    SerialPortInit();
    openSerial();
    this->setLayout(mainLayout);
    layoutAddWidgetByList(mWidgetList[layoutIdx]);
}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}

void SystemContentWidget::setDbusProxy(MxDE *mxde)
{
    m_mxde = mxde;
}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

void SystemContentWidget::initConnection()
{

    connect(mBeingChargingEndBtn, SIGNAL(clicked()), this, SLOT(on_endChargingPushButton_clicked()));
}

void SystemContentWidget::display()
{

}

void SystemContentWidget::setCurrentConfigure(QStringList list)
{
    QString rateStr= strdup(list.at(2).toLocal8Bit().data());
    QString checkBitStr = strdup(list.at(6).toLocal8Bit().data());
    QString dataBitStr = strdup(list.at(3).toLocal8Bit().data());
    QString stopBbitStr = strdup(list.at(7).toLocal8Bit().data());

    QMessageBox::information(this, tr("Warning"), tr("Serial had opened!Now use the opened configuration!"));

}
void SystemContentWidget::openSerial()
{
    if(!openFlag)
    {
        QString serialPortStr = "/dev/ttymxc1";
        QString rateStr = "115200";
        QString checkBitStr = "NONE";
        QString dataBitStr = "8";
        QString stopBbitStr = "1";
        qDebug() << "serialPortStr: " << serialPortStr;
        qDebug() << "port: " << serialPortStr;
        qDebug() << "rate: " << rateStr;
        qDebug() << "check: " << checkBitStr;
        qDebug() << "data: " << dataBitStr;
        qDebug() << "stop: " << stopBbitStr;

        QString tty_configure;
        int fd;
        fd = m_mxde->callOpenSerialPort(serialPortStr,tty_configure);
        qDebug() << "open ret: " << fd;
        if(0 == fd)
        {
            QStringList list = tty_configure.split(" ");
            m_serial_fd = list.at(1).toInt();
            qDebug() << "m_serial_fd: " << m_serial_fd;
            qDebug() << "tty_configure: " <<  tty_configure;

            setCurrentConfigure(list);
            openFlag = true;

        }
        else
        {

            QString serial_param;
            int serial_mode = TTY_RS232_MODE;
            int tty_flow = 0;

            m_serial_fd = fd;
            checkBitStr = "NONE";
            QByteArray check = checkBitStr.toLatin1();
            serial_param.sprintf("%d %d %d %d %d %s %d",m_serial_fd,rateStr.toInt(),dataBitStr.toInt(), serial_mode, tty_flow,check.data(),stopBbitStr.toInt());
            m_mxde->callSetSerialPort(serial_param);

            openFlag = true;
        }
    }
    else    // 关闭串口
    {
        m_mxde->callCloseSerialPort(m_serial_fd);
        m_serial_fd = 0;
        openFlag = false;
        qDebug() << "closed ...";
    }
}
void SystemContentWidget::closeDevice()
{
    qDebug() << "closeDevice ..." << m_serial_fd;
    m_mxde->callCloseSerialPort(m_serial_fd);
    m_serial_fd = 0;
}

void SystemContentWidget::layoutClear()
{
    QLayoutItem *item = nullptr;
    while(nullptr!=(item = mainLayout->takeAt(0)))
    {
        item->widget()->close();
        item = nullptr;
    }
}
void SystemContentWidget::layoutAddWidgetByList(QList<vLayoutList*>wList)
{
    layoutClear();
    for(int i=0;i<wList.size();++i)
    {
        auto var = wList.at(i);
        var->w->show();
        mainLayout->addWidget(var->w, var->p);
    }
}

void SystemContentWidget::on_endChargingPushButton_clicked()
{
    layoutIdx = (layoutIdx+1)%layoutTotal;
    layoutAddWidgetByList(mWidgetList[layoutIdx]);
}

void SystemContentWidget::onSerialRecvData(int uart_fd, const QString &data, int size)
{
    if(m_serial_fd == uart_fd )
    {
        QByteArray ba = data.toUtf8();
        char c = ba.data()[ba.size()-1];
        if (isdigit(c))
        {
            if ((c > '0') && (c <= '0' + layoutTotal))
            {
                layoutIdx = c - '1';
                layoutAddWidgetByList(mWidgetList[layoutIdx]);
            }
        }
    }
}
