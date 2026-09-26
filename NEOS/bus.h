#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <iostream>

class Bus {
public:
    Bus();
    ~Bus() = default;

    // Métodos de lectura/escritura alineados al M68000
    uint8_t  read8(uint32_t addr);
    void     write8(uint32_t addr, uint8_t data);

    uint16_t read16(uint32_t addr);
    void     write16(uint32_t addr, uint16_t data);

    uint32_t read32(uint32_t addr);
    void     write32(uint32_t addr, uint32_t data);

    // Cargar ROMs temporalmente en memoria
    bool loadBios(const std::vector<uint8_t>& biosData);
    bool loadProgramRom(const std::vector<uint8_t>& romData);

private:
    // User RAM principal (64 KB: 0x100000 - 0x10FFFF)
    std::array<uint8_t, 0x10000> userRam{};

    // BIOS del sistema (128 KB: 0xC00000 - 0xC1FFFF)
    std::vector<uint8_t> biosRom;

    // ROM de programa P1/P2 (M68k Code: 0x000000 - 0x0FFFFF)
    std::vector<uint8_t> programRom;
};