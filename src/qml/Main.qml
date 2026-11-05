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

    visible: true
    opacity: isGlowActive ? 1.0 : 0.0

    Behavior on opacity {
        NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
    }

    // Traveling wave phase for Apple Intelligence thinking state
    property real wavePhase: 0.0
    NumberAnimation on wavePhase {
        from: 0.0
        to: 1.0
        duration: 3200
        loops: Animation.Infinite
        running: glowRoot.isGlowActive
    }

    // Dynamic glow depth pulsating with audio level
    readonly property real glowThickness: 8 + 18 * audioAmp
    readonly property real glowSpread: 16 + 28 * audioAmp

    Item {
        anchors.fill: parent

        // Outer Glow Perimeter Frame (Layer 1: Deep atmospheric ambient halo)
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 20
            border.width: glowRoot.glowThickness + 8
            border.color: glowRoot.glowState === "speaking" ? "#00f5d4" : "#ff007f"
            opacity: 0.25 + 0.35 * glowRoot.audioAmp

            Behavior on border.width {
                NumberAnimation { duration: 80 }
            }
            Behavior on opacity {
                NumberAnimation { duration: 80 }
            }
        }

        // Layer 2: Vibrantly animated Apple Intelligence gradient edges
        // Top Edge Glow
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: glowRoot.glowSpread
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#00f0ff" }
                GradientStop { position: (0.33 + glowRoot.wavePhase) % 1.0; color: "#7b2cbf" }
                GradientStop { position: (0.66 + glowRoot.wavePhase) % 1.0; color: "#ff007f" }
                GradientStop { position: 1.0; color: "#ff9e00" }
            }
            opacity: 0.85
            // Fade inwards mask
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 1.0; color: "#000000" }
                }
                color: "transparent"
            }
        }

        // Bottom Edge Glow
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: glowRoot.glowSpread
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#ff9e00" }
                GradientStop { position: (0.33 + glowRoot.wavePhase) % 1.0; color: "#ff007f" }
                GradientStop { position: (0.66 + glowRoot.wavePhase) % 1.0; color: "#7b2cbf" }
                GradientStop { position: 1.0; color: "#00f0ff" }
            }
            opacity: 0.85
        }

        // Left Edge Glow
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: glowRoot.glowSpread
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: "#00f0ff" }
                GradientStop { position: (0.5 + glowRoot.wavePhase) % 1.0; color: "#7b2cbf" }
                GradientStop { position: 1.0; color: "#ff9e00" }
            }
            opacity: 0.85
        }

        // Right Edge Glow
        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: glowRoot.glowSpread
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: "#ff9e00" }
                GradientStop { position: (0.5 + glowRoot.wavePhase) % 1.0; color: "#ff007f" }
                GradientStop { position: 1.0; color: "#00f0ff" }
            }
            opacity: 0.85
        }

        // Layer 3: Razor-sharp Neon Crest (Inner boundary definition)
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 20
            border.width: 3.5
            border.color: {
                if (glowRoot.glowState === "speaking") {
                    return "#18ffff";
                } else if (glowRoot.glowState === "thinking" || glowRoot.glowState === "transcribing") {
                    return "#b388ff";
                }
                return "#00f0ff";
            }
            opacity: 0.95
        }

        // Secondary subtle inner border ring
        Rectangle {
            anchors.fill: parent
            anchors.margins: 4
            color: "transparent"
            radius: 18
            border.width: 1.5
            border.color: "#d63384"
            opacity: 0.75 + 0.25 * glowRoot.audioAmp
        }
    }
}
