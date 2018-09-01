#include <QApplication>
#include <QFile>
#include "MusicUI/MusicWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile nFile("./styles/DarkStyle.css");
    if (nFile.open(QFile::ReadOnly))
    {
        QString styleSheetString = nFile.readAll().data();
        a.setStyleSheet(styleSheetString);
    }

    MusciWidget w;
    w.resize(1200, 800);
    w.show();

    return a.exec();
}
