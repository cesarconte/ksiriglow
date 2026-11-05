/*
    SPDX-FileCopyrightText: 2026 Cesar <yugurta@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "siriglowcontroller.h"

#include <QDBusReply>
#include <QDebug>

SiriGlowController::SiriGlowController(QObject *parent)
    : QObject(parent)
    , m_testTimer(new QTimer(this))
    , m_kityCheckTimer(new QTimer(this))
{
    m_testTimer->setSingleShot(true);
    connect(m_testTimer, &QTimer::timeout, this, [this]() {
        setState(QStringLiteral("idle"));
        setAudioLevel(0.0);
    });

    setupKityWatcher();

    // Check periodically for Kity if not registered yet
    m_kityCheckTimer->setInterval(3000);
    connect(m_kityCheckTimer, &QTimer::timeout, this, &SiriGlowController::checkKityConnection);
    m_kityCheckTimer->start();
}

SiriGlowController::~SiriGlowController()
{
    delete m_kityIface;
}

void SiriGlowController::setupKityWatcher()
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    // Connect to PropertiesChanged of org.cesar.kity
    bus.connect(QStringLiteral("org.cesar.kity"),
                QStringLiteral("/Kity"),
                QStringLiteral("org.freedesktop.DBus.Properties"),
                QStringLiteral("PropertiesChanged"),
                this,
                SLOT(onKityDbusPropertiesChanged(QString,QVariantMap,QStringList)));

    // Connect directly to stateChanged and audioLevelChanged if exported
    bus.connect(QStringLiteral("org.cesar.kity"),
                QStringLiteral("/Kity"),
                QStringLiteral("org.kde.plasmashell.KityApplet"),
                QStringLiteral("stateChanged"),
                this,
                SLOT(checkKityConnection()));

    bus.connect(QStringLiteral("org.cesar.kity"),
                QStringLiteral("/Kity"),
                QStringLiteral("org.kde.plasmashell.KityApplet"),
                QStringLiteral("audioLevelChanged"),
                this,
                SLOT(checkKityConnection()));

    checkKityConnection();
}

void SiriGlowController::checkKityConnection()
{
    if (!m_kityIface || !m_kityIface->isValid()) {
        delete m_kityIface;
        m_kityIface = new QDBusInterface(QStringLiteral("org.cesar.kity"),
                                        QStringLiteral("/Kity"),
                                        QStringLiteral("org.kde.plasmashell.KityApplet"),
                                        QDBusConnection::sessionBus(),
                                        this);
    }

    if (m_kityIface && m_kityIface->isValid()) {
        const QString kityState = m_kityIface->property("state").toString();
        if (!kityState.isEmpty() && !m_testTimer->isActive()) {
            setState(kityState);
        }

        const QVariant levelVar = m_kityIface->property("audioLevel");
        if (levelVar.isValid() && !m_testTimer->isActive()) {
            setAudioLevel(levelVar.toReal());
        }
    }
}

void SiriGlowController::onKityDbusPropertiesChanged(const QString &interface,
                                                    const QVariantMap &changedProperties,
                                                    const QStringList &invalidatedProperties)
{
    Q_UNUSED(interface)
    Q_UNUSED(invalidatedProperties)

    if (m_testTimer->isActive()) {
        return; // Don't override test mode
    }

    if (changedProperties.contains(QStringLiteral("state"))) {
        const QString newState = changedProperties.value(QStringLiteral("state")).toString();
        setState(newState);
    }

    if (changedProperties.contains(QStringLiteral("audioLevel"))) {
        const qreal newLevel = changedProperties.value(QStringLiteral("audioLevel")).toReal();
        setAudioLevel(newLevel);
    }
}

void SiriGlowController::setState(const QString &state)
{
    if (m_state != state) {
        m_state = state;
        Q_EMIT stateChanged();
        updateGlowActive();
    }
}

void SiriGlowController::setAudioLevel(qreal level)
{
    level = qBound<qreal>(0.0, level, 1.0);
    if (!qFuzzyCompare(m_audioLevel, level)) {
        m_audioLevel = level;
        Q_EMIT audioLevelChanged();
    }
}

void SiriGlowController::setGlowEnabled(bool enabled)
{
    if (m_glowEnabled != enabled) {
        m_glowEnabled = enabled;
        Q_EMIT glowEnabledChanged();
        updateGlowActive();
    }
}

void SiriGlowController::updateGlowActive()
{
    const bool shouldBeActive = m_glowEnabled &&
                                (m_state == QLatin1String("listening") ||
                                 m_state == QLatin1String("thinking") ||
                                 m_state == QLatin1String("transcribing") ||
                                 m_state == QLatin1String("speaking"));

    if (m_glowActive != shouldBeActive) {
        m_glowActive = shouldBeActive;
        Q_EMIT glowActiveChanged();
    }
}

void SiriGlowController::triggerTest(const QString &state, int durationMs)
{
    setState(state);
    setAudioLevel(0.85);
    m_testTimer->start(durationMs);
}

void SiriGlowController::Show()
{
    setGlowEnabled(true);
    triggerTest(QStringLiteral("listening"), 5000);
}

void SiriGlowController::Hide()
{
    m_testTimer->stop();
    setState(QStringLiteral("idle"));
    setAudioLevel(0.0);
}

void SiriGlowController::Toggle()
{
    if (m_glowActive) {
        Hide();
    } else {
        Show();
    }
}

void SiriGlowController::SetState(const QString &state)
{
    setState(state);
}

void SiriGlowController::SetAudioLevel(double level)
{
    setAudioLevel(level);
}

void SiriGlowController::TriggerTest(const QString &state, int durationMs)
{
    triggerTest(state, durationMs);
}
