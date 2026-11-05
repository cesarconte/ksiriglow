/*
    SPDX-FileCopyrightText: 2026 Cesar <yugurta@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "siriglowcontroller.h"

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QSignalSpy>
#include <QtTest>

class TestSiriGlow : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInitialState()
    {
        SiriGlowController ctrl;
        QCOMPARE(ctrl.state(), QStringLiteral("idle"));
        QCOMPARE(ctrl.audioLevel(), 0.0);
        QCOMPARE(ctrl.glowActive(), false);
        QCOMPARE(ctrl.isGlowEnabled(), true);
    }

    void testStateTransitionsAndGlowActive()
    {
        SiriGlowController ctrl;
        QSignalSpy stateSpy(&ctrl, &SiriGlowController::stateChanged);
        QSignalSpy glowSpy(&ctrl, &SiriGlowController::glowActiveChanged);

        ctrl.setState(QStringLiteral("listening"));
        QCOMPARE(stateSpy.count(), 1);
        QCOMPARE(ctrl.state(), QStringLiteral("listening"));
        QCOMPARE(ctrl.glowActive(), true);
        QCOMPARE(glowSpy.count(), 1);

        ctrl.setState(QStringLiteral("thinking"));
        QCOMPARE(ctrl.glowActive(), true);

        ctrl.setState(QStringLiteral("speaking"));
        QCOMPARE(ctrl.glowActive(), true);

        ctrl.setState(QStringLiteral("idle"));
        QCOMPARE(ctrl.glowActive(), false);
        QCOMPARE(glowSpy.count(), 2);
    }

    void testAudioLevelBounds()
    {
        SiriGlowController ctrl;
        QSignalSpy audioSpy(&ctrl, &SiriGlowController::audioLevelChanged);

        ctrl.setAudioLevel(0.75);
        QCOMPARE(audioSpy.count(), 1);
        QCOMPARE(ctrl.audioLevel(), 0.75);

        // Clamping to [0, 1]
        ctrl.setAudioLevel(1.5);
        QCOMPARE(ctrl.audioLevel(), 1.0);

        ctrl.setAudioLevel(-0.5);
        QCOMPARE(ctrl.audioLevel(), 0.0);
    }

    void testGlowEnabledToggle()
    {
        SiriGlowController ctrl;
        ctrl.setState(QStringLiteral("listening"));
        QCOMPARE(ctrl.glowActive(), true);

        ctrl.setGlowEnabled(false);
        QCOMPARE(ctrl.glowActive(), false);

        ctrl.setGlowEnabled(true);
        QCOMPARE(ctrl.glowActive(), true);
    }

    void testQmlLoadsWithoutErrors()
    {
        SiriGlowController ctrl;
        QQmlEngine engine;
        engine.rootContext()->setContextProperty(QStringLiteral("Controller"), &ctrl);

        QQmlComponent comp(&engine, QUrl(QStringLiteral("qrc:/org/cesar/ksiriglow/qml/Main.qml")));
        QVERIFY2(!comp.isError(), qPrintable(comp.errorString()));
    }
};

QTEST_MAIN(TestSiriGlow)
#include "test_siriglow.moc"
