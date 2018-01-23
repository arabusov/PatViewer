QT += widgets
qtHaveModule(printsupport): QT += printsupport
QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
HEADERS       = gui/include/patternviewer.hpp Pattern/include/Pattern.hpp Pattern/include/DEPFET.hpp
SOURCES       = gui/src/patternviewer.cpp Pattern/src/*\
                main.cpp
# install
target.path = ./build
INSTALLS += target
