# GUI-модуль Qt (виджеты). Обязателен для окон.
QT += widgets

# Современный C++17 (для удобной работы с контейнерами и лямбдами).
CONFIG += c++17

# macOS: не создавать .app-bundle — проще запускать из Qt Creator.
CONFIG -= app_bundle

# На macOS НЕ добавляем "console":
# — на Windows она открывает консоль, на macOS ломает показ окна.
# — qDebug() всё равно виден в панели "Вывод приложения" внутри Qt Creator.

TARGET = AdaptiveForm
TEMPLATE = app

# Все .cpp. Если забыть один — линкер выдаст undefined reference.
SOURCES += \
    main.cpp \
    adaptivefield.cpp \
    formwindow.cpp

# Все заголовки. Каждый — ровно один раз (дубль = мусор в репо).
HEADERS += \
    adaptivefield.h \
    formwindow.h

# Секции FORMS нет — интерфейс строится кодом (как в ЛР2 и ЛР3).