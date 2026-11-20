#ifndef FORMWINDOW_H
#define FORMWINDOW_H

#include <QWidget>
#include "adaptivefield.h"

// Forward-declarations: используются только как указатели.
class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QCheckBox;
class QResizeEvent;

class FormWindow : public QWidget
{
    Q_OBJECT
    //  В отличие от adaptivefield.h, здесь Q_OBJECT ОБЯЗАТЕЛЕН,
    // так как есть слоты (private slots) и используется connect().

public:
    explicit FormWindow(QWidget *parent = nullptr);
    ~FormWindow() override;

protected:
    // Переопределяем, чтобы логировать изменение размера всего окна
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void setCompactMode();            // Слот: уменьшить отступы (margins/spacing)
    void setComfortableMode();        // Слот: вернуть нормальные отступы
    void toggleDescriptionPolicy();   // Слот: тумблер для поля "Описание"
    void toggleAgePolicy();           // Слот: тумблер для поля "Возраст"
    void dumpSizePolicies();          // Слот: вывести политики в консоль
    void dumpSizes();                 // Слот: вывести текущие размеры в консоль
    void onAccept();                  // Слот: нажатие ОК
    void onReject();                  // Слот: нажатие Отмена

private:
    void setupUi();                   // Метод сборки интерфейса

    // Вспомогательный метод для перевода enum в строку
    static QString policyName(QSizePolicy::Policy p);

    // Наши поля (объекты, живут на стеке класса)
    AdaptiveLineEdit m_name;
    AdaptiveLineEdit m_email;
    AdaptiveLineEdit m_age;
    QComboBox       *m_country;
    AdaptiveTextEdit m_description;
    QCheckBox       *m_agree;

    // Layouts
    QVBoxLayout *m_rootLayout;        // Основной вертикальный
    QFormLayout *m_formLayout;        // Форма "Метка: Поле"

    // Указатель на обертку для поля "Возраст" (чтобы менять пружину)
    QHBoxLayout *m_ageRowLayout;

    QList<QWidget*> m_trackedFields;  // Список полей для "дампа"

    // Флаги текущего состояния тумблеров
    bool m_descriptionExpanding;
    bool m_ageExpanding;
};

#endif // FORMWINDOW_H