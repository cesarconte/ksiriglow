/*
    SPDX-FileCopyrightText: 2026 Cesar <yugurta@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
#include <QObject>
#include <QString>
#include <QTimer>

class SiriGlowController : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.cesar.ksiriglow")

    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(qreal audioLevel READ audioLevel WRITE setAudioLevel NOTIFY audioLevelChanged)
    Q_PROPERTY(bool glowActive READ glowActive NOTIFY glowActiveChanged)
    Q_PROPERTY(bool glowEnabled READ isGlowEnabled WRITE setGlowEnabled NOTIFY glowEnabledChanged)

public:
    explicit SiriGlowController(QObject *parent = nullptr);
    ~SiriGlowController() override;

    QString state() const { return m_state; }
    void setState(const QString &state);

    qreal audioLevel() const { return m_audioLevel; }
    void setAudioLevel(qreal level);

    bool glowActive() const { return m_glowActive; }
    bool isGlowEnabled() const { return m_glowEnabled; }
    void setGlowEnabled(bool enabled);

    Q_INVOKABLE void triggerTest(const QString &state = QStringLiteral("listening"), int durationMs = 4000);

public Q_SLOTS:
    Q_SCRIPTABLE void Show();
    Q_SCRIPTABLE void Hide();
    Q_SCRIPTABLE void Toggle();
    Q_SCRIPTABLE void SetState(const QString &state);
    Q_SCRIPTABLE void SetAudioLevel(double level);
    Q_SCRIPTABLE void TriggerTest(const QString &state, int durationMs);

Q_SIGNALS:
    void stateChanged();
    void audioLevelChanged();
    void glowActiveChanged();
    void glowEnabledChanged();

private Q_SLOTS:
    void onKityDbusPropertiesChanged(const QString &interface,
                                    const QVariantMap &changedProperties,
                                    const QStringList &invalidatedProperties);
    void checkKityConnection();

private:
    void setupKityWatcher();
    void updateGlowActive();

    QString m_state = QStringLiteral("idle");
    qreal m_audioLevel = 0.0;
    bool m_glowActive = false;
    bool m_glowEnabled = true;

    QTimer *m_testTimer = nullptr;
    QTimer *m_kityCheckTimer = nullptr;
    QDBusInterface *m_kityIface = nullptr;
};
