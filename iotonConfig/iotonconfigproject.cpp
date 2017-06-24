/**
 ******************************************************************************
 *
 * iotonconfigproject.cpp - Implementation of ioTon Config (iotonconfigproject)
 *  - This class has a set of methods to create the project
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
#include "iotonconfigproject.h"
#include "iotonconfigwindow.h"


/* Constructor --------------------------------------------------------------*/
IotonConfigProject::IotonConfigProject()
{
}


/** ----------------------------------------------------------------------- **/
/**                             Static methods                              **/
/** ----------------------------------------------------------------------- **/

/** Return the status to try create the project
 *
 *  @param projectPath The absolute path of the project name
 *
 *  @returns
 *    0 if successful,
 *    -1 error creating the folder,
 *    -2 error creating the PlatformIO files
 */
int IotonConfigProject::createProject(const QString projectPath)
{
    QProcess processPlatformio;

    // Creates the project folder
    if (!QDir().mkdir(projectPath))
    {
        return -1;  // Error creating the folder
    }

    // Start Platformio - initialize new PlatformIO based project
#if defined(Q_OS_WIN)
    processPlatformio.start(QDir::homePath() + "\\.platformio\\penv\\Scripts\\pio -f init -b ton_f405rg -d " + projectPath);
#else
    processPlatformio.start(QDir::homePath() + "/.platformio/penv/bin/pio -f init -b ton_f405rg -d " + projectPath);
#endif

    // Creates the PlatformIO based project
    if (processPlatformio.waitForFinished() == false)
    {
        return -2;  // Error creating the PlatformIO files
    }

    return 0;   // Success
}


/** Return the status to try create the template
 *
 *  @param projectPath The absolute path of the project name
 *  @param pins The pins configurations array
 *  @param examples If true then generate examples
 *
 *  @returns
 *    0 if successful,
 *    -1 error creating the template
 */
int IotonConfigProject::createTemplate(const QString projectPath, IotonConfigWindow::pin pins[], bool examples)
{
    int status = 0;
    int input = -1, interrupt = -1, output = -1, pwm = -1, adc = -1, can = -1, dac = -1, i2c = -1, sdio = -1, spi = -1, uart = -1;
    bool advanced = false, advancedPins[N_PINS] = {false};
    struct IotonConfigWindow::pin defaultPin;

    // Declaration of files (templates, main.h and main.cpp)
    QString executablePath = QCoreApplication::applicationDirPath();
#if defined(Q_OS_WIN)
    QFile mainSourceFile(projectPath + "\\src\\main.cpp");
    QFile mainHeaderFile(projectPath + "\\src\\main.h");
    QFile mainCppPt1File(executablePath + "\\templates\\main.cpp.pt1");
    QFile mainCppPt2File(executablePath + "\\templates\\main.cpp.pt2");
    QFile mainCppPt3File(executablePath + "\\templates\\main.cpp.pt3");
    QFile mainHPt1File(executablePath + "\\templates\\main.h.pt1");
#else
    QFile mainSourceFile(projectPath + "/src/main.cpp");
    QFile mainHeaderFile(projectPath + "/src/main.h");
    QFile mainCppPt1File(executablePath + "/templates/main.cpp.pt1");
    QFile mainCppPt2File(executablePath + "/templates/main.cpp.pt2");
    QFile mainCppPt3File(executablePath + "/templates/main.cpp.pt3");
    QFile mainHPt1File(executablePath + "/templates/main.h.pt1");
#endif

    // Try read templates files and generates source code (main.h and main.cpp)
    if (mainSourceFile.open(QIODevice::WriteOnly) &&
        mainHeaderFile.open(QIODevice::WriteOnly) &&
        mainCppPt1File.open(QIODevice::ReadOnly) &&
        mainCppPt2File.open(QIODevice::ReadOnly) &&
        mainCppPt3File.open(QIODevice::ReadOnly) &&
        mainHPt1File.open(QIODevice::ReadOnly))
    {
        QTextStream mainCppPt1Stream(&mainCppPt1File);
        QTextStream mainCppPt2Stream(&mainCppPt2File);
        QTextStream mainCppPt3Stream(&mainCppPt3File);
        QTextStream mainHPt1Stream(&mainHPt1File);

        /** -------------------------------------------------------------------
         *      main.cpp structure:
         * --------------------------------------------------------------------
         *  Part 1 - header
         *  Part 2 - private variables
         *  Part 3 - interrupts callbacks (if interrupt pin)
         *  Part 4 - examples of use (if 'bool examples' = true)
         *  Part 5 - initializations (if advanced configurations)
         *  Part 6 - main function
         *  Part 7 - call init function (if advanced configurations)
         *  Part 8 - main loop
         *********************************************************************/

        /** Generates part 1 - main.h (all contents), main.cpp (header) */
        mainSourceFile.write(mainCppPt1Stream.readAll().toLatin1().data());
        mainHeaderFile.write(mainHPt1Stream.readAll().toLatin1().data());
        // end of part 1

        /** Generates part 2 - main.cpp (private variables) */
        for (int i = 0; i < N_PINS; i++)
        {
            // Individual pins: ADC, DAC, INPUT, OUTPUT, PWM
            // Peripherals (pin group): CAN, I2C, SDIO, SPI, UART
            switch (pins[i].pinModeUsed)
            {
                case ADC:   // Type: individual pin
                {
                    QString pinName = "AnalogIn " + pins[i].name + "(PIN" + QString::number(i) + ");\n";
                    mainSourceFile.write(pinName.toLatin1().data());
                    adc = i;

                    break;
                }   // end of case ADC

                case CAN:   // Type: peripheral (pin group)
                {
                    if (i == 11)
                    {
                        QString pinName = "CAN " + pins[i].name + "(PIN11, PIN12);\n";
                        mainSourceFile.write(pinName.toLatin1().data());
                        can = i;
                    }

                    break;
                }   // end of case CAN

                case DAC:   // Type: individual pin
                {
                    QString pinName = "AnalogOut " + pins[i].name + "(PIN" + QString::number(i) + ");\n";
                    mainSourceFile.write(pinName.toLatin1().data());
                    dac = i;

                    break;
                }   // end of case DAC

                case I2C:   // Type: peripheral (pin group)
                {
                    if (i == 11)
                    {
                        QString pinName = "I2C " + pins[i].name + "(PIN12, PIN11);\n";
                        mainSourceFile.write(pinName.toLatin1().data());
                        i2c = i;

                        // Signal if advanced configurations - i2c
                        if (pins[i].i2c.frequency != defaultPin.i2c.frequency)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }

                    break;
                }   // end of case I2C

                case INPUT: // Type: individual pin
                {
                    QString pinName = (pins[i].input.externalInterrupt ? "InterruptIn " : "DigitalIn ") + pins[i].name + "(PIN" + QString::number(i) + ");\n";
                    mainSourceFile.write(pinName.toLatin1().data());

                    // Signal if advanced configurations - external interrupt
                    if (pins[i].input.externalInterrupt)
                    {
                        interrupt = i;
                        advancedPins[i] = true;
                        advanced = true;
                    }
                    else
                    {
                        input = i;

                        // Signal if advanced configurations - digital input
                        if (pins[i].input.mode != defaultPin.input.mode)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }

                    break;
                }   // end of case INPUT

                case OUTPUT:    // Type: individual pin
                {
                    QString pinName = "DigitalOut " + pins[i].name + "(PIN" + QString::number(i) + ");\n";
                    mainSourceFile.write(pinName.toLatin1().data());
                    output = i;

                    break;
                }   // end of case OUTPUT

                case PWM:   // Type: individual pin
                {
                    QString pinName = "PwmOut " + pins[i].name + "(PIN" + QString::number(i) + ");\n";
                    mainSourceFile.write(pinName.toLatin1().data());
                    pwm = i;

                    // Signal if advanced configurations - pwm out
                    if (pins[i].pwm.dutyCycle != defaultPin.pwm.dutyCycle || pins[i].pwm.frequency != defaultPin.pwm.frequency)
                    {
                        advancedPins[i] = true;
                        advanced = true;
                    }

                    break;
                }   // end of case PWM

                case SDIO:  // Type: peripheral (pin group)
                {
                    if (i == 2)
                    {
                        QString pinName = "SDIO " + pins[i].name + "(PIN7, PIN8, PIN2, PIN3, PIN5, PIN6, \"" + pins[i].name + "\");\n";
                        mainSourceFile.write(pinName.toLatin1().data());
                        sdio = i;
                    }

                    break;
                }   // end of case SDIO

                case SPI:   // Type: peripheral (pin group)
                {
                    if (i == 20)
                    {
                        QString pinName = "SPI " + pins[i].name + "(PIN23, PIN22, PIN21, PIN20);\n";
                        mainSourceFile.write(pinName.toLatin1().data());
                        spi = i;

                        // Signal if advanced configurations - spi
                        if (pins[i].spi.bits != defaultPin.spi.bits || pins[i].spi.frequency != defaultPin.spi.frequency || pins[i].spi.mode != defaultPin.spi.mode)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }

                    break;
                }   // end of case SPI

                case UART:  // Type: peripheral (pin group)
                {
                    QString pinName = NULL;

                    if (i == 0)         // UART1
                    {
                        pinName += "Serial " + pins[i].name + "(PIN0, PIN1);\n";
                        uart = i;

                        // Signal if advanced configurations - uart
                        if (pins[i].uart.baud != defaultPin.uart.baud || pins[i].uart.bits != defaultPin.uart.bits || pins[i].uart.parity != defaultPin.uart.parity || pins[i].uart.stopBit != defaultPin.uart.stopBit)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }
                    else if (i == 5)    // UART2
                    {
                        pinName += "Serial " + pins[i].name + "(PIN5, PIN6);\n";
                        uart = i;

                        // Signal if advanced configurations - uart
                        if (pins[i].uart.baud != defaultPin.uart.baud || pins[i].uart.bits != defaultPin.uart.bits || pins[i].uart.parity != defaultPin.uart.parity || pins[i].uart.stopBit != defaultPin.uart.stopBit)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }
                    else if (i == 7)    // UART3
                    {
                        pinName += "Serial " + pins[i].name + "(PIN7, PIN8);\n";
                        uart = i;

                        // Signal if advanced configurations - uart
                        if (pins[i].uart.baud != defaultPin.uart.baud || pins[i].uart.bits != defaultPin.uart.bits || pins[i].uart.parity != defaultPin.uart.parity || pins[i].uart.stopBit != defaultPin.uart.stopBit)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }
                    else if (i == 18)   // UART4
                    {
                        pinName += "Serial " + pins[i].name + "(PIN18, PIN19);\n";
                        uart = i;

                        // Signal if advanced configurations - uart
                        if (pins[i].uart.baud != defaultPin.uart.baud || pins[i].uart.bits != defaultPin.uart.bits || pins[i].uart.parity != defaultPin.uart.parity || pins[i].uart.stopBit != defaultPin.uart.stopBit)
                        {
                            advancedPins[i] = true;
                            advanced = true;
                        }
                    }

                    if (pinName != NULL)
                    {
                        mainSourceFile.write(pinName.toLatin1().data());
                    }

                    break;
                }   // end of case UART

                default:
                {
                    break;
                }
            }   // end of switch (pinMode)
        }   // end of part 2

        /** Generates part 3 - main.cpp (interrupts callbacks) */
        if (interrupt != -1)
        {
            QString callbackCode = NULL;

            callbackCode += "\n\n// Interrupts callbacks --------------------------------------------------------";

            // Generates callbacks
            for (int i = 0; i < N_PINS; i++)
            {
                if (advancedPins[i] == true && pins[i].input.externalInterrupt == true)
                {
                    callbackCode += "\nvoid callback_" + pins[i].name + "(void)\n";
                    callbackCode += "{\n    // your code here\n}\n";
                }
            }

            mainSourceFile.write(callbackCode.toLatin1().data());
        }   // end of part 3

        /** Generates part 4 - main.cpp (examples of use) */
        if (examples == true)
        {
            QString exampleCode = NULL;

            exampleCode += "\n\n// Examples of use -------------------------------------------------------------\n";
            exampleCode += "void examples(void)\n{\n";

            // ADC example
            if (adc != -1)
            {
                exampleCode += "    // AnalogIn\n";
                exampleCode += "    float adc_value = " + pins[adc].name + "; // Read AnalogIn\n\n";
            }   // end of ADC example

            // DAC example
            if (dac != -1)
            {
                exampleCode += "    // AnalogOut\n";
                exampleCode += "    " + pins[dac].name + " = 0.5; // Update AnalogOut\n\n";
            }   // end of DAC example

            // INPUT example
            if (input != -1)
            {
                exampleCode += "    // DigitalIn\n";
                exampleCode += "    if (" + pins[input].name + ") // Read DigitalIn\n";
                exampleCode += "    {\n        // your code here\n    }\n\n";
            }   // end of INPUT example

            // INTERRUPT example
            if (interrupt != -1)
            {
                exampleCode += "    // InterruptIn\n";
                exampleCode += "    " + pins[interrupt].name + (pins[interrupt].input.edge == RISE ? ".rise" : ".fall") + "(&callback_" + pins[interrupt].name + "); // Attach the address of the function\n\n";
            }   // end of INTERRUPT example

            // OUTPUT example
            if (output != -1)
            {
                exampleCode += "    // DigitalOut\n";
                exampleCode += "    " + pins[output].name + " = 1; // Set DigitalOut\n";
                exampleCode += "    wait(1);\n";
                exampleCode += "    " + pins[output].name + " = 0; // Clear DigitalOut\n\n";
            }   // end of OUTPUT example

            // PWM example
            if (pwm != -1)
            {
                exampleCode += "    // PwmOut\n";
                exampleCode += "    " + pins[pwm].name + " = 0.5; // Update duty cycle\n\n";
            }   // end of PWM example

            // CAN example
            if (can != -1)
            {
                exampleCode += "    // CAN interface\n";
                exampleCode += "    CANMessage msg;\n";
                exampleCode += "    int data = 0x01;\n";
                exampleCode += "    " + pins[can].name + ".write(CANMessage(405, &data, 1)); // Write a CANMessage(id, data, lenght)\n";
                exampleCode += "    " + pins[can].name + ".read(msg); // Read a CANMessage\n\n";
            }   // end of CAN example

            // I2C example
            if (i2c != -1)
            {
                exampleCode += "    // I2C - bus master\n";
                exampleCode += "    const int add = 0x55; // Slave addresss\n";
                exampleCode += "    char data[2] = {0x01, 0x00}; // Data buffer\n";
                exampleCode += "    " + pins[i2c].name + ".write(add, data, 2); // Write 2 bytes to I2C\n";
                exampleCode += "    " + pins[i2c].name + ".read(add, data, 2); // Read 2 bytes to I2C\n\n";
            }   // end of CAN example

            // SDIO example
            if (sdio != -1)
            {
                exampleCode += "    // SDIO interface\n";
                exampleCode += "    FILE *fp = fopen(\"/" + pins[sdio].name + "/myfile.txt\", \"w\");\n";
                exampleCode += "    fprintf(fp, \"Hello World!\\n\");\n";
                exampleCode += "    fclose(fp);\n\n";
            }   // end of SDIO example

            // SPI example
            if (spi != -1)
            {
                exampleCode += "    // SPI - bus master\n";
                exampleCode += "    " + pins[spi].name + ".write(0x55); // Send the register\n";
                exampleCode += "    int response = " + pins[spi].name + ".write(0x00); // Send a dummy byte to receive the contents of the register\n\n";
            }   // end of SPI example

            // UART example
            if (uart != -1)
            {
                exampleCode += "    // Serial\n";
                exampleCode += "    " + pins[uart].name + ".printf(\"Hello World\\n\"); // Write a message\n";
                exampleCode += "    " + pins[uart].name + ".putc(" + pins[uart].name + ".getc()); // Echo\n\n";
            }   // end of UART example

            // IOTON examples
            exampleCode += "    // Class Ioton - examples\n";
            exampleCode += "    //   see demo code: https://github.com/iotontech/ton_demo\n";
            exampleCode += "    if (ton.USERisPressed()) ton.setLED(\"00AFEF\");\n";
            exampleCode += "    bluetooth.printf(\"Battery: %0.2f V\\n\", ton.getBattery()); // Prints messages to the Bluethooth\n";
            exampleCode += "    usb.printf(\"pitch: %0.3f\\n\", imu.getPitch()); // Prints messages to the USB\n";
            exampleCode += "    wifi.connect(\"MY_SSID\", \"MY_PASS\"); // Try to connect to the access point\n";
            // end of IOTON examples

            exampleCode += "}\n";

            mainSourceFile.write(exampleCode.toLatin1().data());
        }   // end of part 4

        /** Generates part 5 - main.cpp (initializations) */
        if (advanced == true)
        {
            QString advancedCode = NULL;

            advancedCode += "\n\n// Initializations -------------------------------------------------------------\n";
            advancedCode += "void init(void)\n{\n";
            advancedCode += "    // Advanced configurations\n";

            // Generate advanced configurations
            for (int i = 0; i < N_PINS; i++)
            {
                // If the pin/peripheral has advanced configurations...
                if (advancedPins[i] == true)
                {
                    // Pins/peripheral with advanced configurations: I2C, INPUT, PWM, SPI, UART
                    switch (pins[i].pinModeUsed)
                    {
                        case I2C:   // Frequency
                        {
                            advancedCode += "    " + pins[i].name + ".frequency(" + QString::number(pins[i].i2c.frequency) + ");\n";

                            break;
                        }   // end of case I2C

                        case INPUT: // Mode, edge
                        {
                            if (pins[i].input.mode != defaultPin.input.mode)
                            {
                                advancedCode += "    " + pins[i].name + ".mode(" + (pins[i].input.mode == PULLUP ? "PullUp" : pins[i].input.mode == PULLDOWN ? "PullDown" : pins[i].input.mode == OPENDRAIN ? "OpenDrain" : "PullNone") + ");\n";
                            }

                            if (pins[i].input.externalInterrupt)
                            {
                                advancedCode += "    " + pins[i].name + (pins[i].input.edge == RISE ? ".rise" : ".fall") + "(&callback_" + pins[i].name + ");\n";
                            }

                            break;
                        }   // end of case INPUT

                        case PWM:   // Frequency, duty cycle
                        {
                            if (pins[i].pwm.frequency != defaultPin.pwm.frequency)
                            {
                                advancedCode += "    " + pins[i].name + ".period(" + QString::number(1.0f / (float)pins[i].pwm.frequency) + ");\n";
                            }

                            if (pins[i].pwm.dutyCycle != defaultPin.pwm.dutyCycle)
                            {
                                advancedCode += "    " + pins[i].name + " = " + QString::number((float)pins[i].pwm.dutyCycle / 100.0f) + ";\n";
                            }

                            break;
                        }   // end of case PWM

                        case SPI:   // Bits, mode, frequency
                        {
                            if (pins[i].spi.bits != defaultPin.spi.bits || pins[i].spi.mode != defaultPin.spi.mode)
                            {
                                advancedCode += "    " + pins[i].name + ".format(" + QString::number(pins[i].spi.bits) + ", " + QString::number(pins[i].spi.mode) + ");\n";
                            }

                            if (pins[i].spi.frequency != defaultPin.spi.frequency)
                            {
                                advancedCode += "    " + pins[i].name + ".frequency(" + QString::number(pins[i].spi.frequency) + ");\n";
                            }

                            break;
                        }   // end of case SPI

                        case UART:  // Baudrate, bits, stop bits, parity
                        {
                            if (pins[i].uart.baud != defaultPin.uart.baud)
                            {
                                advancedCode += "    " + pins[i].name + ".baud(" + QString::number(pins[i].uart.baud) + ");\n";
                            }

                            if (pins[i].uart.bits != defaultPin.uart.bits || pins[i].uart.parity != defaultPin.uart.parity || pins[i].uart.stopBit != defaultPin.uart.stopBit)
                            {
                                advancedCode += "    " + pins[i].name + ".format(" + QString::number(pins[i].uart.bits) + ", SerialBase::";
                                advancedCode += (pins[i].uart.parity == EVEN ? "Even" : pins[i].uart.parity == ODD ? "Odd" : pins[i].uart.parity == FORCED1 ? "Forced1" : pins[i].uart.parity == FORCED0 ? "Forced0" : "None");
                                advancedCode += ", " + QString::number(pins[i].uart.stopBit) + ");\n";
                            }

                            break;
                        }   // end of case UART

                        default:
                        {
                            break;
                        }
                    }   // end of switch (pinMode)
                }   // end of if - advanced configurations
            }   // end of for - advanced configurations

            advancedCode += "}\n";

            mainSourceFile.write(advancedCode.toLatin1().data());
        }   // end of part 5

        /** Generates part 6 - main.cpp (main function) */
        mainSourceFile.write(mainCppPt2Stream.readAll().toLatin1().data());
        // end of part 6

        /** Generates part 7 - main.cpp (call init function) */
        if (advanced == true)
        {
            QString advancedCode = "    // Initialization of Peripherals\n    init();\n\n";
            mainSourceFile.write(advancedCode.toLatin1().data());
        }   // end of part 7

        /** Generates part 8 - main.cpp (main loop) */
        mainSourceFile.write(mainCppPt3Stream.readAll().toLatin1().data());
        // end of part 8

    }   // end of open files (if successful)
    else
    {
        status = -1;    // Error
    }

    // Close the files
    mainSourceFile.close();
    mainHeaderFile.close();
    mainCppPt1File.close();
    mainCppPt2File.close();
    mainCppPt3File.close();
    mainHPt1File.close();

    return status;
}

/** Open the project with Atom
 *
 *  @param projectPath The absolute path of the project name
 */
void IotonConfigProject::openAtomFolder(const QString projectPath)
{
    QProcess processAtom;

    // Start Atom - open path as a new project
#if defined(Q_OS_WIN)
    processAtom.start(QDir::homePath() + "/AppData/Local/atom/bin/atom.cmd -a " + projectPath);
#else
    processAtom.start("atom -a " + projectPath);
#endif

    processAtom.waitForStarted();
    processAtom.waitForFinished();
}
