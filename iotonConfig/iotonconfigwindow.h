/**
 ******************************************************************************
 *
 * iotonconfigwindow.h - Header file of ioTon Config (iotonconfigwindow.cpp)
 *     https://github.com/iotontech/ioton-config
 *
 * Copyright (c) 2016 Ioton Technology - http://ioton.cc
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License.
 * To view a copy of this license, visit:
 *
 *     https://creativecommons.org/licenses/by-sa/4.0/
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef IOTONCONFIGWINDOW_H
#define IOTONCONFIGWINDOW_H

/* Includes -----------------------------------------------------------------*/
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QMainWindow>
#include <QComboBox>
#include <QLayout>
#include <QTreeWidgetItem>
#include <QThread>


/* Exported constants -------------------------------------------------------*/
#define N_PINS 26

#define NONE 1

#define EVEN    2
#define ODD     3
#define FORCED1 4
#define FORCED0 5

#define PULLNONE    2
#define PULLUP      3
#define PULLDOWN    4
#define OPENDRAIN   5

#define RISE    1
#define FALL    2

#define ADC     0
#define CAN     1
#define DAC     2
#define I2C     3
#define INPUT   4
#define OUTPUT  5
#define PWM     6
#define SDIO    7
#define SPI     8
#define UART    9


/** IotonConfigWindow ********************************************************/

namespace Ui {
class IotonConfigWindow;
}

class IotonConfigWindow : public QMainWindow
{
    Q_OBJECT

public:

    QString pinModeMap[10] = {"ADC", "CAN", "DAC", "I2C", "INPUT", "OUTPUT", "PWM", "SDIO", "SPI", "UART"};

    struct i2c
    {
        int frequency = 100000;
    } i2c;

    struct input
    {
        bool externalInterrupt = false;
        int mode = PULLNONE;
        int edge = RISE;
    } input;

    struct pwm
    {
        int frequency = 50;
        int dutyCycle = 0;
    } pwm;

    struct spi
    {
        int frequency = 1000000;
        int mode = 0;
        int bits = 8;
    } spi;

    struct uart
    {
        int baud = 9600;
        int bits = 8;
        int stopBit = 1;
        int parity = NONE;
    } uart;

    struct pin
    {
        int pinModeUsed = -1;
        QString name = "default";
        struct input input;
        struct uart uart;
        struct pwm pwm;
        struct i2c i2c;
        struct spi spi;
    };

    struct pin pins[26];
    struct pin defaultPin;

    explicit IotonConfigWindow(QWidget *parent = 0);
    ~IotonConfigWindow();

private slots:
    void on_comboBox_0_currentIndexChanged(const QString &arg1);

    void on_comboBox_1_currentIndexChanged(const QString &arg1);

    void on_comboBox_5_currentIndexChanged(const QString &arg1);

    void on_comboBox_6_currentIndexChanged(const QString &arg1);

    void on_comboBox_7_currentIndexChanged(const QString &arg1);

    void on_comboBox_8_currentIndexChanged(const QString &arg1);

    void on_comboBox_19_currentIndexChanged(const QString &arg1);

    void on_comboBox_20_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_3_currentIndexChanged(const QString &arg1);

    void on_comboBox_4_currentIndexChanged(const QString &arg1);

    void on_comboBox_9_currentIndexChanged(const QString &arg1);

    void on_comboBox_10_currentIndexChanged(const QString &arg1);

    void on_comboBox_11_currentIndexChanged(const QString &arg1);

    void on_comboBox_12_currentIndexChanged(const QString &arg1);

    void on_comboBox_13_currentIndexChanged(const QString &arg1);

    void on_comboBox_14_currentIndexChanged(const QString &arg1);

    void on_comboBox_15_currentIndexChanged(const QString &arg1);

    void on_comboBox_16_currentIndexChanged(const QString &arg1);

    void on_comboBox_17_currentIndexChanged(const QString &arg1);

    void on_comboBox_18_currentIndexChanged(const QString &arg1);

    void on_comboBox_21_currentIndexChanged(const QString &arg1);

    void on_comboBox_22_currentIndexChanged(const QString &arg1);

    void on_comboBox_23_currentIndexChanged(const QString &arg1);

    void on_comboBox_24_currentIndexChanged(const QString &arg1);

    void on_comboBox_25_currentIndexChanged(const QString &arg1);

    void on_comboBox_advanced_currentIndexChanged(const QString &arg1);

    void on_lineEdit_advanced_name_editingFinished();

    void on_lineEdit_advanced_1_editingFinished();

    void on_lineEdit_advanced_2_editingFinished();

    void on_comboBox_advanced_1_currentIndexChanged(const QString &arg1);

    void on_comboBox_advanced_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_advanced_3_currentIndexChanged(const QString &arg1);

    void on_comboBox_advanced_4_currentIndexChanged(const QString &arg1);

    void on_checkBox_advanced_1_clicked();

    void on_checkBox_advanced_2_clicked();

    void on_radioButton_expand_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_createProject_clicked();

    void on_actionOpen_project_triggered();

    void on_actionSave_project_triggered();

    void on_actionClear_Pins_triggered();

    void on_actionPinout_Guide_triggered();

    void on_actionManual_TON_triggered();

    void on_actionAbout_Ioton_Config_triggered();

private:
    Ui::IotonConfigWindow *ui;

    int numAdcAvailable = 12;
    int numPwmAvailable = 12;
    int numUartAvailable = 4;

    bool openFile = false;

    QList<QTreeWidgetItem *> treeViewItems;

    int getAdvancedPinIndex(int pinNumber);

    int getAdvancedPinNumber(QString comboBoxText);

    int getAdvancedPinMode(QString comboBoxText);

    void fillPinConfigurations(int pinNumber, int pinMode);

    void savePinConfigurations(int pinNumber, int pinMode);

    void updateStatusBar(void);

    void updatePinConfigurations(int pinNumber, int pinMode);

    void updateTreeViewItem(int pinNumber, int pinMode);

    void pinNameDefault(void);

    void toggleAdvancedOff(void);
};

#endif // IOTONCONFIGWINDOW_H
