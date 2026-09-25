# 🕹️ NEOS (Neo Emulation Operating System)

**NEOS** es un emulador de código abierto para el sistema arcade y doméstico **SNK Neo Geo (MVS / AES)** desarrollado desde cero en **C++20** utilizando **SDL2** en **Windows 11 (x64)**.

El objetivo del proyecto es estudiar y emular la arquitectura interna del procesador Motorola 68000, la gestión de tiras de sprites en el chip de video LSPC y la sincronización del subsistema de sonido.

---

## 🛠️ Especificaciones del Hardware Emulado

| Componente | Hardware Original | Función en NEOS |
| :--- | :--- | :--- |
| **CPU Principal** | Motorola 68000 @ 12 MHz (16/32-bit) | Lógica de juego, físicas, IA e interacciones. |
| **CPU Audio** | Zilog Z80 @ 4 MHz (8-bit) | Sub-sistema de control de audio. |
| **Video (VDP)** | Custom Chip LSPC (Neo-Geo Video) | **380 sprites simultáneos**, encogimiento de sprites y capa de texto (*Fix Layer*). |
| **Resolución** | $320 \times 224$ píxeles @ 59.18 Hz | Renderizado a buffer lineal de píxeles escalable. |
| **Paleta de Color** | 4096 colores simultáneos (15-bit RGB) | Manejo de tablas de paletas indexadas en RAM. |
| **Audio** | Yamaha YM2610 | Síntesis FM + Canales ADPCM (Voces y efectos). |

---

## 🧰 Pila de Herramientas (Toolchain)

* **Sistema Operativo:** Windows 11 (x64)
* **Lenguaje:** C++20
* **IDE Recomendado:** Visual Studio 2022 (Community Edition)
* **Compilador:** MSVC / Clang-CL
* **Librerías Clave:**
  * **SDL2:** Gestión de ventana, entrada de usuario y buffers de audio.
  * **Musashi (C Core):** Núcleo *open-source* para la emulación exacta del procesador Motorola 68000.

---

## 📂 Estructura del Proyecto

```text
NEOS/
├── README.md
├── .gitignore
├── docs/                       # Documentación técnica y mapas de memoria
├── external/                   # Cores de CPU y librerías de terceros (Musashi)
└── src/
    ├── main.cpp                # Punto de entrada y loop de tiempo a 60 FPS (SDL2)
    ├── core/
    │   ├── emulator.cpp        # Coordinación del sistema global
    │   └── bus.cpp             # Mapeo de memoria (Bus de datos y direcciones)
    ├── cpu/
    │   ├── m68k_adapter.cpp    # Integración del núcleo M68000
    │   └── z80_core.cpp        # Procesador de audio Z80
    ├── video/
    │   ├── lspc.cpp            # Generador de gráficos y sprites
    │   ├── fix_layer.cpp       # Renderizado de la capa de texto y HUD
    │   └── palette.cpp         # Conversión de paletas 15-bit a RGB888
    ├── audio/
    │   └── ym2610_adapter.cpp  # Sintetizador de audio FM / ADPCM
    └── input/
        └── controllers.cpp     # Mapeo de mandos (P1/P2, Monedas, Start)