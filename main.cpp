#include <QApplication>
#include "formwindow.h"

int main(int argc, char *argv[])
{
    // QApplication создает цикл событий (event loop).
    QApplication app(argc, argv);

    FormWindow w;
    w.show();

    // ЗАЩИТА: app.exec() запускает цикл событий.
    // Без него не работают ни resizeEvent (изменение размера),
    // ни клики мышью, ни перерисовка. Вся адаптивность интерфейса
    // живет именно в этом цикле.
    return app.exec();
}