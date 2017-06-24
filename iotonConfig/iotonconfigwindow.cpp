/**
 ******************************************************************************
 *
 * iotonconfigwindow.cpp - Implementation of ioTon Config (iotonconfigwindow)
 *  - This is the main window and its signals/functions
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

/* Includes -----------------------------------------------------------------*/
#include "ui_iotonconfigwindow.h"
#include "iotonconfigwindow.h"
#include "iotonconfigproject.h"


/* Constructor --------------------------------------------------------------*/
IotonConfigWindow::IotonConfigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IotonConfigWindow)
{
    ui->setupUi(this);

    // Configure window geometry and layout
    ui->tabWidget->setFixedWidth(2 * ui->comboBox_0->width() + 14 + ui->label_pinoutImage->width());
    ui->tabWidget->setFixedHeight(ui->label_pinoutImage->height() + 35);
    this->setFixedWidth(ui->tabWidget->width() + 22);
    this->setFixedHeight(ui->tabWidget->height() + 79);
    ui->progressBar->setVisible(false);

    // Create items (for each pin) in the treeWidget and hidden them
    for (int i = 0; i < N_PINS; i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        treeViewItems.append(item);
    }
    ui->treeWidget->addTopLevelItems(treeViewItems);
    for (int i = 0; i < N_PINS; i++)
    {
        treeViewItems.at(i)->setHidden(false);
    }

    // Configure pattern for the frequency and duty cycle fields
    QRegExp frequencyRx("(|[1-9][0-9]{0,6}|1)");
    QRegExp dutyCycleRx("(|100|[1-9]?[0-9])");
    ui->lineEdit_advanced_1->setValidator(new QRegExpValidator(frequencyRx, this));
    ui->lineEdit_advanced_2->setValidator(new QRegExpValidator(dutyCycleRx, this));

    // Hidden elements in the advanced tab
    toggleAdvancedOff();
    ui->comboBox_advanced->addItem("");

    // Initializes the options - Pin0
    ui->comboBox_0->addItem("");
    ui->comboBox_0->addItem("INPUT");
    ui->comboBox_0->addItem("OUTPUT");
    ui->comboBox_0->addItem("PWM_1A");
    ui->comboBox_0->addItem("UART_TX1");

    // Initializes the options - Pin1
    ui->comboBox_1->addItem("");
    ui->comboBox_1->addItem("INPUT");
    ui->comboBox_1->addItem("OUTPUT");
    ui->comboBox_1->addItem("PWM_1B");
    ui->comboBox_1->addItem("UART_RX1");

    // Initializes the options - Pin2
    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("INPUT");
    ui->comboBox_2->addItem("OUTPUT");
    ui->comboBox_2->addItem("PWM_1C");
    ui->comboBox_2->addItem("SDIO_D0");

    // Initializes the options - Pin3
    ui->comboBox_3->addItem("");
    ui->comboBox_3->addItem("INPUT");
    ui->comboBox_3->addItem("OUTPUT");
    ui->comboBox_3->addItem("PWM_1D");
    ui->comboBox_3->addItem("SDIO_D1");

    // Initializes the options - Pin4
    ui->comboBox_4->addItem("");
    ui->comboBox_4->addItem("OUTPUT");
    ui->comboBox_4->addItem("INPUT");
    ui->comboBox_4->addItem("PWM_2A");

    // Initializes the options - Pin5
    ui->comboBox_5->addItem("");
    ui->comboBox_5->addItem("OUTPUT");
    ui->comboBox_5->addItem("INPUT");
    ui->comboBox_5->addItem("UART_TX2");
    ui->comboBox_5->addItem("SDIO_D2");

    // Initializes the options - Pin6
    ui->comboBox_6->addItem("");
    ui->comboBox_6->addItem("OUTPUT");
    ui->comboBox_6->addItem("INPUT");
    ui->comboBox_6->addItem("UART_RX2");
    ui->comboBox_6->addItem("SDIO_D3");

    // Initializes the options - Pin7
    ui->comboBox_7->addItem("");
    ui->comboBox_7->addItem("OUTPUT");
    ui->comboBox_7->addItem("INPUT");
    ui->comboBox_7->addItem("UART_TX3");
    ui->comboBox_7->addItem("SDIO_CK");

    // Initializes the options - Pin8
    ui->comboBox_8->addItem("");
    ui->comboBox_8->addItem("OUTPUT");
    ui->comboBox_8->addItem("INPUT");
    ui->comboBox_8->addItem("UART_RX3");
    ui->comboBox_8->addItem("SDIO_CMD");

    // Initializes the options - Pin9
    ui->comboBox_9->addItem("");
    ui->comboBox_9->addItem("OUTPUT");
    ui->comboBox_9->addItem("INPUT");
    ui->comboBox_9->addItem("PWM_2B");

    // Initializes the options - Pin10
    ui->comboBox_10->addItem("");
    ui->comboBox_10->addItem("OUTPUT");
    ui->comboBox_10->addItem("INPUT");
    ui->comboBox_10->addItem("PWM_3A");

    // Initializes the options - Pin11
    ui->comboBox_11->addItem("");
    ui->comboBox_11->addItem("OUTPUT");
    ui->comboBox_11->addItem("INPUT");
    ui->comboBox_11->addItem("PWM_4A");
    ui->comboBox_11->addItem("I2C_SCL");
    ui->comboBox_11->addItem("CAN_RX");

    // Initializes the options - Pin12
    ui->comboBox_12->addItem("");
    ui->comboBox_12->addItem("OUTPUT");
    ui->comboBox_12->addItem("INPUT");
    ui->comboBox_12->addItem("PWM_4B");
    ui->comboBox_12->addItem("I2C_SDA");
    ui->comboBox_12->addItem("CAN_TX");

    // Initializes the options - Pin13
    ui->comboBox_13->addItem("");
    ui->comboBox_13->addItem("OUTPUT");
    ui->comboBox_13->addItem("INPUT");
    ui->comboBox_13->addItem("PWM_3B");

    // Initializes the options - Pin14
    ui->comboBox_14->addItem("");
    ui->comboBox_14->addItem("OUTPUT");
    ui->comboBox_14->addItem("INPUT");
    ui->comboBox_14->addItem("ADC_1");

    // Initializes the options - Pin15
    ui->comboBox_15->addItem("");
    ui->comboBox_15->addItem("OUTPUT");
    ui->comboBox_15->addItem("INPUT");
    ui->comboBox_15->addItem("ADC_2");

    // Initializes the options - Pin16
    ui->comboBox_16->addItem("");
    ui->comboBox_16->addItem("OUTPUT");
    ui->comboBox_16->addItem("INPUT");
    ui->comboBox_16->addItem("ADC_3");

    // Initializes the options - Pin17
    ui->comboBox_17->addItem("");
    ui->comboBox_17->addItem("OUTPUT");
    ui->comboBox_17->addItem("INPUT");
    ui->comboBox_17->addItem("ADC_4");

    // Initializes the options - Pin18
    ui->comboBox_18->addItem("");
    ui->comboBox_18->addItem("OUTPUT");
    ui->comboBox_18->addItem("INPUT");
    ui->comboBox_18->addItem("ADC_5");
    ui->comboBox_18->addItem("PWM_5A");
    ui->comboBox_18->addItem("UART_TX4");

    // Initializes the options - Pin19
    ui->comboBox_19->addItem("");
    ui->comboBox_19->addItem("OUTPUT");
    ui->comboBox_19->addItem("INPUT");
    ui->comboBox_19->addItem("ADC_6");
    ui->comboBox_19->addItem("PWM_5B");
    ui->comboBox_19->addItem("UART_RX4");

    // Initializes the options - Pin20
    ui->comboBox_20->addItem("");
    ui->comboBox_20->addItem("OUTPUT");
    ui->comboBox_20->addItem("INPUT");
    ui->comboBox_20->addItem("ADC_7");
    ui->comboBox_20->addItem("DAC_1");
    ui->comboBox_20->addItem("SPI_NSS");

    // Initializes the options - Pin21
    ui->comboBox_21->addItem("");
    ui->comboBox_21->addItem("OUTPUT");
    ui->comboBox_21->addItem("INPUT");
    ui->comboBox_21->addItem("ADC_8");
    ui->comboBox_21->addItem("DAC_2");
    ui->comboBox_21->addItem("SPI_SCK");

    // Initializes the options - Pin22
    ui->comboBox_22->addItem("");
    ui->comboBox_22->addItem("OUTPUT");
    ui->comboBox_22->addItem("INPUT");
    ui->comboBox_22->addItem("ADC_9");
    ui->comboBox_22->addItem("SPI_MISO");

    // Initializes the options - Pin23
    ui->comboBox_23->addItem("");
    ui->comboBox_23->addItem("OUTPUT");
    ui->comboBox_23->addItem("INPUT");
    ui->comboBox_23->addItem("ADC_10");
    ui->comboBox_23->addItem("SPI_MOSI");

    // Initializes the options - Pin24
    ui->comboBox_24->addItem("");
    ui->comboBox_24->addItem("OUTPUT");
    ui->comboBox_24->addItem("INPUT");
    ui->comboBox_24->addItem("ADC_11");

    // Initializes the options - Pin25
    ui->comboBox_25->addItem("");
    ui->comboBox_25->addItem("OUTPUT");
    ui->comboBox_25->addItem("INPUT");
    ui->comboBox_25->addItem("ADC_12");

    // Initializes the status bar
    updateStatusBar();
}


/* Destructor ---------------------------------------------------------------*/
IotonConfigWindow::~IotonConfigWindow()
{
    delete ui;
}


/** ----------------------------------------------------------------------- **/
/**                              Private slots                              **/
/** ----------------------------------------------------------------------- **/

/* Slot: Pin0 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_0_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(0);
    if (!openFile) pins[0] = defaultPin;
    pins[0].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(0, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "UART_TX1")
    {
        ui->comboBox_advanced->addItem("Pino 0: " + arg1);
    }

    // Linkage of UART1 (Pin0, Pin1)
    if (arg1 == "UART_TX1")
    {
        ui->comboBox_1->setCurrentText("UART_RX1");
        ui->comboBox_advanced->addItem("UART1");
    }
    else if (ui->comboBox_1->currentText() == "UART_RX1")
    {
        ui->comboBox_1->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART1"));
    }
}

/* Slot: Pin1 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_1_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(1);
    if (!openFile) pins[1] = defaultPin;
    pins[1].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(1, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "UART_RX1")
    {
        ui->comboBox_advanced->addItem("Pino 1: " + arg1);
    }

    // Linkage of UART1 (Pin0, Pin1)
    if (arg1 == "UART_RX1")
    {
        ui->comboBox_0->setCurrentText("UART_TX1");
    }
    else if (ui->comboBox_0->currentText() == "UART_TX1")
    {
        ui->comboBox_0->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART1"));
    }
}

/* Slot: Pin2 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(2);
    if (!openFile) pins[2] = defaultPin;
    pins[2].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(2, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_D0")
    {
        ui->comboBox_advanced->addItem("Pino 2: " + arg1);
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_D0")
    {
        ui->comboBox_3->setCurrentText("SDIO_D1");
        if (ui->comboBox_advanced->findText("SDIO") == -1)
        {
            ui->comboBox_advanced->addItem("SDIO");
        }
    }
    else if (ui->comboBox_3->currentText() == "SDIO_D1")
    {
        ui->comboBox_3->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin3 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(3);
    if (!openFile) pins[3] = defaultPin;
    pins[3].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(3, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_D1")
    {
        ui->comboBox_advanced->addItem("Pino 3: " + arg1);
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_D1")
    {
        ui->comboBox_5->setCurrentText("SDIO_D2");
    }
    else if (ui->comboBox_5->currentText() == "SDIO_D2")
    {
        ui->comboBox_5->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin4 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(4);
    if (!openFile) pins[4] = defaultPin;
    pins[4].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(4, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 4: " + arg1);
    }
}

/* Slot: Pin5 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(5);
    if (!openFile) pins[5] = defaultPin;
    pins[5].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(5, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_D2" && arg1 != "UART_TX2")
    {
        ui->comboBox_advanced->addItem("Pino 5: " + arg1);
    }

    // Linkage of UART2 (Pin5, Pin6)
    if (arg1 == "UART_TX2")
    {
        ui->comboBox_6->setCurrentText("UART_RX2");
        ui->comboBox_advanced->addItem("UART2");
    }
    else if (ui->comboBox_6->currentText() == "UART_RX2")
    {
        ui->comboBox_6->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART2"));
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_D2")
    {
        ui->comboBox_6->setCurrentText("SDIO_D3");
    }
    else if (ui->comboBox_6->currentText() == "SDIO_D3")
    {
        ui->comboBox_6->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin6 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_6_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(6);
    if (!openFile) pins[6] = defaultPin;
    pins[6].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(6, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_D3" && arg1 != "UART_RX2")
    {
        ui->comboBox_advanced->addItem("Pino 6: " + arg1);
    }

    // Linkage of UART2 (Pin5, Pin6)
    if (arg1 == "UART_RX2")
    {
        ui->comboBox_5->setCurrentText("UART_TX2");
    }
    else if (ui->comboBox_5->currentText() == "UART_TX2")
    {
        ui->comboBox_5->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART2"));
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_D3")
    {
        ui->comboBox_7->setCurrentText("SDIO_CK");
    }
    else if (ui->comboBox_7->currentText() == "SDIO_CK")
    {
        ui->comboBox_7->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin7 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_7_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(7);
    if (!openFile) pins[7] = defaultPin;
    pins[7].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(7, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_CK" && arg1 != "UART_TX3")
    {
        ui->comboBox_advanced->addItem("Pino 7: " + arg1);
    }

    // Linkage of UART3 (Pin7, Pin8)
    if (arg1 == "UART_TX3")
    {
        ui->comboBox_8->setCurrentText("UART_RX3");
        ui->comboBox_advanced->addItem("UART3");
    }
    else if (ui->comboBox_8->currentText() == "UART_RX3")
    {
        ui->comboBox_8->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART3"));
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_CK")
    {
        ui->comboBox_8->setCurrentText("SDIO_CMD");
    }
    else if (ui->comboBox_8->currentText() == "SDIO_CMD")
    {
        ui->comboBox_8->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin8 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_8_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(8);
    if (!openFile) pins[8] = defaultPin;
    pins[8].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(8, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SDIO_CMD" && arg1 != "UART_RX3")
    {
        ui->comboBox_advanced->addItem("Pino 8: " + arg1);
    }

    // Linkage of UART3 (Pin7, Pin8)
    if (arg1 == "UART_RX3")
    {
        ui->comboBox_7->setCurrentText("UART_TX3");
    }
    else if (ui->comboBox_7->currentText() == "UART_TX3")
    {
        ui->comboBox_7->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART3"));
    }

    // Linkage of SDIO (Pin2, Pin3, Pin5, Pin6, Pin7, Pin8)
    if (arg1 == "SDIO_CMD")
    {
        ui->comboBox_2->setCurrentText("SDIO_D0");
    }
    else if (ui->comboBox_2->currentText() == "SDIO_D0")
    {
        ui->comboBox_2->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SDIO"));
    }
}

/* Slot: Pin0 changed -------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_9_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(9);
    if (!openFile) pins[9] = defaultPin;
    pins[9].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(9, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 9: " + arg1);
    }
}

/* Slot: Pin10 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_10_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(10);
    if (!openFile) pins[10] = defaultPin;
    pins[10].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(10, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 10: " + arg1);
    }
}

/* Slot: Pin11 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_11_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(11);
    if (!openFile) pins[11] = defaultPin;
    pins[11].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(11, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "I2C_SCL" && arg1 != "CAN_RX")
    {
        ui->comboBox_advanced->addItem("Pino 11: " + arg1);
    }

    // Linkage of I2C (Pin11, Pin12)
    if (arg1 == "I2C_SCL")
    {
        ui->comboBox_12->setCurrentText("I2C_SDA");
        ui->comboBox_advanced->addItem("I2C");
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("CAN"));
    }
    else if (ui->comboBox_12->currentText() == "I2C_SDA")
    {
        ui->comboBox_12->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("I2C"));
    }

    // Linkage of CAN (Pin11, Pin12)
    if (arg1 == "CAN_RX")
    {
        ui->comboBox_12->setCurrentText("CAN_TX");
        ui->comboBox_advanced->addItem("CAN");
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("I2C"));
    }
    else if (ui->comboBox_12->currentText() == "CAN_TX" && arg1 != "")
    {
        ui->comboBox_12->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("CAN"));
    }
}

/* Slot: Pin12 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_12_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(12);
    if (!openFile) pins[12] = defaultPin;
    pins[12].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(12, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "I2C_SDA" && arg1 != "CAN_TX")
    {
        ui->comboBox_advanced->addItem("Pino 12: " + arg1);
    }

    // Linkage of I2C (Pin11, Pin12)
    if (arg1 == "I2C_SDA")
    {
        ui->comboBox_11->setCurrentText("I2C_SCL");
    }
    else if (ui->comboBox_11->currentText() == "I2C_SCL")
    {
        ui->comboBox_11->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("I2C"));
    }

    // Linkage of CAN (Pin11, Pin12)
    if (arg1 == "CAN_TX")
    {
        ui->comboBox_11->setCurrentText("CAN_RX");
    }
    else if (ui->comboBox_11->currentText() == "CAN_RX" && arg1 != "")
    {
        ui->comboBox_11->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("CAN"));
    }
}

/* Slot: Pin13 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_13_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(13);
    if (!openFile) pins[13] = defaultPin;
    pins[13].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(13, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 13: " + arg1);
    }
}

/* Slot: Pin14 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_14_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(14);
    if (!openFile) pins[14] = defaultPin;
    pins[14].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(14, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 14: " + arg1);
    }
}

/* Slot: Pin15 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_15_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(15);
    if (!openFile) pins[15] = defaultPin;
    pins[15].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(15, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 15: " + arg1);
    }
}

/* Slot: Pin16 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_16_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(16);
    if (!openFile) pins[16] = defaultPin;
    pins[16].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(16, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 16: " + arg1);
    }
}

/* Slot: Pin17 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_17_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(17);
    if (!openFile) pins[17] = defaultPin;
    pins[17].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(17, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 17: " + arg1);
    }
}

/* Slot: Pin18 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_18_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(18);
    if (!openFile) pins[18] = defaultPin;
    pins[18].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(18, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "UART_TX4")
    {
        ui->comboBox_advanced->addItem("Pino 18: " + arg1);
    }

    // Linkage of UART4 (Pin18, Pin19)
    if (arg1 == "UART_TX4")
    {
        ui->comboBox_19->setCurrentText("UART_RX4");
        ui->comboBox_advanced->addItem("UART4");
    }
    else if (ui->comboBox_19->currentText() == "UART_RX4")
    {
        ui->comboBox_19->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART4"));
    }
}

/* Slot: Pin19 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_19_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(19);
    if (!openFile) pins[19] = defaultPin;
    pins[19].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(19, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "UART_RX4")
    {
        ui->comboBox_advanced->addItem("Pino 19: " + arg1);
    }

    // Linkage of UART4 (Pin18, Pin19)
    if (arg1 == "UART_RX4")
    {
        ui->comboBox_18->setCurrentText("UART_TX4");
    }
    else if (ui->comboBox_18->currentText() == "UART_TX4")
    {
        ui->comboBox_18->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("UART4"));
    }
}

/* Slot: Pin20 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_20_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(20);
    if (!openFile) pins[20] = defaultPin;
    pins[20].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(20, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SPI_NSS")
    {
        ui->comboBox_advanced->addItem("Pino 20: " + arg1);
    }

    // Linkage of SPI (Pin20, Pin21, Pin22, Pin23)
    if (arg1 == "SPI_NSS")
    {
        ui->comboBox_21->setCurrentText("SPI_SCK");
        ui->comboBox_advanced->addItem("SPI");
    }
    else if (ui->comboBox_21->currentText() == "SPI_SCK")
    {
        ui->comboBox_21->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SPI"));
    }
}

/* Slot: Pin21 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_21_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(21);
    if (!openFile) pins[21] = defaultPin;
    pins[21].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(21, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SPI_SCK")
    {
        ui->comboBox_advanced->addItem("Pino 21: " + arg1);
    }

    // Linkage of SPI (Pin20, Pin21, Pin22, Pin23)
    if (arg1 == "SPI_SCK")
    {
        ui->comboBox_22->setCurrentText("SPI_MISO");
    }
    else if (ui->comboBox_22->currentText() == "SPI_MISO")
    {
        ui->comboBox_22->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SPI"));
    }
}

/* Slot: Pin22 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_22_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(22);
    if (!openFile) pins[22] = defaultPin;
    pins[22].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(22, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SPI_MISO")
    {
        ui->comboBox_advanced->addItem("Pino 22: " + arg1);
    }

    // Linkage of SPI (Pin20, Pin21, Pin22, Pin23)
    if (arg1 == "SPI_MISO")
    {
        ui->comboBox_23->setCurrentText("SPI_MOSI");
    }
    else if (ui->comboBox_23->currentText() == "SPI_MOSI")
    {
        ui->comboBox_23->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SPI"));
    }
}

/* Slot: Pin23 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_23_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(23);
    if (!openFile) pins[23] = defaultPin;
    pins[23].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(23, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "" && arg1 != "SPI_MOSI")
    {
        ui->comboBox_advanced->addItem("Pino 23: " + arg1);
    }

    // Linkage of SPI (Pin20, Pin21, Pin22, Pin23)
    if (arg1 == "SPI_MOSI")
    {
        ui->comboBox_20->setCurrentText("SPI_NSS");
    }
    else if (ui->comboBox_20->currentText() == "SPI_NSS")
    {
        ui->comboBox_20->setCurrentIndex(0);
        ui->comboBox_advanced->removeItem(ui->comboBox_advanced->findText("SPI"));
    }
}

/* Slot: Pin24 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_24_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(24);
    if (!openFile) pins[24] = defaultPin;
    pins[24].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(24, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 24: " + arg1);
    }
}

/* Slot: Pin25 changed ------------------------------------------------------*/
void IotonConfigWindow::on_comboBox_25_currentIndexChanged(const QString &arg1)
{
    // Save configurations and update elements
    int pinMode = getAdvancedPinMode(arg1);
    int index = getAdvancedPinIndex(25);
    if (!openFile) pins[25] = defaultPin;
    pins[25].pinModeUsed = pinMode;
    updateStatusBar();
    updatePinConfigurations(25, pinMode);

    // Update used pins/peripherals (advanced tab)
    if (index != -1)
    {
        ui->comboBox_advanced->removeItem(index);
    }
    if (arg1 != "")
    {
        ui->comboBox_advanced->addItem("Pino 25: " + arg1);
    }
}

/* Slot: Pin/peripheral selection -------------------------------------------*/
void IotonConfigWindow::on_comboBox_advanced_currentIndexChanged(const QString &arg1)
{
    // Block signals of the objects (advanced tab fields) to load the pins configurations
    ui->lineEdit_advanced_name->blockSignals(true);
    ui->comboBox_advanced_1->blockSignals(true);
    ui->comboBox_advanced_2->blockSignals(true);
    ui->comboBox_advanced_3->blockSignals(true);
    ui->comboBox_advanced_4->blockSignals(true);
    ui->lineEdit_advanced_1->blockSignals(true);
    ui->lineEdit_advanced_2->blockSignals(true);
    ui->checkBox_advanced_1->blockSignals(true);
    ui->checkBox_advanced_2->blockSignals(true);

    // Hide all graphical elements of advanced tab
    toggleAdvancedOff();

    // Get the pin mode/number of the current selected pin
    int pinMode = getAdvancedPinMode(arg1);
    int pinNumber = getAdvancedPinNumber(arg1);

    // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
    // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
    switch (pinMode)
    {
        case ADC:   // Type: individual pin
        {
            // Turn visible only the ADC graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome do Pino");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);

            break;
        }   // end of case ADC

        case CAN:   // Type: peripheral (pin group)
        {
            // Turn visible only the CAN graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome da CAN");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(11, pinMode);
            fillPinConfigurations(12, pinMode);

            break;
        }   // end of case CAN

        case DAC:   // Type: individual pin
        {
            // Turn visible only the DAC graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome do Pino");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);

            break;
        }   // end of case DAC

        case I2C:   // Type: individual pin
        {
            // Turn visible only the I2C graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->label_advanced_1->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);
            ui->lineEdit_advanced_1->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome da I2C");
            ui->label_advanced_1->setText("Frequência (Hz)");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(11, pinMode);
            fillPinConfigurations(12, pinMode);

            break;
        }   // end of case I2C

        case INPUT: // Type: individual pin
        {
            // Turn visible only the INPUT graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->label_advanced_1->setVisible(true);
            ui->label_advanced_2->setVisible(true);
            ui->label_advanced_3->setVisible(pins[pinNumber].input.externalInterrupt);
            ui->lineEdit_advanced_name->setVisible(true);
            ui->checkBox_advanced_1->setVisible(true);
            ui->comboBox_advanced_2->setVisible(true);
            ui->comboBox_advanced_3->setVisible(pins[pinNumber].input.externalInterrupt);

            // Fieldnames
            ui->label_advanced_name->setText("Nome do Pino");
            ui->label_advanced_1->setText("Interrupção externa");
            ui->label_advanced_2->setText("Modo");
            ui->label_advanced_3->setText("Borda");

            // Mode
            ui->comboBox_advanced_2->clear();
            ui->comboBox_advanced_2->addItem("default");
            ui->comboBox_advanced_2->addItem("PullUp");
            ui->comboBox_advanced_2->addItem("PullDown");
            ui->comboBox_advanced_2->addItem("PullNone");
            ui->comboBox_advanced_2->addItem("OpenDrain");

            // Edge
            ui->comboBox_advanced_3->clear();
            ui->comboBox_advanced_3->addItem("default");
            ui->comboBox_advanced_3->addItem("Subida");
            ui->comboBox_advanced_3->addItem("Descida");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);

            break;
        }   // end of case INPUT

        case OUTPUT:    // Type: individual pin
        {
            // turn visible only the OUTPUT graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome do Pino");

            // load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);

            break;
        }   // end of case OUTPUT

        case PWM:   // Type: individual pin
        {
            // Turn visible only the PWM graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->label_advanced_1->setVisible(true);
            ui->label_advanced_2->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);
            ui->lineEdit_advanced_1->setVisible(true);
            ui->lineEdit_advanced_2->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome do Pino");
            ui->label_advanced_1->setText("Frequência (Hz)");
            ui->label_advanced_2->setText("Duty cycle (%)");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);

            break;
        }   // end of case PWM

        case SDIO:  // Type: peripheral (pin group)
        {
            // Turn visible only the SDIO graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome da SDIO");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(2, pinMode);
            fillPinConfigurations(3, pinMode);
            fillPinConfigurations(5, pinMode);
            fillPinConfigurations(6, pinMode);
            fillPinConfigurations(7, pinMode);
            fillPinConfigurations(8, pinMode);

            break;
        }   // end of case SDIO

        case SPI:   // Type: peripheral (pin group)
        {
            // Turn visible only the SPI graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->label_advanced_1->setVisible(true);
            ui->label_advanced_2->setVisible(true);
            ui->label_advanced_3->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);
            ui->lineEdit_advanced_1->setVisible(true);
            ui->lineEdit_advanced_2->setVisible(true);
            ui->comboBox_advanced_3->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome da SPI");
            ui->label_advanced_1->setText("Frequência (Hz)");
            ui->label_advanced_2->setText("Bits");
            ui->label_advanced_3->setText("Modo");

            // Mode
            ui->comboBox_advanced_3->clear();
            ui->comboBox_advanced_3->addItem("default");
            ui->comboBox_advanced_3->addItem("0");
            ui->comboBox_advanced_3->addItem("1");
            ui->comboBox_advanced_3->addItem("3");
            ui->comboBox_advanced_3->addItem("4");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(20, pinMode);
            fillPinConfigurations(21, pinMode);
            fillPinConfigurations(22, pinMode);
            fillPinConfigurations(23, pinMode);

            break;
        }   // end of case SPI

        case UART:  // Type: peripheral (pin group)
        {
            // Turn visible only the UART graphical elements
            ui->label_advanced_name->setVisible(true);
            ui->label_advanced_1->setVisible(true);
            ui->label_advanced_2->setVisible(true);
            ui->label_advanced_3->setVisible(true);
            ui->label_advanced_4->setVisible(true);
            ui->lineEdit_advanced_name->setVisible(true);
            ui->comboBox_advanced_1->setVisible(true);
            ui->comboBox_advanced_2->setVisible(true);
            ui->comboBox_advanced_3->setVisible(true);
            ui->comboBox_advanced_4->setVisible(true);

            // Fieldnames
            ui->label_advanced_name->setText("Nome da UART");
            ui->label_advanced_1->setText("Baudrate");
            ui->label_advanced_2->setText("Bits");
            ui->label_advanced_3->setText("Stop bits");
            ui->label_advanced_4->setText("Paridade");

            // Baudrate
            ui->comboBox_advanced_1->clear();
            ui->comboBox_advanced_1->addItem("default");
            ui->comboBox_advanced_1->addItem("4800");
            ui->comboBox_advanced_1->addItem("9600");
            ui->comboBox_advanced_1->addItem("19200");
            ui->comboBox_advanced_1->addItem("38400");
            ui->comboBox_advanced_1->addItem("57600");
            ui->comboBox_advanced_1->addItem("115200");
            ui->comboBox_advanced_1->addItem("230400");

            // Bits
            ui->comboBox_advanced_2->clear();
            ui->comboBox_advanced_2->addItem("default");
            ui->comboBox_advanced_2->addItem("5");
            ui->comboBox_advanced_2->addItem("6");
            ui->comboBox_advanced_2->addItem("7");
            ui->comboBox_advanced_2->addItem("8");

            // Stop bits
            ui->comboBox_advanced_3->clear();
            ui->comboBox_advanced_3->addItem("default");
            ui->comboBox_advanced_3->addItem("1");
            ui->comboBox_advanced_3->addItem("2");

            // Parity
            ui->comboBox_advanced_4->clear();
            ui->comboBox_advanced_4->addItem("default");
            ui->comboBox_advanced_4->addItem("NONE");
            ui->comboBox_advanced_4->addItem("ODD");
            ui->comboBox_advanced_4->addItem("EVEN");
            ui->comboBox_advanced_4->addItem("FORCED1");
            ui->comboBox_advanced_4->addItem("FORCED0");

            // Load the configurations values into the graphical elements
            fillPinConfigurations(pinNumber, pinMode);
            fillPinConfigurations(pinNumber + 1, pinMode);

            break;
        }   // end of case UART

        default:
        {
            break;
        }
    }   // end of switch (pinMode)

    // Enable block signals
    ui->lineEdit_advanced_name->blockSignals(false);
    ui->comboBox_advanced_1->blockSignals(false);
    ui->comboBox_advanced_2->blockSignals(false);
    ui->comboBox_advanced_3->blockSignals(false);
    ui->comboBox_advanced_4->blockSignals(false);
    ui->lineEdit_advanced_1->blockSignals(false);
    ui->lineEdit_advanced_2->blockSignals(false);
    ui->checkBox_advanced_1->blockSignals(false);
    ui->checkBox_advanced_2->blockSignals(false);
}

/* Slot: 'lineEdit_advanced_name' editing finished --------------------------*/
void IotonConfigWindow::on_lineEdit_advanced_name_editingFinished()
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default name
    if (ui->lineEdit_advanced_name->text() == "default" || ui->lineEdit_advanced_name->text() == "")
    {
        // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
        // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
        switch (pinMode)
        {
            case ADC:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_AnalogIn");
                break;
            }   // end of case ADC

            case CAN:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText("can");
                break;
            }   // end of case CAN

            case DAC:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_AnalogOut");
                break;
            }   // end of case DAC

            case I2C:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText("i2c");
                break;
            }   // end of case I2C

            case INPUT: // Type: individual pin
            {
                if (ui->checkBox_advanced_1->isChecked())
                {
                    ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_InterruptIn");
                }
                else
                {
                    ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_DigitalIn");
                }
                break;
            }   // end of case INPUT

            case OUTPUT:    // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_DigitalOut");
                break;
            }   // end of case OUTPUT

            case PWM:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_PwmOut");
                break;
            }   // end of case PWM

            case SDIO:  // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText("sdio");
                break;
            }   // end of case SDIO

            case SPI:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText("spi");
                break;
            }   // end of case SPI

            case UART:  // Type: peripheral (pin group)
            {
                if (pinNumber == 0 || pinNumber == 1)
                {
                    ui->lineEdit_advanced_name->setText("uart1");
                }
                else if (pinNumber == 5 || pinNumber == 6)
                {
                    ui->lineEdit_advanced_name->setText("uart2");
                }
                else if (pinNumber == 7 || pinNumber == 8)
                {
                    ui->lineEdit_advanced_name->setText("uart3");
                }
                else if (pinNumber == 18 || pinNumber == 19)
                {
                    ui->lineEdit_advanced_name->setText("uart4");
                }
                break;
            }   // end of case UART

            default:
            {
                break;
            }
        }   // end of switch (pinMode)
    }   // end of load default name

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'lineEdit_advanced_1' editing finished -----------------------------*/
// Relations: Frequency (I2C, PWM and SPI)
void IotonConfigWindow::on_lineEdit_advanced_1_editingFinished()
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (ui->lineEdit_advanced_1->text() == "")
    {
        switch (pinMode)
        {
            case PWM:   // Frequency
            {
                ui->lineEdit_advanced_1->setText(QString::number(pwm.frequency));
                break;
            }

            case I2C:   // Frequency
            {
                ui->lineEdit_advanced_1->setText(QString::number(i2c.frequency));
                break;
            }

            case SPI:   // Frequency
            {
                ui->lineEdit_advanced_1->setText(QString::number(spi.frequency));
                break;
            }

            default:
            {
                break;
            }
        }   // end of switch (pinMode)
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'lineEdit_advanced_2' editing finished -----------------------------*/
// Relations: Duty cycle (PWM), Bits (SPI)
void IotonConfigWindow::on_lineEdit_advanced_2_editingFinished()
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (ui->lineEdit_advanced_2->text() == "")
    {
        switch (pinMode)
        {
            case PWM:   // Duty cycle
            {
                ui->lineEdit_advanced_2->setText(QString::number(pwm.dutyCycle));
                break;
            }

            case SPI:   // Bits
            {
                ui->lineEdit_advanced_2->setText(QString::number(spi.bits));
                break;
            }

            default:
            {
                break;
            }
        }   // end of switch (pinMode)
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'comboBox_advanced_1' index changed --------------------------------*/
// Relations: Baudrate (UART)
void IotonConfigWindow::on_comboBox_advanced_1_currentIndexChanged(const QString &arg1)
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (arg1 == "default")
    {
        switch (pinMode)
        {
            case UART:  // Baudrate
            {
                ui->comboBox_advanced_1->setCurrentText(QString::number(uart.baud));
                break;
            }

            default:
            {
                break;
            }
        }
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'comboBox_advanced_2' index changed --------------------------------*/
// Relations: Mode (INPUT), Bits (UART)
void IotonConfigWindow::on_comboBox_advanced_2_currentIndexChanged(const QString &arg1)
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (arg1 == "default")
    {
        switch (pinMode)
        {
            case INPUT: // Mode
            {
                ui->comboBox_advanced_2->setCurrentText(input.mode == PULLUP ? "PullUp" : input.mode == PULLDOWN ? "PullDown" : input.mode == OPENDRAIN ? "OpenDrain" : "PullNone");
                break;
            }

            case UART:  // Bits
            {
                ui->comboBox_advanced_2->setCurrentText(QString::number(uart.bits));
                break;
            }

            default:
            {
                break;
            }
        }
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'comboBox_advanced_3' index changed --------------------------------*/
// Relations: Edge (INPUT), Stop bits (UART), Mode (SPI)
void IotonConfigWindow::on_comboBox_advanced_3_currentIndexChanged(const QString &arg1)
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (arg1 == "default")
    {
        switch (pinMode)
        {
            case INPUT: // Edge
            {
                ui->comboBox_advanced_3->setCurrentText(input.edge == RISE ? "Subida" : "Descida");
                break;
            }

            case UART:  // Stop bits
            {
                ui->comboBox_advanced_3->setCurrentText(QString::number(uart.stopBit));
                break;
            }

            case SPI:   // Mode
            {
                ui->comboBox_advanced_3->setCurrentText(QString::number(spi.mode));
                break;
            }

            default:
            {
                break;
            }
        }
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'comboBox_advanced_4' index changed --------------------------------*/
// Relations: Parity (UART)
void IotonConfigWindow::on_comboBox_advanced_4_currentIndexChanged(const QString &arg1)
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks need to load the default value
    if (arg1 == "default")
    {
        switch (pinMode)
        {
            case UART:  // Parity
            {
                ui->comboBox_advanced_4->setCurrentText(uart.parity == NONE ? "NONE" : uart.parity == EVEN ? "EVEN" : "ODD");
                break;
            }

            default:
            {
                break;
            }
        }
    }   // end of load default value

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'checkBox_advanced_1' clicked --------------------------------------*/
// Relations: External interrupt (INPUT)
void IotonConfigWindow::on_checkBox_advanced_1_clicked()
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Checks 'external interrupt'
    if (ui->checkBox_advanced_1->isChecked())
    {
        // Show 'Edge' field
        ui->label_advanced_3->setVisible(true);
        ui->comboBox_advanced_3->setVisible(true);

        // Load default name (interrupt mode)
        ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_InterruptIn");
    }
    else
    {
        // Hide 'Edge' field
        ui->label_advanced_3->setVisible(false);
        ui->comboBox_advanced_3->setVisible(false);

        // Load default name (digital input mode)
        ui->lineEdit_advanced_name->setText("pin" + QString::number(pinNumber) + "_DigitalIn");
    }

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'checkBox_advanced_2' clicked --------------------------------------*/
// Relations: Reserved (none)
void IotonConfigWindow::on_checkBox_advanced_2_clicked()
{
    int pinNumber = getAdvancedPinNumber(ui->comboBox_advanced->currentText());
    int pinMode = getAdvancedPinMode(ui->comboBox_advanced->currentText());

    // Reserved for future use

    // Save configurations and update elements
    savePinConfigurations(pinNumber, pinMode);
    updatePinConfigurations(pinNumber, pinMode);
}

/* Slot: 'radioButton_expand' clicked ---------------------------------------*/
void IotonConfigWindow::on_radioButton_expand_clicked()
{
    // Expands/collapses all items of 'Tree View'
    for (int i = 0; i < N_PINS; i++)
    {
        treeViewItems.at(i)->setExpanded(ui->radioButton_expand->isChecked());
    }
}

/* Slot: 'tabWidget' changed ------------------------------------------------*/
void IotonConfigWindow::on_tabWidget_currentChanged(int index)
{
    // Sort the list of pins and peripherals
    if (index == 1) // index 1: 'advanced tab'
    {
        QStringList list;
        int index_from = 0, index_to = 1;
        pinNameDefault();

        // Keep the items in an auxiliary list and sort them (ascending order)
        for (int index = 0; index < ui->comboBox_advanced->count(); index++)
        {
            list << ui->comboBox_advanced->itemText(index);
        }
        ui->comboBox_advanced->clear();
        list.sort();

        // Fixes the cases: 10, 11, 1, 20, 21, 22, 2 -> 1, 2, 10, 11, 20, 21, 22
        for (int pinNumber = 0; pinNumber < N_PINS; pinNumber++)
        {
            index_from = list.indexOf(QRegExp("^Pino " + QString::number(pinNumber) + ":.+"));
            if (index_from != -1)
            {
                list.move(index_from, index_to);
                index_to++;
            }
        }

        // Update 'comboBox_advanced' with the sorted list
        ui->comboBox_advanced->addItems(list);
    }
}

/* Slot: 'Create Project' button clicked ------------------------------------*/
void IotonConfigWindow::on_pushButton_createProject_clicked()
{
    // Create the default directory if it does not exist
    if (!QDir().exists(QDir::homePath() + "/ioton"))
    {
        QDir().mkdir(QDir::homePath() + "/ioton");
    }

    // Configure the 'Create Project' dialog
    QFileDialog dialog(this, tr("Criar Projeto"), QDir::homePath() + "/ioton", tr("Pasta"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setLabelText(QFileDialog::LookIn, "Local:");
    dialog.setLabelText(QFileDialog::FileName, "Nome do Projeto:");
    dialog.setLabelText(QFileDialog::FileType, "Tipo:");
    dialog.setLabelText(QFileDialog::Accept, "Criar");
    dialog.setLabelText(QFileDialog::Reject, "Cancelar");

    // The absolute path of the project name selected in the dialog
    QString projectName;
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        projectName = fileNames[0];
    }

    // Try create the project
    if (projectName != NULL)
    {
        // Create the project (folder and PlatformIO files)
        int status = IotonConfigProject::createProject(projectName);

        if (status == -1)       // Error creating the folder
        {
            QMessageBox::critical(this, tr("IOTON"), "Erro ao criar diretório " + projectName);
        }
        else if (status == -2)  // Error creating the PlatformIO files
        {
            QMessageBox::critical(this, tr("IOTON"), "PlatformIO: erro ao criar projeto no diretório " + projectName);
        }
        else if (status == 0)   // Success
        {
            // Load the default pins names
            pinNameDefault();

            // Try create the template of the project
            status = IotonConfigProject::createTemplate(projectName, pins, ui->actionGenerate_Examples->isChecked());

            if (status != 0)    // Error creating the template
            {
                QMessageBox::critical(this, tr("IOTON"), "Erro ao criar projeto no diretório " + projectName);
            }
            else    // Close program successfully
            {
                IotonConfigProject::openAtomFolder(projectName);

                ui->progressBar->setVisible(true);
                for (int i = 0; i <= 100; i++)
                {
                    ui->progressBar->setValue(i);
                    QThread::msleep(30);
                }

                QApplication::quit();
            }
        }
    }
}

/* Slot: 'actionOpen_project' triggered -------------------------------------*/
void IotonConfigWindow::on_actionOpen_project_triggered()
{
    // Configure the 'Open Configurations' dialog
    QFileDialog dialog(this, tr("Abrir Configurações"), QDir::homePath(), tr("Arquivo ioTon Config (*.iotonconfig)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setLabelText(QFileDialog::LookIn, "Local:");
    dialog.setLabelText(QFileDialog::FileName, "Nome:");
    dialog.setLabelText(QFileDialog::FileType, "Tipo:");
    dialog.setLabelText(QFileDialog::Accept, "Abrir");
    dialog.setLabelText(QFileDialog::Reject, "Cancelar");

    // The absolute path of the file name selected in the dialog
    QString fileName;
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        fileName = fileNames[0];
    }

    // Try open/read the file
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream dataIn(&file);
        on_actionClear_Pins_triggered();

        while (!dataIn.atEnd())
        {
            // Read line - pinData Format: pin&name&mode&param1&param2...&paramN
            QStringList pinData = dataIn.readLine().split("&");

            bool ok;
            int pinNumber = pinData[0].toInt(&ok);
            int pinMode = (pinData[2] == "input" ? INPUT : pinData[2] == "output" ? OUTPUT : pinData[2] == "adc" ? ADC : pinData[2] == "dac" ? DAC : pinData[2] == "i2c" ? I2C : pinData[2] == "pwm" ? PWM : pinData[2] == "sdio" ? SDIO : pinData[2] == "spi" ? SPI : pinData[2] == "uart" ? UART : -1);

            // Check if is valid data
            if (pinMode != -1 && ok == true)
            {
                pins[pinNumber].name = pinData[1];
                pins[pinNumber].pinModeUsed = pinMode;

                // Pins/peripheral with advanced configurations: I2C, INPUT, PWM, SPI, UART
                switch (pinMode)
                {
                    case I2C:
                    {
                        pins[pinNumber].i2c.frequency = pinData[3].toInt();
                        break;
                    }

                    case INPUT:
                    {
                        pins[pinNumber].input.mode = (pinData[3] == "PullUp" ? PULLUP : pinData[3] == "PullDown" ? PULLDOWN : pinData[3] == "OpenDrain" ? OPENDRAIN : PULLNONE);
                        pins[pinNumber].input.externalInterrupt = (pinData[4] == "true" ? true : false);
                        if (pinData[4] == "true") pins[pinNumber].input.edge = (pinData[5] == "Subida" ? RISE : FALL);
                        break;
                    }

                    case PWM:
                    {
                        pins[pinNumber].pwm.frequency = pinData[3].toInt();
                        pins[pinNumber].pwm.dutyCycle = pinData[4].toInt();
                        break;
                    }

                    case SPI:
                    {
                        pins[pinNumber].spi.frequency = pinData[3].toInt();
                        pins[pinNumber].spi.bits = pinData[4].toInt();
                        pins[pinNumber].spi.mode = pinData[5].toInt();
                        break;
                    }

                    case UART:
                    {
                        pins[pinNumber].uart.baud = pinData[3].toInt();
                        pins[pinNumber].uart.bits = pinData[4].toInt();
                        pins[pinNumber].uart.stopBit = pinData[5].toInt();
                        pins[pinNumber].uart.parity = (pinData[6] == "NONE" ? NONE : pinData[6] == "EVEN" ? EVEN : pinData[6] == "ODD" ? ODD : pinData[6] == "FORCED1" ? FORCED1 : FORCED0);
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }   // end of switch (pinMode)
            }   // end of if is valid data
        }   // end of while !(end file)

        // Update pins selection
        openFile = true;    // Flag for disable default pin values while loading the file configurations
        if (pins[0].pinModeUsed != -1) ui->comboBox_0->setCurrentIndex(ui->comboBox_0->findText(pinModeMap[pins[0].pinModeUsed], Qt::MatchStartsWith));
        if (pins[1].pinModeUsed != -1) ui->comboBox_1->setCurrentIndex(ui->comboBox_1->findText(pinModeMap[pins[1].pinModeUsed], Qt::MatchStartsWith));
        if (pins[2].pinModeUsed != -1) ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(pinModeMap[pins[2].pinModeUsed], Qt::MatchStartsWith));
        if (pins[3].pinModeUsed != -1) ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findText(pinModeMap[pins[3].pinModeUsed], Qt::MatchStartsWith));
        if (pins[4].pinModeUsed != -1) ui->comboBox_4->setCurrentIndex(ui->comboBox_4->findText(pinModeMap[pins[4].pinModeUsed], Qt::MatchStartsWith));
        if (pins[5].pinModeUsed != -1) ui->comboBox_5->setCurrentIndex(ui->comboBox_5->findText(pinModeMap[pins[5].pinModeUsed], Qt::MatchStartsWith));
        if (pins[6].pinModeUsed != -1) ui->comboBox_6->setCurrentIndex(ui->comboBox_6->findText(pinModeMap[pins[6].pinModeUsed], Qt::MatchStartsWith));
        if (pins[7].pinModeUsed != -1) ui->comboBox_7->setCurrentIndex(ui->comboBox_7->findText(pinModeMap[pins[7].pinModeUsed], Qt::MatchStartsWith));
        if (pins[8].pinModeUsed != -1) ui->comboBox_8->setCurrentIndex(ui->comboBox_8->findText(pinModeMap[pins[8].pinModeUsed], Qt::MatchStartsWith));
        if (pins[9].pinModeUsed != -1) ui->comboBox_9->setCurrentIndex(ui->comboBox_9->findText(pinModeMap[pins[9].pinModeUsed], Qt::MatchStartsWith));
        if (pins[10].pinModeUsed != -1) ui->comboBox_10->setCurrentIndex(ui->comboBox_10->findText(pinModeMap[pins[10].pinModeUsed], Qt::MatchStartsWith));
        if (pins[11].pinModeUsed != -1) ui->comboBox_11->setCurrentIndex(ui->comboBox_11->findText(pinModeMap[pins[11].pinModeUsed], Qt::MatchStartsWith));
        if (pins[12].pinModeUsed != -1) ui->comboBox_12->setCurrentIndex(ui->comboBox_12->findText(pinModeMap[pins[12].pinModeUsed], Qt::MatchStartsWith));
        if (pins[13].pinModeUsed != -1) ui->comboBox_13->setCurrentIndex(ui->comboBox_13->findText(pinModeMap[pins[13].pinModeUsed], Qt::MatchStartsWith));
        if (pins[14].pinModeUsed != -1) ui->comboBox_14->setCurrentIndex(ui->comboBox_14->findText(pinModeMap[pins[14].pinModeUsed], Qt::MatchStartsWith));
        if (pins[15].pinModeUsed != -1) ui->comboBox_15->setCurrentIndex(ui->comboBox_15->findText(pinModeMap[pins[15].pinModeUsed], Qt::MatchStartsWith));
        if (pins[16].pinModeUsed != -1) ui->comboBox_16->setCurrentIndex(ui->comboBox_16->findText(pinModeMap[pins[16].pinModeUsed], Qt::MatchStartsWith));
        if (pins[17].pinModeUsed != -1) ui->comboBox_17->setCurrentIndex(ui->comboBox_17->findText(pinModeMap[pins[17].pinModeUsed], Qt::MatchStartsWith));
        if (pins[18].pinModeUsed != -1) ui->comboBox_18->setCurrentIndex(ui->comboBox_18->findText(pinModeMap[pins[18].pinModeUsed], Qt::MatchStartsWith));
        if (pins[19].pinModeUsed != -1) ui->comboBox_19->setCurrentIndex(ui->comboBox_19->findText(pinModeMap[pins[19].pinModeUsed], Qt::MatchStartsWith));
        if (pins[20].pinModeUsed != -1) ui->comboBox_20->setCurrentIndex(ui->comboBox_20->findText(pinModeMap[pins[20].pinModeUsed], Qt::MatchStartsWith));
        if (pins[21].pinModeUsed != -1) ui->comboBox_21->setCurrentIndex(ui->comboBox_21->findText(pinModeMap[pins[21].pinModeUsed], Qt::MatchStartsWith));
        if (pins[22].pinModeUsed != -1) ui->comboBox_22->setCurrentIndex(ui->comboBox_22->findText(pinModeMap[pins[22].pinModeUsed], Qt::MatchStartsWith));
        if (pins[23].pinModeUsed != -1) ui->comboBox_23->setCurrentIndex(ui->comboBox_23->findText(pinModeMap[pins[23].pinModeUsed], Qt::MatchStartsWith));
        if (pins[24].pinModeUsed != -1) ui->comboBox_24->setCurrentIndex(ui->comboBox_24->findText(pinModeMap[pins[24].pinModeUsed], Qt::MatchStartsWith));
        if (pins[25].pinModeUsed != -1) ui->comboBox_25->setCurrentIndex(ui->comboBox_25->findText(pinModeMap[pins[25].pinModeUsed], Qt::MatchStartsWith));
        openFile = false;
    }   // end of if file open

    file.close();
}

/* Slot: 'actionSave_project' triggered -------------------------------------*/
void IotonConfigWindow::on_actionSave_project_triggered()
{
    // Configure the 'Save Configurations' dialog
    QFileDialog dialog(this, tr("Salvar Configurações"), QDir::homePath(), tr("Arquivo ioTon Config (*.iotonconfig)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setLabelText(QFileDialog::LookIn, "Local:");
    dialog.setLabelText(QFileDialog::FileName, "Nome:");
    dialog.setLabelText(QFileDialog::FileType, "Tipo:");
    dialog.setLabelText(QFileDialog::Accept, "Salvar");
    dialog.setLabelText(QFileDialog::Reject, "Cancelar");

    // The absolute path of the file name selected in the dialog
    QString fileName;
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        fileName = fileNames[0];
    }

    // Try create/write the file
    QFile file(fileName + (fileName.endsWith(".iotonconfig") ? "" : ".iotonconfig"));
    if (file.open(QIODevice::WriteOnly))
    {
        QString dataOut = NULL;
        pinNameDefault();

        for (int i = 0; i < N_PINS; i++)
        {
            // Check if is valid pin
            if (pins[i].pinModeUsed != -1)
            {
                // dataOut Format: pin&name&mode&param1&param2...&paramN
                dataOut += QString::number(i) + "&" + pins[i].name + "&";

                // Pins/peripheral with advanced configurations: I2C, INPUT, PWM, SPI, UART
                switch (pins[i].pinModeUsed)
                {
                    case I2C:
                    {
                        dataOut += "i2c";
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].i2c.frequency));
                        break;
                    }

                    case INPUT:
                    {
                        dataOut += "input";
                        dataOut += "&";
                        dataOut += (pins[i].input.mode == PULLUP ? "PullUp" : pins[i].input.mode == PULLDOWN ? "PullDown" : pins[i].input.mode == OPENDRAIN ? "OpenDrain" : "PullNone");
                        dataOut += "&";
                        dataOut += (pins[i].input.externalInterrupt ? "true" : "false");
                        if (pins[i].input.externalInterrupt) dataOut += (pins[i].input.edge == RISE ? "&Subida" : "&Descida");
                        break;
                    }

                    case PWM:
                    {
                        dataOut += "pwm";
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].pwm.frequency));
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].pwm.dutyCycle));
                        break;
                    }

                    case SPI:
                    {
                        dataOut += "spi";
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].spi.frequency));
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].spi.bits));
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].spi.mode));
                        break;
                    }

                    case UART:
                    {
                        dataOut += "uart";
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].uart.baud));
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].uart.bits));
                        dataOut += "&";
                        dataOut += (QString::number(pins[i].uart.stopBit));
                        dataOut += "&";
                        dataOut += (pins[i].uart.parity == NONE ? "NONE" : pins[i].uart.parity == EVEN ? "EVEN" : pins[i].uart.parity == ODD ? "ODD" : pins[i].uart.parity == FORCED1 ? "FORCED1" : "FORCED0");
                        break;
                    }

                    // Pins/peripheral without advanced configurations
                    default:
                    {
                        dataOut += (pins[i].pinModeUsed == ADC ? "adc" : pins[i].pinModeUsed == DAC ? "dac" : pins[i].pinModeUsed == SDIO ? "sdio" : pins[i].pinModeUsed == OUTPUT ? "output" : "null");
                        break;
                    }
                }   // end of switch (pinMode)

                dataOut += "\n";
            }   // end of if valid pin
        }   // end of for N_PINS

        file.write(dataOut.toLatin1().data());
    }   // end of if file open

    file.close();
}

/* Slot: 'actionClear_Pins' triggered ---------------------------------------*/
void IotonConfigWindow::on_actionClear_Pins_triggered()
{
    // Clears the selection of all pins
    ui->comboBox_0->setCurrentIndex(0);
    ui->comboBox_1->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);
    ui->comboBox_6->setCurrentIndex(0);
    ui->comboBox_7->setCurrentIndex(0);
    ui->comboBox_8->setCurrentIndex(0);
    ui->comboBox_9->setCurrentIndex(0);
    ui->comboBox_10->setCurrentIndex(0);
    ui->comboBox_11->setCurrentIndex(0);
    ui->comboBox_12->setCurrentIndex(0);
    ui->comboBox_13->setCurrentIndex(0);
    ui->comboBox_14->setCurrentIndex(0);
    ui->comboBox_15->setCurrentIndex(0);
    ui->comboBox_16->setCurrentIndex(0);
    ui->comboBox_17->setCurrentIndex(0);
    ui->comboBox_18->setCurrentIndex(0);
    ui->comboBox_19->setCurrentIndex(0);
    ui->comboBox_20->setCurrentIndex(0);
    ui->comboBox_21->setCurrentIndex(0);
    ui->comboBox_22->setCurrentIndex(0);
    ui->comboBox_23->setCurrentIndex(0);
    ui->comboBox_24->setCurrentIndex(0);
    ui->comboBox_25->setCurrentIndex(0);
}

/* Slot: 'actionPinout_Guide' triggered -------------------------------------*/
void IotonConfigWindow::on_actionPinout_Guide_triggered()
{
    // Pinout guide title
    QLabel *label_title = new QLabel("<p><b><font size = 12 style=\"color: #ffffff\">Guia da Pinagem - Placa TON</b></p>");
    label_title->setAlignment(Qt::AlignCenter);

    // Pinout guide image
    QLabel *label_image = new QLabel();
    label_image->setPixmap(QPixmap(":/images/ton_v1.2_mbed.png").scaledToWidth(940, Qt::SmoothTransformation));
    label_image->setAlignment(Qt::AlignCenter);

    // Pinout guide text
    QString pinoutText = "<p><b><ul><li>OBSERVAÇÕES:</li></ul></b></p>";
    pinoutText += "<p><b>- ADC (AnalogIn):</b> a resolução do conversor é de 12-bit (4096 valores diferentes). Neste modo a tensão do pino não deve exceder 3,3 V.</p>";
    pinoutText += "<p><b>- CAN, I2C e SPI:</b> barramentos seriais para comunicação com uma infinidade de circuitos integrados.</p>";
    pinoutText += "<p><b>- DAC (AnalogOut):</b> saídas analógicas de 12-bit, resistência de carga mínima = 5 kΩ.</p>";
    pinoutText += "<p><b>- INPUT (DigitalIn):</b> todos os pinos podem ser configurados como entrada digital e também aceitam interrupção externa.</p>";
    pinoutText += "<p><b>- OUTPUT (DigitalOut):</b> todos os pinos podem ser configurados com saída digital.</p>";
    pinoutText += "<p><b>- PWM (PwmOut):</b> são 12 pinos de PWM com cinco grupos diferentes (alguns grupos possuem mais de um canal e são diferenciados pelas letras A, B, C e D). Cada grupo pode ser configurado com uma frequência diferente.</p>";
    pinoutText += "<p><b>- SDIO:</b> interface para cartões de multimídia (SD Card e MMC).</p>";
    pinoutText += "<p><b>- UART (Serial):</b> cada uma das quatro UART são configuradas separadas e podem ter baudrates diferentes.</p>";
    pinoutText += "<p> </p>";
    pinoutText += "<p>*Os pinos de I/O são 5V tolerantes, exceto os pinos 20 e 21.</p>";
    pinoutText += "<p> </p>";
    pinoutText += "<p>Mais detalhes em: <a href=\"http://ioton/ton\" style=\"color: #ffffff\">http://ioton/ton</a></p>";
    QLabel *label_text = new QLabel(pinoutText);
    label_text->setWordWrap(true);
    label_text->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

    // Layout of window
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label_title);
    layout->addWidget(label_image);
    layout->addWidget(label_text);

    // Show pinout guide window
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Guia da Pinagem - Placa TON");
    dialog->setWindowIcon(QIcon(":/images/icon1.png"));
    dialog->setLayout(layout);
    dialog->show();

    dialog->connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

/* Slot: 'actionManual_TON' triggered ---------------------------------------*/
void IotonConfigWindow::on_actionManual_TON_triggered()
{
    // Open link - Manual TON Board
    QDesktopServices::openUrl(QUrl("https://github.com/iotontech/ton/blob/master/manual_ton_v1_2.pdf"));
}

/* Slot: 'actionAbout_Ioton_Config' triggered -------------------------------*/
void IotonConfigWindow::on_actionAbout_Ioton_Config_triggered()
{
    // About text
    QString aboutText = "<p><b><font size = 12 style=\"color: #ffffff\">ioTon Config 0.2.0</b></p>";
    aboutText += "<p>Este programa é um gerador de projetos para a Plataforma TON.</p>";
    aboutText += "<hr><p>Copyright (c) 2016 <a href=\"http://ioton.cc\" style=\"color: #ffffff\">IOTON Technology</a>.</p>";
    aboutText += "<p>Licenciado com uma <a href=\"https://creativecommons.org/licenses/by-sa/4.0/\" style=\"color: #ffffff\">Licença Creative Commons Atribuição-CompartilhaIgual 4.0 Internacional</a>.</p>";
    aboutText += "<p> </p>";

    // Show about window
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("Sobre ioTon Config");
    aboutBox.setText(aboutText);
    aboutBox.setStandardButtons(QMessageBox::Close);
    aboutBox.setWindowIcon(QIcon(":/images/icon1.png"));
    aboutBox.setIconPixmap(QPixmap(":/images/icon1.png").scaledToHeight(128, Qt::SmoothTransformation));
    aboutBox.exec();
}


/** ----------------------------------------------------------------------- **/
/**                           Private functions                             **/
/** ----------------------------------------------------------------------- **/

/** Return the index of the 'comboBox_advanced' if the pin is present (only individual pins)
 *
 *  @param pinNumber The pin number to search in the 'comboBox_advanced'
 *
 *  @returns
 *    index if successful,
 *    -1 otherwise (not found)
 */
int IotonConfigWindow::getAdvancedPinIndex(int pinNumber)
{
    QString pin = QString("Pino %1:").arg(pinNumber);

    // Search the pin number in every comboBox_advanced items
    for (int index = 0; index < ui->comboBox_advanced->count(); index++)
    {
        if (ui->comboBox_advanced->itemText(index).contains(pin))
        {
            return index;
        }
    }

    return -1;
}

/** Return the number of the selected pin in the 'comboBox_advanced'
 *
 *  @param comboBoxText The text selected in the 'comboBox_advanced'
 *
 *  @returns
 *    pinNumber if successful (for group pins: the first number),
 *    -1 otherwise
 */
int IotonConfigWindow::getAdvancedPinNumber(QString comboBoxText)
{
    if (comboBoxText == NULL || comboBoxText == "")
    {
        return -1;
    }
    else if (comboBoxText == "CAN")
    {
        return 11;
    }
    else if (comboBoxText == "I2C")
    {
        return 11;
    }
    else if (comboBoxText == "SDIO")
    {
        return 2;
    }
    else if (comboBoxText == "SPI")
    {
        return 20;
    }
    else if (comboBoxText == "UART1")
    {
        return 0;
    }
    else if (comboBoxText == "UART2")
    {
        return 5;
    }
    else if (comboBoxText == "UART3")
    {
        return 7;
    }
    else if (comboBoxText == "UART4")
    {
        return 18;
    }
    else
    {
        QStringList pinNumber = comboBoxText.split(":");
        pinNumber = pinNumber[0].split(" ");
        return pinNumber[1].toInt();
    }

    return -1;
}

/** Return the mode of the selected pin in the 'comboBox_advanced'
 *
 *  @param comboBoxText The text selected in the 'comboBox_advanced'
 *
 *  @returns
 *    pinMode if successful,
 *    -1 no selection,
 *    -2 otherwise (invalid mode)
 */
int IotonConfigWindow::getAdvancedPinMode(QString comboBoxText)
{
    if (comboBoxText == NULL || comboBoxText == "")
    {
        return -1;
    }

    if (comboBoxText.contains("ADC"))
    {
        return ADC;
    }
    else if (comboBoxText.contains("CAN"))
    {
        return CAN;
    }
    else if (comboBoxText.contains("DAC"))
    {
        return DAC;
    }
    else if (comboBoxText.contains("I2C"))
    {
        return I2C;
    }
    else if (comboBoxText.contains("INPUT"))
    {
        return INPUT;
    }
    else if (comboBoxText.contains("OUTPUT"))
    {
        return OUTPUT;
    }
    else if (comboBoxText.contains("PWM"))
    {
        return PWM;
    }
    else if (comboBoxText.contains("SDIO"))
    {
        return SDIO;
    }
    else if (comboBoxText.contains("SPI"))
    {
        return SPI;
    }
    else if (comboBoxText.contains("UART"))
    {
        return UART;
    }

    return -2;
}

/** Fill the graphics elements (advanced tab) with the configurations of the selected pin (pinNumber)
 *
 *  @param pinNumber The pin number
 *  @param pinMode The pin mode
 */
void IotonConfigWindow::fillPinConfigurations(int pinNumber, int pinMode)
{
    // Checks whether the pin is valid
    if (pinNumber != -1 && pinMode != -1)
    {
        // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
        // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
        switch (pinMode)
        {
            case ADC:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                break;
            }   // end of case ADC

            case CAN:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                break;
            }   // end of case CAN

            case DAC:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                break;
            }   // end of case DAC

            case I2C:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                ui->lineEdit_advanced_1->setText(QString::number(pins[pinNumber].i2c.frequency));
                break;
            }   // end of case I2C

            case INPUT: // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                ui->checkBox_advanced_1->setChecked(pins[pinNumber].input.externalInterrupt);
                ui->comboBox_advanced_2->setCurrentText(pins[pinNumber].input.mode == PULLUP ? "PullUp" : pins[pinNumber].input.mode == PULLDOWN ? "PullDown" : pins[pinNumber].input.mode == OPENDRAIN ? "OpenDrain" : "PullNone");
                ui->comboBox_advanced_3->setCurrentText(pins[pinNumber].input.edge == RISE ? "Subida" : "Descida");
                break;
            }   // end of case INPUT

            case OUTPUT:    // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                break;
            }   // end of case OUTPUT

            case PWM:   // Type: individual pin
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                ui->lineEdit_advanced_1->setText(QString::number(pins[pinNumber].pwm.frequency));
                ui->lineEdit_advanced_2->setText(QString::number(pins[pinNumber].pwm.dutyCycle));
                break;
            }   // end of case PWM

            case SDIO:  // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                break;
            }   // end of case SDIO

            case SPI:   // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                ui->lineEdit_advanced_1->setText(QString::number(pins[pinNumber].spi.frequency));
                ui->lineEdit_advanced_2->setText(QString::number(pins[pinNumber].spi.bits));
                ui->comboBox_advanced_3->setCurrentText(QString::number(pins[pinNumber].spi.mode));
                break;
            }   // end of case SPI

            case UART:  // Type: peripheral (pin group)
            {
                ui->lineEdit_advanced_name->setText(pins[pinNumber].name);
                ui->comboBox_advanced_1->setCurrentText(QString::number(pins[pinNumber].uart.baud));
                ui->comboBox_advanced_2->setCurrentText(QString::number(pins[pinNumber].uart.bits));
                ui->comboBox_advanced_3->setCurrentText(QString::number(pins[pinNumber].uart.stopBit));
                ui->comboBox_advanced_4->setCurrentText("NONE");
                break;
            }   // end of case UART

            default:
            {
                break;
            }
        }   // end of switch (pinMode)

        // Load the default pin name
        on_lineEdit_advanced_name_editingFinished();
    }
}

/** Save the configurations of the selected pin (pinNumber)
 *
 *  @param pinNumber The pin number
 *  @param pinMode The pin mode
 */
void IotonConfigWindow::savePinConfigurations(int pinNumber, int pinMode)
{
    // Checks whether the pin is valid
    if (pinNumber != -1 && pinMode != -1)
    {
        // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
        // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
        switch (pinMode)
        {
            case ADC:   // Type: individual pin
            {
                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                break;
            }   // end of case ADC

            case CAN:   // Type: peripheral (pin group)
            {
                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                break;
            }   // end of case CAN

            case DAC:   // Type: individual pin
            {
                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                break;
            }   // end of case DAC

            case I2C:   // Type: peripheral (pin group)
            {
                pins[11].name = ui->lineEdit_advanced_name->text();
                pins[11].i2c.frequency = ui->lineEdit_advanced_1->text() == "" ? i2c.frequency : ui->lineEdit_advanced_1->text().toInt();
                pins[12].name = ui->lineEdit_advanced_name->text();
                pins[12].i2c.frequency = ui->lineEdit_advanced_1->text() == "" ? i2c.frequency : ui->lineEdit_advanced_1->text().toInt();
                break;
            }   // end of case I2C

            case INPUT: // Type: individual pin
            {
                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                pins[pinNumber].input.externalInterrupt = ui->checkBox_advanced_1->isChecked();
                pins[pinNumber].input.mode = ui->comboBox_advanced_2->currentText() == "PullUp" ? PULLUP : ui->comboBox_advanced_2->currentText() == "PullDown" ? PULLDOWN : ui->comboBox_advanced_2->currentText() == "OpenDrain" ? OPENDRAIN : PULLNONE;
                pins[pinNumber].input.edge = ui->comboBox_advanced_3->currentText() == "Subida" ? RISE : FALL;
                break;
            }   // end of case INPUT

            case OUTPUT:    // Type: individual pin
            {
                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                break;
            }   // end of case OUTPUT

            case PWM:   // Type: individual pin
            {
                int pwmPins[] = {-1, -1, -1, -1, -1};

                // PWM channel 1
                if (pinNumber == 0 || pinNumber == 1 || pinNumber == 2 || pinNumber == 3)
                {
                    pwmPins[0] = 0;
                    pwmPins[1] = 1;
                    pwmPins[2] = 2;
                    pwmPins[3] = 3;
                }
                // PWM channel 2
                else if (pinNumber == 4 || pinNumber == 9)
                {
                    pwmPins[0] = 4;
                    pwmPins[1] = 9;
                }
                // PWM channel 3
                else if (pinNumber == 10 || pinNumber == 13)
                {
                    pwmPins[0] = 10;
                    pwmPins[1] = 13;
                }
                // PWM channel 4
                else if (pinNumber == 11 || pinNumber == 12)
                {
                    pwmPins[0] = 11;
                    pwmPins[1] = 12;
                }
                // PWM channel 5
                else if (pinNumber == 18 || pinNumber == 19)
                {
                    pwmPins[0] = 18;
                    pwmPins[1] = 19;
                }

                pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                for (int *pinNumber = pwmPins; *pinNumber != -1; pinNumber++)
                {
                    pins[*pinNumber].pwm.frequency = ui->lineEdit_advanced_1->text().toInt();
                    pins[*pinNumber].pwm.dutyCycle = ui->lineEdit_advanced_2->text().toInt();

                }

                break;
            }   // end of case PWM

            case SDIO:  // Type: peripheral (pin group)
            {
                for (int pinNumber = 2; pinNumber <= 8; pinNumber++)
                {
                    if (pinNumber == 4) pinNumber++;
                    pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                }
                break;
            }   // end of case SDIO

            case SPI:   // Type: peripheral (pin group)
            {
                for (int pinNumber = 20; pinNumber <= 23; pinNumber++)
                {
                    pins[pinNumber].name = ui->lineEdit_advanced_name->text();
                    pins[pinNumber].spi.frequency = ui->lineEdit_advanced_1->text() == "" ? spi.frequency : ui->lineEdit_advanced_1->text().toInt();
                    pins[pinNumber].spi.bits = ui->lineEdit_advanced_2->text() == "" ? spi.bits : ui->lineEdit_advanced_2->text().toInt();
                    pins[pinNumber].spi.mode= ui->comboBox_advanced_3->currentText().toInt();
                }
                break;
            }   // end of case SPI

            case UART:  // Type: peripheral (pin group)
            {
                int uartPins[] = {-1, -1, -1};

                // UART 1
                if (pinNumber == 0 || pinNumber == 1)
                {
                    uartPins[0] = 0;
                    uartPins[1] = 1;
                }
                // UART 2
                else if (pinNumber == 5 || pinNumber == 6)
                {
                    uartPins[0] = 5;
                    uartPins[1] = 6;
                }
                // UART 3
                else if (pinNumber == 7 || pinNumber == 8)
                {
                    uartPins[0] = 7;
                    uartPins[1] = 8;
                }
                // UART 4
                else if (pinNumber == 18 || pinNumber == 19)
                {
                    uartPins[0] = 18;
                    uartPins[1] = 19;
                }

                for (int *pinNumber = uartPins; *pinNumber != -1; pinNumber++)
                {
                    pins[*pinNumber].name = ui->lineEdit_advanced_name->text();
                    pins[*pinNumber].uart.baud = ui->comboBox_advanced_1->currentText().toInt();
                    pins[*pinNumber].uart.bits = ui->comboBox_advanced_2->currentText().toInt();
                    pins[*pinNumber].uart.stopBit = ui->comboBox_advanced_3->currentText().toInt();
                    pins[*pinNumber].uart.parity = ui->comboBox_advanced_4->currentText() == "NONE" ? NONE : ui->comboBox_advanced_4->currentText() == "EVEN" ? EVEN : ui->comboBox_advanced_4->currentText() == "ODD" ? ODD : ui->comboBox_advanced_4->currentText() == "FORCED1" ? FORCED1 : FORCED0;
                }

                break;
            }   // end of case UART

            default:
            {
                break;
            }
        }   // end of switch (pinMode)
    }
}

/** Update 'Status Bar' with the use of the pins/peripherals (ADC, I2C, PWM, SPI, UART)
 */
void IotonConfigWindow::updateStatusBar(void)
{
    int numUsedAdc  = 0;
    int numUsedPwm  = 0;
    int numUsedUart = 0;
    bool usingSpi   = false;
    bool usingI2c  = false;

    // Search for: ADC, I2C, PWM, SPI or UART
    for (int pinNumber = 0; pinNumber < N_PINS; pinNumber++)
    {
        switch (pins[pinNumber].pinModeUsed)
        {
            case ADC:
                numUsedAdc++;
                break;

            case I2C:
                usingI2c = true;
                break;

            case PWM:
                numUsedPwm++;
                break;

            case SPI:
                usingSpi = true;
                break;

            case UART:
                numUsedUart++;
                break;

            default:
                break;
        }   // end of switch
    }   // end of for

    // Update 'Status Bar' text
    QString statusBarText = "ADC: " + QString::number(numUsedAdc) + "/" + QString::number(numAdcAvailable);
    statusBarText += "       PWM: " + QString::number(numUsedPwm) + "/" + QString::number(numPwmAvailable);
    statusBarText += "       UART: " + QString::number(numUsedUart / 2) + "/" + QString::number(numUartAvailable);
    statusBarText += usingI2c ? "       I2C" : "";
    statusBarText += usingSpi ? "       SPI" : "";
    ui->statusBar->showMessage(statusBarText);
}

/** Update configurations of the selected pin (pinNumber)
 *
 *  @param pinNumber The pin number
 *  @param pinMode The pin mode
 */
void IotonConfigWindow::updatePinConfigurations(int pinNumber, int pinMode)
{
    // Check if is valid pin
    if (pinNumber != -1)
    {
        switch (pinMode)
        {
            case PWM:   // Linkage of PWM channel
            {
                int pwmPins[] = {-1, -1, -1, -1, -1};

                // PWM channel 1
                if (pinNumber == 0 || pinNumber == 1 || pinNumber == 2 || pinNumber == 3)
                {
                    pwmPins[0] = 0;
                    pwmPins[1] = 1;
                    pwmPins[2] = 2;
                    pwmPins[3] = 3;
                }
                // PWM channel 2
                else if (pinNumber == 4 || pinNumber == 9)
                {
                    pwmPins[0] = 4;
                    pwmPins[1] = 9;
                }
                // PWM channel 3
                else if (pinNumber == 10 || pinNumber == 13)
                {
                    pwmPins[0] = 10;
                    pwmPins[1] = 13;
                }
                // PWM channel 4
                else if (pinNumber == 11 || pinNumber == 12)
                {
                    pwmPins[0] = 11;
                    pwmPins[1] = 12;
                }
                // PWM channel 5
                else if (pinNumber == 18 || pinNumber == 19)
                {
                    pwmPins[0] = 18;
                    pwmPins[1] = 19;
                }

                // Update the pin configurations (all channels) in the 'Tree View'
                for (int *pinNumber = pwmPins; *pinNumber != -1; pinNumber++)
                {
                    updateTreeViewItem(*pinNumber, pinMode);
                }

                break;
            }   // end of case PWM

            default:
            {
                // Update the pin configurations in the 'Tree View'
                updateTreeViewItem(pinNumber, pinMode);
                break;
            }
        }   // end of switch (pinMode)
    }   // end of if is valid pin
}

/** Update configurations (advanced tab - tree view) of the selected pin (pinNumber)
 *
 *  @param pinNumber The pin number
 *  @param pinMode The pin mode
 */
void IotonConfigWindow::updateTreeViewItem(int pinNumber, int pinMode)
{
    // Check if the pin was removed and clean their configurations
    if (pins[pinNumber].pinModeUsed == pinMode)
    {
        while (treeViewItems.at(pinNumber)->childCount())
        {
            QTreeWidgetItem *subItem = treeViewItems.at(pinNumber)->child(0);
            subItem->~QTreeWidgetItem();
        }

        treeViewItems.at(pinNumber)->setHidden(true);
    }

    // Update the pin configurations in the 'Tree View'
    if (pinMode != -1 && pins[pinNumber].pinModeUsed == pinMode)
    {
        QList<QTreeWidgetItem *> subItems;

        // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
        // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
        switch (pinMode)
        {
            case ADC:   // Type: individual pin
            {
                subItems.append(new QTreeWidgetItem());
                subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                treeViewItems.at(pinNumber)->addChildren(subItems);
                treeViewItems.at(pinNumber)->setText(0, "Pino " + QString::number(pinNumber) + " - ADC");
                treeViewItems.at(pinNumber)->setHidden(false);

                break;
            }   // end of case ADC

            case CAN:   // Type: peripheral (pin group)
            {
                if (pinNumber == 11)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                    treeViewItems.at(pinNumber)->addChildren(subItems);
                    treeViewItems.at(pinNumber)->setText(0, "Pinos: 11, 12 - CAN");
                    treeViewItems.at(pinNumber)->setHidden(false);
                }

                break;
            }   // end of case CAN

            case DAC:   // Type: individual pin
            {
                subItems.append(new QTreeWidgetItem());
                subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                treeViewItems.at(pinNumber)->addChildren(subItems);
                treeViewItems.at(pinNumber)->setText(0, "Pino " + QString::number(pinNumber) + " - DAC");
                treeViewItems.at(pinNumber)->setHidden(false);

                break;
            }   // end of case DAC

            case I2C:   // Type: peripheral (pin group)
            {
                if (pinNumber == 11)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                    subItems.at(1)->setText(0, "Frequência (Hz): " + QString::number(pins[pinNumber].i2c.frequency));
                    treeViewItems.at(pinNumber)->addChildren(subItems);
                    treeViewItems.at(pinNumber)->setText(0, "Pinos: 11, 12 - I2C");
                    treeViewItems.at(pinNumber)->setHidden(false);
                }

                break;
            }   // end of case I2C

            case INPUT: // Type: individual pin
            {
                subItems.append(new QTreeWidgetItem());
                subItems.append(new QTreeWidgetItem());
                subItems.append(new QTreeWidgetItem());
                subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                subItems.at(1)->setText(0, QString("Modo: ") + (pins[pinNumber].input.mode == PULLUP ? "PullUp" : pins[pinNumber].input.mode == PULLDOWN ? "PullDown" : pins[pinNumber].input.mode == OPENDRAIN ? "OpenDrain" : "PullNone"));
                subItems.at(2)->setText(0, QString("Interrupção externa: ") + (pins[pinNumber].input.externalInterrupt ? "true" : "false"));
                if (pins[pinNumber].input.externalInterrupt)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(3)->setText(0, QString("Borda: ") + (pins[pinNumber].input.edge == RISE ? "Subida" : "Descida"));
                }
                treeViewItems.at(pinNumber)->addChildren(subItems);
                treeViewItems.at(pinNumber)->setText(0, "Pino " + QString::number(pinNumber) + " - INPUT");
                treeViewItems.at(pinNumber)->setHidden(false);

                break;
            }   // end of case INPUT

            case OUTPUT:    // Type: individual pin
            {
                subItems.append(new QTreeWidgetItem());
                subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                treeViewItems.at(pinNumber)->addChildren(subItems);
                treeViewItems.at(pinNumber)->setText(0, "Pino " + QString::number(pinNumber) + " - OUTPUT");
                treeViewItems.at(pinNumber)->setHidden(false);

                break;
            }   // end of case OUTPUT

            case PWM:   // Type: individual pin
            {
                subItems.append(new QTreeWidgetItem());
                subItems.append(new QTreeWidgetItem());
                subItems.append(new QTreeWidgetItem());
                subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                subItems.at(1)->setText(0, "Frequência (Hz): " + QString::number(pins[pinNumber].pwm.frequency));
                subItems.at(2)->setText(0, "Duty cycle (%): " + QString::number(pins[pinNumber].pwm.dutyCycle));
                treeViewItems.at(pinNumber)->addChildren(subItems);
                treeViewItems.at(pinNumber)->setText(0, "Pino " + QString::number(pinNumber) + " - PWM");
                treeViewItems.at(pinNumber)->setHidden(false);

                break;
            }   // end of case PWM

            case SDIO:  // Type: peripheral (pin group)
            {
                if (pinNumber == 2)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                    treeViewItems.at(pinNumber)->addChildren(subItems);
                    treeViewItems.at(pinNumber)->setText(0, "Pinos: 2, 3, 5, 6, 7, 8 - SDIO");
                    treeViewItems.at(pinNumber)->setHidden(false);
                }

                break;
            }   // end of case SDIO

            case SPI:   // Type: peripheral (pin group)
            {
                if (pinNumber == 20)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                    subItems.at(1)->setText(0, "Frequência (Hz): " + QString::number(pins[pinNumber].spi.frequency));
                    subItems.at(2)->setText(0, "Bits: " + QString::number(pins[pinNumber].spi.bits));
                    subItems.at(3)->setText(0, "Modo: " + QString::number(pins[pinNumber].spi.mode));
                    treeViewItems.at(pinNumber)->addChildren(subItems);
                    treeViewItems.at(pinNumber)->setText(0, "Pinos: 20, 21, 22, 23 - SPI");
                    treeViewItems.at(pinNumber)->setHidden(false);
                }

                break;
            }   // end of case SPI

            case UART:  // Type: peripheral (pin group)
            {
                if (pinNumber == 0 || pinNumber == 5 || pinNumber == 7 || pinNumber == 18)
                {
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.append(new QTreeWidgetItem());
                    subItems.at(0)->setText(0, "Nome: " + pins[pinNumber].name);
                    subItems.at(1)->setText(0, "Baud rate: " + QString::number(pins[pinNumber].uart.baud));
                    subItems.at(2)->setText(0, "Bits: " + QString::number(pins[pinNumber].uart.bits));
                    subItems.at(3)->setText(0, "Stop bits: " + QString::number(pins[pinNumber].uart.stopBit));
                    subItems.at(4)->setText(0, QString("Paridade: ") + (pins[pinNumber].uart.parity == EVEN ? "even" : pins[pinNumber].uart.parity == ODD ? "odd" : pins[pinNumber].uart.parity == FORCED1 ? "forced1" : pins[pinNumber].uart.parity == FORCED0 ? "forced0" : "none"));
                    treeViewItems.at(pinNumber)->addChildren(subItems);
                    treeViewItems.at(pinNumber)->setText(0, "Pinos: " + QString::number(pinNumber) + ", " + QString::number(pinNumber + 1) + " - UART");
                    treeViewItems.at(pinNumber)->setHidden(false);
                }

                break;
            }   // end of case UART

            default:
            {
                break;
            }
        }   // end of switch (pinMode)
    }   // end of update pin configurations
}

/** Load the default pins names
 */
void IotonConfigWindow::pinNameDefault(void)
{
    for (int pinNumber = 0; pinNumber < N_PINS; pinNumber++)
    {
        int pinMode = pins[pinNumber].pinModeUsed;

        // Checks if default value
        if (pinMode != -1 && pins[pinNumber].name == "default")
        {
            // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
            // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
            switch (pinMode)
            {
                case ADC:
                {
                    pins[pinNumber].name = "pin" + QString::number(pinNumber) + "_AnalogIn";
                    break;
                }

                case CAN:
                {
                    pins[pinNumber].name = "can";
                    break;
                }

                case DAC:
                {
                    pins[pinNumber].name = "pin" + QString::number(pinNumber) + "_AnalogOut";
                    break;
                }

                case I2C:
                {
                    pins[pinNumber].name = "i2c";
                    break;
                }

                case INPUT:
                {
                    pins[pinNumber].name = "pin" + QString::number(pinNumber) + "_DigitalIn";
                    break;
                }

                case OUTPUT:
                {
                    pins[pinNumber].name = "pin" + QString::number(pinNumber) + "_DigitalOut";
                    break;
                }

                case PWM:
                {
                    pins[pinNumber].name = "pin" + QString::number(pinNumber) + "_PwmOut";
                    break;
                }

                case SDIO:
                {
                    pins[pinNumber].name = "sdio";
                    break;
                }

                case SPI:
                {
                    pins[pinNumber].name = "spi";
                    break;
                }

                case UART:
                {
                    if (pinNumber == 0 || pinNumber == 1)
                    {
                        pins[pinNumber].name = "uart1";
                    }
                    else if (pinNumber == 5 || pinNumber == 6)
                    {
                        pins[pinNumber].name = "uart2";
                    }
                    else if (pinNumber == 7 || pinNumber == 8)
                    {
                        pins[pinNumber].name = "uart3";
                    }
                    else if (pinNumber == 18 || pinNumber == 19)
                    {
                        pins[pinNumber].name = "uart4";
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }

            // Save configurations and update elements
            updatePinConfigurations(pinNumber, pinMode);
        }   // end of if default value
    }   // end of for N_PINS
}

/** Hide all graphics elements of advanced tab
 */
void IotonConfigWindow::toggleAdvancedOff(void)
{
    ui->label_advanced_name->setVisible(false);
    ui->lineEdit_advanced_name->setVisible(false);
    ui->checkBox_advanced_1->setVisible(false);
    ui->checkBox_advanced_2->setVisible(false);
    ui->comboBox_advanced_1->setVisible(false);
    ui->comboBox_advanced_2->setVisible(false);
    ui->comboBox_advanced_3->setVisible(false);
    ui->comboBox_advanced_4->setVisible(false);
    ui->label_advanced_1->setVisible(false);
    ui->label_advanced_2->setVisible(false);
    ui->label_advanced_3->setVisible(false);
    ui->label_advanced_4->setVisible(false);
    ui->lineEdit_advanced_1->setVisible(false);
    ui->lineEdit_advanced_2->setVisible(false);
}
