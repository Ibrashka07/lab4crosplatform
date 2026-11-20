#ifndef ADAPTIVEFIELD_H
#define ADAPTIVEFIELD_H

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSizePolicy>

// Forward-declaration: в заголовке мы ссылаемся только на указатель.
// Полный #include <QResizeEvent> будет в .cpp, где мы читаем event->size().
class QResizeEvent;

// ============================================================
//  AdaptiveLineEdit — однострочное поле с логированием размера.
//
//  Q_OBJECT здесь НЕ НУЖЕН!
//  Мы только переопределяем виртуальный метод resizeEvent().
//  Q_OBJECT нужен только для СИГНАЛОВ и СЛОТОВ. Здесь их нет.
//  Это граница применимости макроса MOC.
// ============================================================
class AdaptiveLineEdit : public QLineEdit
{
public:
    explicit AdaptiveLineEdit(const QString &label, QWidget *parent = nullptr);

    // Удобные сеттеры для смены политики только по одной оси
    void setHorizontalPolicy(QSizePolicy::Policy hp);
    void setVerticalPolicy(QSizePolicy::Policy vp);

    QString label() const { return m_label; }

protected:
    // Переопределение виртуального метода.
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;   // Имя поля для красивых логов в консоли
};

// ============================================================
//  AdaptiveTextEdit — то же самое, но на базе QPlainTextEdit.
//   тоже без Q_OBJECT по той же причине (нет сигналов/слотов).
// ============================================================
class AdaptiveTextEdit : public QPlainTextEdit
{
public:
    explicit AdaptiveTextEdit(const QString &label, QWidget *parent = nullptr);

    void setHorizontalPolicy(QSizePolicy::Policy hp);
    void setVerticalPolicy(QSizePolicy::Policy vp);

    QString label() const { return m_label; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;
};

#endif // ADAPTIVEFIELD_H