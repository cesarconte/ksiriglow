/*
    SPDX-FileCopyrightText: 2026 Cesar <yugurta@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "siriglowcontroller.h"

#include <KLocalizedString>

#include <QCommandLineParser>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("ksiriglow");
    app.setApplicationName(QStringLiteral("ksiriglow"));
    app.setApplicationDisplayName(QStringLiteral("Siri Screen Glow"));
    app.setOrganizationDomain(QStringLiteral("cesar.org"));
    app.setQuitOnLastWindowClosed(false);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kity-symbolic"), QIcon::fromTheme(QStringLiteral("preferences-desktop-display"))));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Apple Intelligence Siri Screen Glow Overlay para KDE Plasma 6 Wayland"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption daemonOpt(QStringList{QStringLiteral("d"), QStringLiteral("daemon")},
                                 QStringLiteral("Iniciar en segundo plano escuchando a Kiri"));
    parser.addOption(daemonOpt);

    QCommandLineOption testOpt(QStringList{QStringLiteral("t"), QStringLiteral("test")},
                               QStringLiteral("Probar visualmente el efecto perimetral con un estado"),
                               QStringLiteral("estado"),
                               QStringLiteral("listening"));
    parser.addOption(testOpt);

    QCommandLineOption toggleOpt(QStringList{QStringLiteral("g"), QStringLiteral("toggle")},
                                 QStringLiteral("Alternar visualización del halo"));
    parser.addOption(toggleOpt);

    parser.process(app);

    const QString serviceName = QStringLiteral("org.cesar.ksiriglow");
    const QString objectPath = QStringLiteral("/Glow");
    const QString interfaceName = QStringLiteral("org.cesar.ksiriglow");

    QDBusConnection bus = QDBusConnection::sessionBus();
    if (bus.isConnected() && !bus.registerService(serviceName)) {
        // Otra instancia en ejecución: reenviar petición
        QDBusInterface iface(serviceName, objectPath, interfaceName, bus);
        if (iface.isValid()) {
            if (parser.isSet(testOpt)) {
                iface.call(QStringLiteral("TriggerTest"), parser.value(testOpt), 5000);
            } else {
                iface.call(QStringLiteral("Toggle"));
            }
            return 0;
        }
    }

    SiriGlowController controller;

    if (bus.isConnected()) {
        bus.registerObject(objectPath, &controller, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties);
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("Controller"), &controller);
    engine.load(QUrl(QStringLiteral("qrc:/org/cesar/ksiriglow/qml/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    if (parser.isSet(testOpt)) {
        controller.triggerTest(parser.value(testOpt), 5000);
    } else if (parser.isSet(toggleOpt)) {
        controller.Toggle();
    }

    return app.exec();
}
