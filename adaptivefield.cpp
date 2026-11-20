#include "adaptivefield.h"
#include <QDebug>       // Для вывода логов в консоль (qDebug)
#include <QResizeEvent> // Для доступа к event->size() в resizeEvent

// ==============================================================================
//  ЛОГИКА КЛАССА AdaptiveLineEdit
// ==============================================================================

// ---------------------------------------------------------
// 1. Конструктор
// ---------------------------------------------------------
AdaptiveLineEdit::AdaptiveLineEdit(const QString &label, QWidget *parent)
    : QLineEdit(parent)   // Передаём родителя базовому классу QLineEdit
    , m_label(label)      // Сохраняем человекочитаемое имя поля в переменную
{
    // Логируем создание. Это помогает видеть в консоли,
    // в каком порядке создаются наши объекты (связь с ЛР2 - дерево объектов).
    qDebug() << "[CREATE]" << m_label << "(AdaptiveLineEdit)";
}

// ---------------------------------------------------------
// 2. Установка горизонтальной политики
// ---------------------------------------------------------
void AdaptiveLineEdit::setHorizontalPolicy(QSizePolicy::Policy hp)
{
    //  sizePolicy() возвращает копию текущей политики.
    QSizePolicy sp = sizePolicy();

    //  Меняем только горизонтальную ось,
    // оставляя вертикальную такой, какая она была.
    sp.setHorizontalPolicy(hp);
    setSizePolicy(sp);             // Применяем обратно к виджету

    // Выводим в лог, какую политику установили.
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(hp);

    // updateGeometry() — это критически важный вызов.
    // Он сообщает Layout'у: "Я изменился, пересчитай мою геометрию СРАЗУ".
    // Без него новая политика могла бы примениться только после того,
    // как пользователь вручную дернет окно мышкой.
    updateGeometry();
}

// ---------------------------------------------------------
// 3. Установка вертикальной политики
// ---------------------------------------------------------
void AdaptiveLineEdit::setVerticalPolicy(QSizePolicy::Policy vp)
{
    //  Логика аналогична горизонтальной оси.
    // Читаем -> меняем одну ось -> применяем -> просим пересчитать.
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(vp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(vp);
    updateGeometry();
}

// ---------------------------------------------------------
// 4. Переопределенный обработчик изменения размера
// ---------------------------------------------------------
void AdaptiveLineEdit::resizeEvent(QResizeEvent *event)
{
    // ЗАЩИТА: Обязательно вызываем реализацию базового класса (QLineEdit).
    // Внутри QLineEdit есть своя логика (отрисовка рамки, текста),
    // и если мы не передадим ему событие, поле может сломаться.
    QLineEdit::resizeEvent(event);

    // ЗАЩИТА: А вот теперь логируем новые размеры. Именно эти строки
    // в консоли доказывают преподавателю, что наше поле реально
    // меняет размер при растягивании окна.
    qDebug() << "[RESIZE]" << m_label << "->"
             << event->size().width() << "x" << event->size().height();
}

// ==============================================================================
//  ЛОГИКА КЛАССА AdaptiveTextEdit (полностью аналогична AdaptiveLineEdit)
// ==============================================================================

// ---------------------------------------------------------
// 5. Конструктор
// ---------------------------------------------------------
AdaptiveTextEdit::AdaptiveTextEdit(const QString &label, QWidget *parent)
    : QPlainTextEdit(parent) // Передаём родителя базовому классу
    , m_label(label)         // Сохраняем имя
{
    qDebug() << "[CREATE]" << m_label << "(AdaptiveTextEdit)";
}

// ---------------------------------------------------------
// 6. Установка горизонтальной политики
// ---------------------------------------------------------
void AdaptiveTextEdit::setHorizontalPolicy(QSizePolicy::Policy hp)
{
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(hp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(hp);
    updateGeometry(); // Просим Layout пересчитать
}

// ---------------------------------------------------------
// 7. Установка вертикальной политики
// ---------------------------------------------------------
void AdaptiveTextEdit::setVerticalPolicy(QSizePolicy::Policy vp)
{
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(vp);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(vp);
    updateGeometry(); // Просим Layout пересчитать
}

// ---------------------------------------------------------
// 8. Переопределенный обработчик изменения размера
// ---------------------------------------------------------
void AdaptiveTextEdit::resizeEvent(QResizeEvent *event)
{
    // ЗАЩИТА: Сначала базовая логика QPlainTextEdit, затем лог.
    QPlainTextEdit::resizeEvent(event);
    qDebug() << "[RESIZE]" << m_label << "->"
             << event->size().width() << "x" << event->size().height();
}