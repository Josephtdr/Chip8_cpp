#include "Consts.h"
#include "Chip8.h"

#include<cstring> //memset, 
#include <cstdint>
#include <fstream>
#include <string_view>
#include <chrono>
#include <random>
#include <numeric>//accumulate

std::default_random_engine randGen;
std::uniform_int_distribution<uint8_t> randByte;

Chip8::Chip8()
    // : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	// Initialize PC
	pc = START_ADDRESS;

    // Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = FONTSET[i];
	}


    // Initialize RNG
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::loadROM(std::string_view filename)
{

}

void Chip8::Cycle()
{

}

void Chip8::Table0()
{

}
void Chip8::Table8()
{

}
void Chip8::TableE()
{

}
void Chip8::TableF()
{

}

void Chip8::OP_1nnn() // Jump
{
    uint16_t address = opcode & 0x0FFFu; 
    pc = address;
}
void Chip8::OP_2nnn() // Call
{
    stack[sp] = pc;
    ++sp;

    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}
void Chip8::OP_3xkk() // Skip if Vx = kk
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}
}
void Chip8::OP_4xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] != byte)
	{
		pc += 2;
	}
}
void Chip8::OP_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}
void Chip8::OP_6xkk() //LD Vx
{
    uint8_t byte = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = byte;
}
void Chip8::OP_7xkk() //ADD Vx
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    

    registers[Vx] += byte;
}
void Chip8::OP_9xy0() //LD Vx, Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] != registers[Vy])
    {
        pc += 2;
    }
}
void Chip8::OP_Annn() //LD I, addr
{
    uint16_t address = opcode & 0x0FFFu;

    index = address;
}
void Chip8::OP_Bnnn() //JP V0, addr
{
    uint16_t address = opcode & 0x0FFFu;

    pc = address + registers[0];
}
void Chip8::OP_Cxkk() //TODO: random gen 
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint16_t byte = (opcode & 0x00FFu);

    registers[Vx] = byte & randByte(randGen);
}
void Chip8::OP_Dxyn()// DRW Vx, Vy, nibble
{

}


void Chip8::OP_00E0() // CLS
{
    memset(video, 0, sizeof(video));
}
void Chip8::OP_00EE() // RET
{
    --sp;
	pc = stack[sp];
}

void Chip8::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] += registers[Vy];
}
void Chip8::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];
}
void Chip8::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}
void Chip8::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}
void Chip8::OP_8xy4() //ADD Vx, Vy with carry
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t result = registers[Vx] + registers[Vy];

    registers[0xF] = (result > 0x00FFu ? 1 : 0);
    registers[Vx] = result & 0x00FFu;
}
void Chip8::OP_8xy5() //SUB Vx, Vy
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vx] > registers[Vy] ? 1 : 0);
    registers[Vx] -= registers[Vy];
}
void Chip8::OP_8xy6() //SHR Vx
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[Vx] & 0x1u);

    registers[Vx] >>= 1;

}
void Chip8::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vx] < registers[Vy] ? 1 : 0);
    registers[Vx] = registers[Vy] - registers[Vx];
}
void Chip8::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

    registers[Vx] <<= 1;
}

void Chip8::OP_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (keypad[key])
    {
        pc += 2;
    }
}
void Chip8::OP_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (!keypad[key])
    {
        pc += 2;
    }
}

void Chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delayTimer;
}
void Chip8::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (std::size_t i=0; i < std::size(keypad); ++i)
    {
        if (keypad[i])
        {
            registers[Vx] = i;       
            return;
        }
    }
    //Fail case, no keys pressed
    pc -= 2;
}
void Chip8::OP_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    delayTimer = registers[Vx];
}
void Chip8::OP_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    soundTimer = registers[Vx];
}
void Chip8::OP_Fx1E() // ADD I, Vx
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    index += registers[Vx];
}
void Chip8::OP_Fx29()//Set I = location of sprite for digit Vx.
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t offset = registers[Vx];

    index = FONTSET_START_ADDRESS + (5 * offset);
}
void Chip8::OP_Fx33()//Store BCD representation of Vx in memory locations I, I+1, and I+2.
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[Vx];

    memory[index+2] = value % 10;
    value /= 10;
	
    memory[index + 1] = value % 10;
	value /= 10;

	memory[index] = value % 10;

}
void Chip8::OP_Fx55()//Store registers V0 through Vx in memory starting at location I.
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i=0; i <= Vx; ++i)
    {
        memory[index + i] = registers[i];
    }
}
void Chip8::OP_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i=0; i <= Vx; ++i)
    {
        registers[i] = memory[index + i];
    }
}