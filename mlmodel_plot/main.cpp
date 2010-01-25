#include <QApplication>
#include "window.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    // Widget voor aanmaken dummy-data, werkt nog niet.
    // Widget w;
    // w.show();
    return app.exec();
}
