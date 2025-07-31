#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //防止点击多次exe，出现多个界面
    qInstallMessageHandler(MessWriteLog);
    QSharedMemory shared("Yanding");
    if(shared.attach())
    {
        return 0;
    }
    shared.create(1);



    MainWindow w;
    QFile styleSheetFile(":/style/resources/style.qss");
    if (styleSheetFile.open(QFile::ReadOnly)){
       QString styleSheet = styleSheetFile.readAll();
       styleSheetFile.close();
       qApp->setStyleSheet(styleSheet);
    }
    w.show();
    //w.showMaximized();
    return a.exec();
}
