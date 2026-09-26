#include "bus.h"

Bus::Bus() {
    // Inicializar la RAM con ceros
    userRam.fill(0);
}

bool Bus::loadBios(const std::vector<uint8_t>& biosData) {
    if (biosData.empty()) return false;
    biosRom = biosData;
    std::cout << "[Bus] BIOS cargada correctamente (" << biosRom.size() << " bytes)." << std::endl;
    return true;
}

bool Bus::loadProgramRom(const std::vector<uint8_t>& romData) {
    if (romData.empty()) return false;
    programRom = romData;
    std::cout << "[Bus] Program ROM cargada correctamente (" << programRom.size() << " bytes)." << std::endl;
    return true;
}

// ============================================================================
// LECTURA DE MEMORIA (READ)
// ============================================================================

uint8_t Bus::read8(uint32_t addr) {
    // El M68000 solo usa las direcciones de 24 bits
    addr &= 0xFFFFFF;

    // 1. Program ROM (M68k Code: 0x000000 - 0x0FFFFF)
    if (addr < 0x100000) {
        if (addr < programRom.size()) {
            return programRom[addr];
        }
        return 0x00; // Valor por defecto si lee fuera de rango
    }

    // 2. User RAM (64 KB: 0x100000 - 0x10FFFF)
    if (addr >= 0x100000 && addr <= 0x10FFFF) {
        return userRam[addr - 0x100000];
    }

    // 3. System BIOS ROM (128 KB: 0xC00000 - 0xC1FFFF)
    if (addr >= 0xC00000 && addr <= 0xC1FFFF) {
        uint32_t biosAddr = addr - 0xC00000;
        if (biosAddr < biosRom.size()) {
            return biosRom[biosAddr];
        }
        return 0x00;
    }

    // Direcciones aún no mapeadas (LSPC, Registros I/O, Paleta, etc.)
    return 0x00;
}

uint16_t Bus::read16(uint32_t addr) {
    // El M68000 es Big-Endian: el byte más significativo (MSB) va primero
    uint8_t msb = read8(addr);
    uint8_t lsb = read8(addr + 1);
    return static_cast<uint16_t>((msb << 8) | lsb);
}

uint32_t Bus::read32(uint32_t addr) {
    // Combina dos lecturas de 16 bits en Big-Endian
    uint16_t high = read16(addr);
    uint16_t low = read16(addr + 2);
    return (static_cast<uint32_t>(high) << 16) | low;
}

// ============================================================================
// ESCRITURA EN MEMORIA (WRITE)
// ============================================================================

void Bus::write8(uint32_t addr, uint8_t data) {
    addr &= 0xFFFFFF;

    // 1. User RAM (64 KB: 0x100000 - 0x10FFFF)
    if (addr >= 0x100000 && addr <= 0x10FFFF) {
        userRam[addr - 0x100000] = data;
        return;
    }

    // Nota: ROMs (0x000000 y 0xC00000) son de solo lectura, por lo que se ignoran escrituras.
}

void Bus::write16(uint32_t addr, uint16_t data) {
    // Descomponer en Big-Endian (MSB primero, LSB después)
    write8(addr, static_cast<uint8_t>(data >> 8));
    write8(addr + 1, static_cast<uint8_t>(data & 0xFF));
}

void Bus::write32(uint32_t addr, uint32_t data) {
    // Descomponer en Big-Endian (Word alta primero, Word baja después)
    write16(addr, static_cast<uint16_t>(data >> 16));
    write16(addr + 2, static_cast<uint16_t>(data & 0xFFFF));
}