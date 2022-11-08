#include "Consts.h"
#include "Chip8.h"

#include<cstring> //memset, 
#include <cstdint>
#include <fstream> //streams
#include <string_view>
#include <chrono>
#include <numeric>//accumulate



Chip8::Chip8()
    : mt{ std::random_device{}() },
      randByte{ std::uniform_int_distribution<uint8_t>(0, 255U) }
{
	// Initialise PC
	pc = START_ADDRESS;

    // Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = FONTSET[i];
    }

    setupTables();


}

void Chip8::LoadROM(char const* filename)
{
    // Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		// Free the buffer
		delete[] buffer;
	}
}

void Chip8::Cycle()
{
	// Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];
	// Increment the PC before we execute anything
	pc += 2;
    // Decode and Execute
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    // Decrement the delay timer if it's been set
	if (delayTimer > 0)
	{
		--delayTimer;
	}
	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		--soundTimer;
	}
}

void Chip8::setupTables()
{
    //Initialise tables
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    for (size_t i = 0; i <= 0xE; i++)
    {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }
    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    for (size_t i = 0; i <= 0x65; i++)
    {
        tableF[i] = &Chip8::OP_NULL;
    }

    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}
void Chip8::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}
void Chip8::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}
void Chip8::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
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
    uint8_t byte = (opcode & 0x00FFu);

    registers[Vx] = byte & randByte(mt);
}
void Chip8::OP_Dxyn()// DRW Vx, Vy, nibble
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    // Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}

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