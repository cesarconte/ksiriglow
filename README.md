# KSiriglow (Apple Intelligence Siri Screen Glow para KDE Plasma 6 Wayland)

Replicación nativa en alta fidelidad del **efecto perimetral luminoso iridiscente de Apple Intelligence (Siri Screen Glow)** de macOS 15.1 Sequoia / Golden Gate para **KDE Plasma 6 (Wayland)**.

![KSiriglow](https://raw.githubusercontent.com/cesarconte/ksiriglow/main/screenshot.png)

## Características

- 🌈 **Aura Iridiscente Auténtica de Apple Intelligence**: Degradado perimetral continuo con la paleta oficial (cian eléctrico, violeta, magenta neón y ámbar cálido) y esquinas redondeadas adaptadas a la curvatura de la pantalla.
- 🎙️ **Reactividad de Audio en Tiempo Real**: El grosor, luminosidad y profundidad del halo respiran en sintonía con la amplitud de la voz (`audioLevel`).
- 🧠 **Animación Fluida de Pensamiento (*Thinking Wave*)**: Mientras la IA procesa la respuesta, una onda cromática viaja suavemente en bucle continuo a lo largo de los cuatro bordes del monitor.
- ⚡ **Integración Nativa Wayland vía `LayerShellQt`**: Renderizado sobre la capa `LayerOverlay` con la bandera `Qt.WindowTransparentForInput` (100% click-through, jamás bloquea clics ni interfiere con tus ventanas o aplicaciones).
- 📡 **Sincronización Automática con Kiri / Kity**: Escucha automáticamente las señales D-Bus de `org.cesar.kity` (`/Kity`), activándose de forma instantánea al usar el asistente.
- 🛠️ **Control D-Bus y CLI**:
  ```bash
  ksiriglow --test listening  # Prueba el halo con efecto de escucha por voz
  ksiriglow --test thinking   # Prueba la onda cromática de procesamiento
  ksiriglow --toggle          # Alterna el halo manualmente
  ksiriglow --daemon          # Inicia el demonio en segundo plano
  ```

## Compilación e Instalación

```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=$HOME/.local -DBUILD_TESTING=ON
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
cmake --install build
```

## Servicio de Usuario Systemd

Habilitado por defecto en:
`~/.config/systemd/user/ksiriglow.service`

```bash
systemctl --user enable --now ksiriglow.service
```

## Licencia

GPL-3.0-or-later © 2026 Cesar
