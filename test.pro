TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    BattleEngine.cpp \
    Card.cpp \
    GameLog.cpp \
    GameManager.cpp \
    GameThread.cpp \
    Interface.cpp \
    LogThread.cpp \
    Player.cpp \
    button.cpp \
    context.cpp \
    enemyai.cpp \
    gameboard.cpp

win32 {
    INCLUDEPATH += "C:/SFML-2.5.1/include"
    LIBS += -L"C:/SFML-2.5.1/lib"
    CONFIG(debug, debug|release) {
        LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
    } else {
        LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
    }
} else {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
    QMAKE_LFLAGS += -Wl,-rpath,/usr/local/lib
}
DISTFILES += resources/karta.png resources/placeholder.png resources/phSlot.png resources/phCard.png resources/papyrus.ttf \
    dzikiekarty.qmodel \
    resources/Battleground.png \
    resources/Button.png \
    resources/Button_pressed.png \
    resources/Deck.png \
    resources/Waga.png \
    resources/background-2.png \
    resources/background-3.png \
    resources/background-4.png \
    resources/background-5.png \
    resources/background-6.png \
    resources/background-7.png \
    resources/background-8.png \
    resources/background.png \
    resources/chat.png \
    resources/nietoperz.png \
    resources/raven.png \
    resources/waz.png \
    resources/wiewiorka.png \
    resources/wilk.png

QMAKE_POST_LINK += mkdir -p $$OUT_PWD/resources || true
QMAKE_POST_LINK += cp -r $$PWD/resources/* $$OUT_PWD/resources || true
RESOURCES += resources.qrc

HEADERS += \
    BattleEngine.h \
    Card.h \
    Deck.h \
    GameLog.h \
    GameManager.h \
    Interactive.h \
    LogThread.h \
    Player.h \
    Rendering.h \
    TemplateDeck.h \
    button.h \
    context.h \
    enemyai.h \
    gameboard.h

