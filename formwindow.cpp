#include "formwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <utility> // Для std::as_const

// ==============================================================================
//  КОНСТРУКТОР И ДЕСТРУКТОР
// ==============================================================================
FormWindow::FormWindow(QWidget *parent)
    : QWidget(parent)
    //  Поля-объекты (AdaptiveLineEdit, AdaptiveTextEdit) инициализируем
    // в списке инициализации, так как они лежат на стеке класса (не указатели).
    // Передаем им строку, которая будет писаться в логи при ресайзе.
    , m_name("Имя")
    , m_email("Email")
    , m_age("Возраст")
    , m_country(nullptr)       // Указатели создадим в setupUi() через new
    , m_description("Описание")
    , m_agree(nullptr)
    , m_rootLayout(nullptr)
    , m_formLayout(nullptr)
    , m_ageRowLayout(nullptr)
    , m_descriptionExpanding(true) // Стартовое состояние для тумблера описания
    , m_ageExpanding(false)        // Стартовое состояние для тумблера возраста
{
    setupUi();             // Собираем интерфейс кодом
    setComfortableMode();  // Задаем стартовые отступы (margins/spacing)
    qDebug() << "[INIT] Окно создано. Попробуйте растянуть его!";
}

FormWindow::~FormWindow()
{
    //  Логируем удаление, чтобы видеть жизненный цикл окна (связь с ЛР2).
    qDebug() << "[DESTROY] FormWindow";
}

// ==============================================================================
//  ПЕРЕОПРЕДЕЛЕНИЕ СОБЫТИЯ ИЗМЕНЕНИЯ РАЗМЕРА ОКНА
// ==============================================================================
void FormWindow::resizeEvent(QResizeEvent *event)
{
    //  Сначала вызываем базовый класс, чтобы окно корректно отрисовалось.
    QWidget::resizeEvent(event);

    //  Логируем размеры, чтобы доказать, что окно реально меняет размер.
    // Именно эти строки нужно показать преподавателю в консоли.
    qDebug() << "[RESIZE] FormWindow ->"
             << event->size().width() << "x" << event->size().height();
}

// ==============================================================================
//  СБОРКА ИНТЕРФЕЙСА
// ==============================================================================
void FormWindow::setupUi()
{
    setWindowTitle("Адаптивная форма — ЛР4");
    resize(560, 620); // Стартовый размер окна

    // --------------------------------------------------------------------------
    // 1. КОРНЕВОЙ LAYOUT (вертикальный)
    // --------------------------------------------------------------------------
    m_rootLayout = new QVBoxLayout(this);

    auto *title = new QLabel("<b>Анкета</b>", this);
    m_rootLayout->addWidget(title);

    // --------------------------------------------------------------------------
    // 2. ФОРМА (QFormLayout)
    // --------------------------------------------------------------------------
    m_formLayout = new QFormLayout();

    //  Обнуляем отступы (margins) у формы. Если этого не сделать,
    // у QFormLayout будут свои отступы, у QVBoxLayout свои. Они сложатся,
    // и получится двойной отступ от краев окна. Обнуляем форму,
    // чтобы отступами управлял только корневой layout.
    m_formLayout->setContentsMargins(0, 0, 0, 0);

    //  QFormLayout по умолчанию в стиле macOS
    // не дает полям растягиваться, а "душит" их по sizeHint.
    // Эта политика заставляет QFormLayout уважать политики полей (Expanding).
    m_formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    //  Передаем stretch=1 в addLayout. Это говорит корневому layout'у,
    // что форма должна забирать все свободное вертикальное место,
    // и колонка с полями может расширяться при растягивании окна.
    m_rootLayout->addLayout(m_formLayout, 1);

    // --------------------------------------------------------------------------
    // 3. ПОЛЕ "ИМЯ"
    // --------------------------------------------------------------------------
    // ЗАЩИТА: Expanding по горизонтали (тянется вширь), Fixed по вертикали
    // (высота фиксирована, ввысь не растет).
    m_name.setHorizontalPolicy(QSizePolicy::Expanding);
    m_name.setVerticalPolicy(QSizePolicy::Fixed);
    m_formLayout->addRow("Имя:", &m_name);

    // --------------------------------------------------------------------------
    // 4. ПОЛЕ "EMAIL" (аналогично имени)
    // --------------------------------------------------------------------------
    m_email.setHorizontalPolicy(QSizePolicy::Expanding);
    m_email.setVerticalPolicy(QSizePolicy::Fixed);
    m_formLayout->addRow("Email:", &m_email);

    // --------------------------------------------------------------------------
    // 5. ПОЛЕ "ВОЗРАСТ" (Обертка с пружиной)
    // --------------------------------------------------------------------------
    //  Задаем минимальную ширину, чтобы при сжатии пружины поле
    // не превращалось в точку (невидимое).
    m_age.setMinimumWidth(60);

    //  По умолчанию Fixed (не растягивается).
    m_age.setHorizontalPolicy(QSizePolicy::Fixed);
    m_age.setVerticalPolicy(QSizePolicy::Fixed);

    // Валидатор разрешает вводить только цифры от 0 до 130.
    m_age.setValidator(new QIntValidator(0, 130, &m_age));

    // Создаем горизонтальный слой (обертку) для возраста.
    // Это нужно, чтобы QFormLayout не растянул поле на всю ширину,
    // а оставил его маленьким.
    m_ageRowLayout = new QHBoxLayout();
    m_ageRowLayout->setContentsMargins(0, 0, 0, 0);
    m_ageRowLayout->addWidget(&m_age);

    //  addStretch() — это пружина. Она забирает всё свободное место.
    m_ageRowLayout->addStretch();

    //  Управляем весами (stretch-факторами).
    // Поле весит 0 (не растягивается), пружина весит 1 (забирает всё место).
    m_ageRowLayout->setStretch(0, 0);
    m_ageRowLayout->setStretch(1, 1);

    // Добавляем обертку как строку в форму.
    m_formLayout->addRow("Возраст:", m_ageRowLayout);

    // --------------------------------------------------------------------------
    // 6. ПОЛЕ "СТРАНА"
    // --------------------------------------------------------------------------
    m_country = new QComboBox(this);
    m_country->addItems({"Россия", "Беларусь", "Казахстан", "Другая"});
    // Страна тянется вширь, но не ввысь.
    m_country->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_formLayout->addRow("Страна:", m_country);

    // --------------------------------------------------------------------------
    // 7. ПОЛЕ "ОПИСАНИЕ" (Главный адаптивный элемент)
    // --------------------------------------------------------------------------
    //  Это поле забирает всё свободное место И по горизонтали, И по
    // вертикали. Именно благодаря ему окно адаптируется по высоте.
    m_description.setHorizontalPolicy(QSizePolicy::Expanding);
    m_description.setVerticalPolicy(QSizePolicy::Expanding);
    m_formLayout->addRow("Описание:", &m_description);

    // --------------------------------------------------------------------------
    // 8. ЧЕКБОКС "СОГЛАСИЕ"
    // --------------------------------------------------------------------------
    m_agree = new QCheckBox("Я согласен с условиями", this);
    m_agree->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // Фиксирован
    // Пустая строка в качестве метки, чтобы чекбокс сдвинулся вправо.
    m_formLayout->addRow("", m_agree);

    // --------------------------------------------------------------------------
    // 9. СПИСОК ПОЛЕЙ ДЛЯ ДАМПА
    // --------------------------------------------------------------------------
    // Сохраняем указатели на все поля, чтобы потом в цикле вывести их
    // политики и размеры в консоль.
    m_trackedFields = { &m_name, &m_email, &m_age, m_country, &m_description, m_agree };

    // --------------------------------------------------------------------------
    // 10. ПАНЕЛЬ КНОПОК УПРАВЛЕНИЯ
    // --------------------------------------------------------------------------
    auto *modeRow = new QHBoxLayout();
    modeRow->setContentsMargins(0, 0, 0, 0);

    auto *compactBtn = new QPushButton("Компактный", this);
    auto *comfortBtn = new QPushButton("Удобный", this);
    auto *toggleDescBtn = new QPushButton("Описание: Expanding/Fixed", this);
    auto *toggleAgeBtn = new QPushButton("Возраст: Fixed/Expanding", this);
    auto *dumpPolBtn = new QPushButton("Дамп политик", this);
    auto *dumpSizeBtn = new QPushButton("Дамп размеров", this);

    modeRow->addWidget(compactBtn);
    modeRow->addWidget(comfortBtn);
    modeRow->addWidget(toggleDescBtn);
    modeRow->addWidget(toggleAgeBtn);

    //  Пружина в середине. Прижимает левую группу кнопок влево,
    // а правую группу — вправо. Это наглядная работа addStretch().
    modeRow->addStretch();

    modeRow->addWidget(dumpPolBtn);
    modeRow->addWidget(dumpSizeBtn);
    m_rootLayout->addLayout(modeRow);

    // --------------------------------------------------------------------------
    // 11. НИЖНИЙ РЯД: КНОПКИ OK И ОТМЕНА
    // --------------------------------------------------------------------------
    auto *btnRow = new QHBoxLayout();
    auto *okBtn = new QPushButton("OK", this);
    auto *cancelBtn = new QPushButton("Отмена", this);

    //  Пружина СЛЕВА. Она выталкивает кнопки в правый край окна.
    // Без неё кнопки прилипли бы к левому краю.
    btnRow->addStretch();
    btnRow->addWidget(okBtn);
    btnRow->addWidget(cancelBtn);
    m_rootLayout->addLayout(btnRow);

    // --------------------------------------------------------------------------
    // 12. ПОДКЛЮЧЕНИЕ СИГНАЛОВ К СЛОТАМ
    // --------------------------------------------------------------------------
    //  Сигнал clicked() кнопки соединяем с методом-слотом класса FormWindow.
    connect(compactBtn, &QPushButton::clicked, this, &FormWindow::setCompactMode);
    connect(comfortBtn, &QPushButton::clicked, this, &FormWindow::setComfortableMode);
    connect(toggleDescBtn, &QPushButton::clicked, this, &FormWindow::toggleDescriptionPolicy);
    connect(toggleAgeBtn, &QPushButton::clicked, this, &FormWindow::toggleAgePolicy);
    connect(dumpPolBtn, &QPushButton::clicked, this, &FormWindow::dumpSizePolicies);
    connect(dumpSizeBtn, &QPushButton::clicked, this, &FormWindow::dumpSizes);
    connect(okBtn, &QPushButton::clicked, this, &FormWindow::onAccept);
    connect(cancelBtn, &QPushButton::clicked, this, &FormWindow::onReject);
}

// ==============================================================================
//  СЛОТЫ УПРАВЛЕНИЯ ПЛОТНОСТЬЮ (ОТСТУПЫ И ЗАЗОРЫ)
// ==============================================================================
void FormWindow::setCompactMode()
{
    //  setContentsMargins задает отступы от краев контейнера,
    // setSpacing — зазор между элементами.
    // В компактном режиме уменьшаем и то, и другое.
    m_rootLayout->setContentsMargins(4, 4, 4, 4);
    m_rootLayout->setSpacing(4);
    m_formLayout->setSpacing(4);

    // Принудительно пересчитываем раскладку.
    m_rootLayout->invalidate();
}

void FormWindow::setComfortableMode()
{
    //  Удобный режим — увеличиваем отступы.
    m_rootLayout->setContentsMargins(16, 16, 16, 16);
    m_rootLayout->setSpacing(10);
    m_formLayout->setSpacing(8);
    m_rootLayout->invalidate();
}

// ==============================================================================
//  СЛОТЫ-ТУМБЛЕРЫ (ИЗМЕНЕНИЕ ПОЛИТИК НА ЛЕТУ)
// ==============================================================================
void FormWindow::toggleDescriptionPolicy()
{
    // Переключаем флаг состояния.
    m_descriptionExpanding = !m_descriptionExpanding;

    // В зависимости от флага меняем вертикальную политику.
    if (m_descriptionExpanding) {
        m_description.setVerticalPolicy(QSizePolicy::Expanding);
    } else {
        m_description.setVerticalPolicy(QSizePolicy::Fixed);
    }

    // Просим layout пересчитать размеры.
    m_rootLayout->invalidate();
}

void FormWindow::toggleAgePolicy()
{
    m_ageExpanding = !m_ageExpanding;

    if (m_ageExpanding) {
        //  Если включаем Expanding, поле должно забрать всё место.
        m_age.setHorizontalPolicy(QSizePolicy::Expanding);
        // Поле весит 1 (растет), пружина весит 0 (сжимается).
        m_ageRowLayout->setStretch(0, 1);
        m_ageRowLayout->setStretch(1, 0);
    } else {
        //  Если Fixed, поле маленькое, а место забирает пружина.
        m_age.setHorizontalPolicy(QSizePolicy::Fixed);
        // Поле весит 0, пружина весит 1.
        m_ageRowLayout->setStretch(0, 0);
        m_ageRowLayout->setStretch(1, 1);
    }

    //  Эти три вызова заставляют интерфейс перестроиться ПРЯМО СЕЙЧАС,
    // а не ждать следующего изменения размера окна.
    m_age.updateGeometry();
    m_formLayout->invalidate();
    m_rootLayout->invalidate();
}

// ==============================================================================
//  ДАМПЫ (ВЫВОД ИНФОРМАЦИИ В КОНСОЛЬ ДЛЯ ОТЛАДКИ)
// ==============================================================================

//  Вспомогательный метод. Enum (QSizePolicy::Policy) нельзя
// напрямую вывести в консоль как строку, поэтому используем switch.
QString FormWindow::policyName(QSizePolicy::Policy p)
{
    switch (p) {
    case QSizePolicy::Fixed:            return "Fixed";
    case QSizePolicy::Minimum:          return "Minimum";
    case QSizePolicy::Maximum:          return "Maximum";
    case QSizePolicy::Preferred:        return "Preferred";
    case QSizePolicy::Expanding:        return "Expanding";
    case QSizePolicy::MinimumExpanding: return "MinimumExpanding";
    case QSizePolicy::Ignored:          return "Ignored";
    default:                            return "?";
    }
}

void FormWindow::dumpSizePolicies()
{
    qDebug() << "===== DUMP SIZE POLICIES =====";

    // Итерируемся по списку полей.
    // Используем std::as_const, чтобы Qt не делал глубокую копию (detach) QList.
    for (const QWidget *w : std::as_const(m_trackedFields)) {
        if (!w) continue;

        const QSizePolicy sp = w->sizePolicy();
        qDebug() << w->metaObject()->className()
                 << " H:" << policyName(sp.horizontalPolicy())
                 << " V:" << policyName(sp.verticalPolicy());
    }
    qDebug() << "==============================";
}

void FormWindow::dumpSizes()
{
    qDebug() << "===== DUMP SIZES =====";
    for (const QWidget *w : std::as_const(m_trackedFields)) {
        if (!w) continue;

        //  Выводим текущий размер, желаемый (sizeHint) и минимальный.
        qDebug() << w->metaObject()->className()
                 << " size:" << w->width() << "x" << w->height()
                 << " hint:" << w->sizeHint()
                 << " min:" << w->minimumSizeHint();
    }
    qDebug() << "======================";
}

// ==============================================================================
//  СЛОТЫ КНОПОК OK И ОТМЕНА
// ==============================================================================
void FormWindow::onAccept()
{
    //  В реальном приложении здесь было бы сохранение в файл.
    // В рамках ЛР4 мы просто выводим введенные данные в консоль.
    qDebug() << "[ACCEPT] Имя:" << m_name.text() << " Email:" << m_email.text();
}

void FormWindow::onReject()
{
    // Закрываем окно, завершая приложение.
    close();
}