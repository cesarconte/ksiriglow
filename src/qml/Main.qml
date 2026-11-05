/*
    SPDX-FileCopyrightText: 2026 Cesar <yugurta@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick
import org.kde.layershell as LayerShell

Window {
    id: glowRoot
    objectName: "siriGlowOverlayWindow"

    flags: Qt.FramelessWindowHint | Qt.WindowDoesNotAcceptFocus | Qt.WindowTransparentForInput
    color: "transparent"

    LayerShell.Window.anchors: LayerShell.Window.AnchorTop | LayerShell.Window.AnchorBottom | LayerShell.Window.AnchorLeft | LayerShell.Window.AnchorRight
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
    LayerShell.Window.keyboardInteractivity: LayerShell.Window.KeyboardInteractivityNone
    LayerShell.Window.exclusionZone: 0

    readonly property bool isGlowActive: Controller ? Controller.glowActive : false
    readonly property real audioAmp: Controller ? Controller.audioLevel : 0.0
    readonly property string glowState: Controller ? Controller.state : "idle"

    visible: isGlowActive || fadeAnim.running

    property real animTime: 0.0
    NumberAnimation on animTime {
        from: 0.0
        to: 1000.0
        duration: 1000000
        loops: Animation.Infinite
        running: glowRoot.visible
    }

    Item {
        id: contentContainer
        anchors.fill: parent
        opacity: glowRoot.isGlowActive ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                id: fadeAnim
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }

        ShaderEffect {
            id: shaderEffect
            anchors.fill: parent
            property real time: glowRoot.animTime
            property real audioAmp: glowRoot.audioAmp
            property size resolution: Qt.size(width, height)
            property real isSpeaking: glowRoot.glowState === "speaking" ? 1.0 : 0.0
            fragmentShader: "qrc:/org/cesar/ksiriglow/shaders/glow.frag.qsb"
        }
    }
}
