TEMPLATE = subdirs

QT -= gui
QT += core

CONFIG += c++17 console
SUBDIRS =   \
Server/Server.pro \
Client/Client.pro \

Server.file = Server/Server.pro
Client.file = Client/Client.pro
