/**
 ******************************************************************************
 *
 * iotonconfigproject.h - Header file of ioTon Config (iotonconfigproject.cpp)
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
#ifndef IOTONCONFIGPROJECT_H
#define IOTONCONFIGPROJECT_H

/* Includes -----------------------------------------------------------------*/
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include "iotonconfigwindow.h"


/** IotonConfigProject *******************************************************/

class IotonConfigProject
{
public:
    IotonConfigProject();

    static void openAtomFolder(const QString path);

    static int createProject(const QString path);

    static int createTemplate(const QString path, IotonConfigWindow::pin pins[], bool examples);

private:
    static const char *mainCppCode;

    static const char *mainHCode;
};

#endif // IOTONCONFIGPROJECT_H
