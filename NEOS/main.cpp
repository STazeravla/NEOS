#include <SDL.h>
#include <iostream>
#include <cstdint>

// Resolución nativa de SNK Neo Geo
constexpr int LOGICAL_WIDTH = 320;
constexpr int LOGICAL_HEIGHT = 224;
constexpr int SCALE_FACTOR = 3; // Ventana escalada a 960x672

// Frecuencia de refresco exacta (~59.18 Hz)
constexpr double TARGET_FPS = 59.18;
constexpr double MS_PER_FRAME = 1000.0 / TARGET_FPS;

int main(int argc, char* argv[]) {
    // 1. Inicializar sub-sistemas de SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "Error al inicializar SDL2: " << SDL_GetError() << std::endl;
        return -1;
    }

    // 2. Crear Ventana
    SDL_Window* window = SDL_CreateWindow(
        "NEOS - Neo Emulation Operating System (v0.1.0)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LOGICAL_WIDTH * SCALE_FACTOR,
        LOGICAL_HEIGHT * SCALE_FACTOR,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // 3. Crear Renderizador acelerado por hardware
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Definir la resolución lógica (SDL reescala automáticamente)
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // 4. Bucle Principal de Ejecución
    bool isRunning = true;
    SDL_Event event;

    uint64_t perfFrequency = SDL_GetPerformanceFrequency();
    uint64_t lastCounter = SDL_GetPerformanceCounter();

    std::cout << "[NEOS Engine] Sistema inicializado correctamente." << std::endl;
    std::cout << "[NEOS Engine] Ejecutando bucle a " << TARGET_FPS << " FPS..." << std::endl;

    while (isRunning) {
        uint64_t currentCounter = SDL_GetPerformanceCounter();
        double deltaTime = static_cast<double>(currentCounter - lastCounter) * 1000.0 / perfFrequency;

        // Procesar entradas y eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
        }

        // --- RENDERIZADO DE PRUEBA ---
        // Color de fondo azul oscuro (estilo pantalla de test/BIOS Neo Geo)
        SDL_SetRenderDrawColor(renderer, 10, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Presentar el búfer de imagen en la pantalla
        SDL_RenderPresent(renderer);

        // Control de tiempo para mantener ~60 FPS
        if (deltaTime < MS_PER_FRAME) {
            SDL_Delay(static_cast<Uint32>(MS_PER_FRAME - deltaTime));
        }

        lastCounter = currentCounter;
    }

    // 5. Limpieza y apagado
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "[NEOS Engine] Apagando emulador..." << std::endl;
    return 0;
}