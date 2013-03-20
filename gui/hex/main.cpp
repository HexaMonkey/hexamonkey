#include <QApplication>
#include "hexfilewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HexFileWidget window;
    window.setFile("/home/spam/Etude_de_cas_2012.pdf");
    window.show();
    return app.exec();
}
