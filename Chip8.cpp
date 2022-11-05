#include <cstdint>
#include <fstream>
#include <string_view>


#include "Consts.h"
#include "Chip8.h"


Chip8::Chip8()
{
	// Initialize PC
	pc = START_ADDRESS;

    // Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = FONTSET[i];
	}
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

void Chip8::OP_1nnn()
{

}
void Chip8::OP_2nnn()
{

}
void Chip8::OP_3xkk()
{

}
void Chip8::OP_4xkk()
{

}
void Chip8::OP_5xy0()
{

}
void Chip8::OP_6xkk()
{

}
void Chip8::OP_7xkk()
{

}
void Chip8::OP_9xy0()
{

}
void Chip8::OP_Annn()
{

}
void Chip8::OP_Bnnn()
{

}
void Chip8::OP_Cxkk()
{

}
void Chip8::OP_Dxyn()
{

}

void OP_00E0()
{

}
void OP_00EE()
{

}

void Chip8::OP_8xy0()
{

}
void Chip8::OP_8xy1()
{

}
void Chip8::OP_8xy2()
{

}
void Chip8::OP_8xy3()
{

}
void Chip8::OP_8xy4()
{

}
void Chip8::OP_8xy5()
{

}
void Chip8::OP_8xy6()
{

}
void Chip8::OP_8xy7()
{

}
void Chip8::OP_8xyE()
{

}

void Chip8::OP_ExA1()
{

}
void Chip8::OP_Ex9E()
{

}

void Chip8::OP_Fx07()
{

}
void Chip8::OP_Fx0A()
{

}
void Chip8::OP_Fx15()
{

}
void Chip8::OP_Fx18()
{

}
void Chip8::OP_Fx1E()
{

}
void Chip8::OP_Fx33()
{

}
void Chip8::OP_Fx29()
{

}
void Chip8::OP_Fx65()
{

}
void Chip8::OP_Fx55()
{

}