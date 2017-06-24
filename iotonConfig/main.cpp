#include "iotonconfigwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load and set the stylesheet
    QFile file(":qdarkstyle/style.qss");
    if (file.exists())
    {
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&file);
        app.setStyleSheet(ts.readAll());
        file.close();
    }
    else
    {
        printf("Unable to set stylesheet, file not found\n");
    }

    // Show the main window
    IotonConfigWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
