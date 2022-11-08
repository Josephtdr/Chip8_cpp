#ifndef CHIP_8
#define CHIP_8

#include <cstdint>
#include <random>

#include "Consts.h"

class Chip8
{
private:
	uint8_t registers[REGISTER_COUNT]{};
	uint8_t memory[MEMORY_SIZE]{};
	uint16_t index{};
	uint16_t pc{};
	uint16_t stack[STACK_SIZE]{};
	uint8_t sp{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint16_t opcode;
    
    std::mt19937 mt;
    std::uniform_int_distribution<uint8_t> randByte;

public:
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};
    uint8_t keypad[KEY_COUNT]{};

    Chip8();

    /**
     * @brief Load rom instructions at filename into memory
     *        starting at START_ADDRESS.
     * 
     * @param filename : address to rom
     */
    void LoadROM(char const* filename);

    void Cycle();

private:
    //OPCODES
    using opcodeFnPtr = void(Chip8::*)();

    opcodeFnPtr table[16]{};
    opcodeFnPtr table0[0xF]{};
    opcodeFnPtr table8[0xF]{};
    opcodeFnPtr tableE[0xF]{};
    opcodeFnPtr tableF[0x66]{};
    
    void setupTables();
    void Table0();
    void Table8();
    void TableE();
    void TableF();

    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();

    void OP_00E0();
    void OP_00EE();

    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();

    void OP_ExA1();
    void OP_Ex9E();

    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx33();
    void OP_Fx29();
    void OP_Fx65();
    void OP_Fx55();

    //Null function for incorrect address
    void OP_NULL() {};
};


#endif