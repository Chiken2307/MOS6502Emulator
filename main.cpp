#include <iostream>
#include <array>
#include <cstdint>


class CPU{
    public:
        std::array<uint8_t, 65536> memory{};

        enum Flag: uint8_t{
            C = 1 << 0,
            Z = 1 << 1,
            I = 1 << 2,
            D = 1 << 3,
            B = 1 << 4,
            U = 1 << 5,
            V = 1 << 6,
            N = 1 << 7
        };

        uint8_t status = 0;

        uint8_t A = 0;
        uint8_t Y = 0;
        uint8_t X = 0;

        uint16_t PC = 0;

        bool running = true;

        uint8_t fetch(){
            return memory[PC++];
        }

        void setFlag(Flag flag, bool value){
            if (value)
                status |= flag;
            else
                status &= ~flag;
        }

        bool getFlag(Flag flag){
            return(status & flag) != 0;
        }

        void updateZN(uint8_t value){
            setFlag(Z, value == 0);
            setFlag(N, (value & 0x80) !=0);
        }

        void ADC(uint8_t operand){
                uint16_t sum = A + operand + getFlag(C);
                uint8_t result = static_cast<uint8_t>(sum);

                setFlag(C, sum > 0xFF);
                setFlag(V, (~(A ^ operand) & (A ^ result) & 0x80) !=0);

                A = result;

                setFlag(Z, A == 0);
                setFlag(N, (A & 0x80) != 0);

        }

        void STA(uint8_t address){
            memory[address] = A;
        }


        void execute(uint8_t opcode){
            switch (opcode)
            {
            case 0x01: // Loads value into Register A
                A = fetch();
                break;
            case 0x02: // Loads value into register Y
                Y = fetch();
                break;
            case 0x03:
                X = fetch();
                break;
            case 0x04:// Adds Value
                A += Y;
                break;
            case 0x05: // Subtracts Value
                A -= Y;
                break;
            case 0x18: // Clear Flags
                setFlag(C, false);
                break;
            case 0x69: // ADC - Add to Carry so flags work
                ADC(fetch());
                break;
            case 0x65: // ADC - Add numbers from memory
                ADC(memory[fetch()]);
                break;
            case 0x75:
                ADC(memory[static_cast<uint8_t>(fetch() + X)]);
                break;
            case 0x85: // Stores Values into Memory
                STA(fetch());
                break;
            case 0xFF: // End Code / Stop Reading code
                running = false;
                break;
            default:
                std::cerr <<"Unkown opcode\n";
                running == false;
                break;
            }
        }

        void run(){
            while (running)
            {
                execute(fetch());
            }
            
        }
};

int main() {
    CPU cpu;


    //Stored Value 50 into the memory
    cpu.memory[0] = 0x01;
    cpu.memory[1] = -50;
    cpu.memory[2] = 0x85;
    cpu.memory[3] = 0x10;

    //Add 50 to memory[0x0010] which is valued 50

    //Set A to 50
    cpu.memory[4] = 0x01;
    cpu.memory[5] = 50;
    //Clear Flags to 0
    cpu.memory[6] = 0x18;

    //Add Cary
    cpu.memory[7] = 0x65;
    cpu.memory[8] = 0x10;

    cpu.memory[9] = 0xFF;



    cpu.run();

    std::cout <<"Result: "
            << static_cast<int>(cpu.A)
            <<'\n';

    return 0;
}
