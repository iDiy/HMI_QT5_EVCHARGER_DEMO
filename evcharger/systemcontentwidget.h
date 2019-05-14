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
#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H
#include "basewidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QList>
#include <QTextEdit>
#include <QComboBox>

#include "mxde.h"
#include "mxapplication.h"

class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
    enum layoutType {layoutIdle, layoutStartCharging, layoutBeingCharging, layoutStopCharging, layoutTotal};
public:
    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void setCurrentConfigure(QStringList list);
    void initUI();
    void initConnection();
    void display();
    void closeDevice();

signals:

public slots:

    void on_endChargingPushButton_clicked();

    void onSerialRecvData(int uart_fd, const QString &data, int size);

private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    bool openFlag;
    bool timerFlag;

    int  m_serial_num;
    int  m_serial_fd;

    char *m_serial_name[5];

    void SerialPortInit();

    QVBoxLayout     *mainLayout;
    /* idle screen */
    QWidget       *mIdleTitleWidget;
    QGridLayout     *mIdleTitleLayout;
    QLabel *mIdleTitleLabel;
    QWidget *mIdleBarcodeWidget;
    QGridLayout *mIdleBarcodelayout;
    QLabel *mIdleBarcodeLabel;
    /* start charging screen */
    QWidget       *mStartChargingTitleWidget;
    QGridLayout     *mStartChargingTitleLayout;
    QLabel *mStartChargingTitleLabel;
    QWidget *mStartChargingMovieWidget;
    QGridLayout *mStartChargingMovielayout;
    QLabel *mStartChargingMovieLabel;
    /* being charging screen */
    QWidget       *mBeingChargingTitleWidget;
    QGridLayout     *mBeingChargingTitleLayout;
    QLabel *mBeingChargingTitleLabel;
    QWidget *mBeingChargingDataWidget;
    QGridLayout *mBeingChargingDatalayout;
    QLabel *mTimeElapsedLabel;
    QLineEdit *mTimeElapsedValue;
    QLabel *mMaxCellLabel;
    QLineEdit *mMaxCellValue;
    QLabel *mPowerConsumptionLabel;
    QLineEdit *mPowerConsumptionValue;
    QLabel *mRemainingTimeLabel;
    QLineEdit *mRemainingTimeValue;
    QLabel *mChargingVoltageLabel;
    QLineEdit *mChargingVoltageValue;
    QLabel *mMaxBattTemperatureLabel;
    QLineEdit *mMaxBattTemperatureValue;
    QLabel *mChargingCurrentLabel;
    QLineEdit *mChargingCurrentValue;
    QLabel *mMinBattTemperatureLabel;
    QLineEdit *mMinBattTemperatureValue;
    QLabel *mCurrentSocLabel;
    QLineEdit *mCurrentSocValue;
    QLabel *mBatteryTypeLabel;
    QLineEdit *mBatteryTypeValue;
    QWidget *mBeingChargingEndWidget;
    QGridLayout *mBeingChargingEndLayout;
    QPushButton *mBeingChargingEndBtn;
    /* stop charging screen */
    QWidget       *mStopChargingTitleWidget;
    QGridLayout     *mStopChargingTitleLayout;
    QLabel *mStopChargingTitleLabel;
    QWidget *mStopChargingImageWidget;
    QGridLayout *mStopChargingImagelayout;
    QLabel *mStopChargingImageLabel;

    const static QString cmsCode;
    struct vLayoutList
    {
        QWidget *w;
        int p;
    };

    QList<vLayoutList*>mWidgetList[layoutTotal];
    int layoutIdx;

    void layoutClear();
    void layoutAddWidgetByList(QList<vLayoutList*>wList);
    void openSerial();
};

#endif // SYSTEMCONTENTWIDGET_H
