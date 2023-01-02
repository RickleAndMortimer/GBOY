#include "SDL2/SDL.h"
#include <bitset>
#include <climits>
// Registers

union cpu_register_t {
    struct {
        Uint8 high;   
        Uint8 low;   
    };
    Uint16 combined;
};

struct instruction_t {
    Uint8 opcode;
    Uint8 operand;
};

Uint8 memory[0x10000];

static cpu_register_t af, bc, de, hl;
static Uint16 sp = 0, pc = 0;

inline void setFlag(char bit) {
    af.low |= (1 << bit);
}

inline void resetFlag(char bit) {
    af.low &= ~(1 << bit);
}

inline bool getFlag(char bit) {
    return af.low & (1 << bit);
}

#define setZero() setFlag(7)
#define setSubtraction() setFlag(6)
#define setHalfCarry() setFlag(5)
#define setCarry() setFlag(4)

#define resetZero() resetFlag(7)
#define resetSubtraction() resetFlag(6)
#define resetHalfCarry() resetFlag(5)
#define resetCarry() resetFlag(4)

#define getZero() getFlag(7)
#define getSubtraction() getFlag(6)
#define getHalfCarry() getFlag(5)
#define getCarry() getFlag(4)

void halt() {
}

inline void add(Uint8 amt) {
    setSubtraction();    
    if (af.high & amt & 0x80)
        setCarry();
    if (af.high == 0)
        setZero();    
    if (af.high & amt & 0x8)
        setHalfCarry();

    af.high += amt;    
}

inline void add_reg(cpu_register_t* a, Uint16 b) {
    setSubtraction();    
    if (a->combined & b & 0x80)
        setCarry();
    if (a->combined == 0)
        setZero();    
    if (a->combined & b & 0x8)
        setHalfCarry();

    a->combined += b;
}

inline void sub(Uint8 amt) {
    setSubtraction();    
    if (af.high & amt & 0x80)
        setCarry();
    if (af.high & amt & 0x8)
        setHalfCarry();

    af.high -= amt;    
    if (af.high == 0)
        setZero();    
}

inline void sbc(Uint8 amt) {
    af.high -= getCarry();
    sbc(amt);
}

inline void adc(Uint8 amt) {
    af.high += getCarry();
    add(amt);
}

void no_op() {
}

// Instructions

inline void ld_full(cpu_register_t* a, Uint16 b) {
    a->combined = b;
}

inline void ld_half(Uint8* a, Uint8 b) {
    *a = b;
}

inline void inc(Uint8* a) {
    bool half_carry = *a & (1 << 4);
    (*a)++;
    half_carry = half_carry && ((*a & (1 << 4)) == 0);

    resetSubtraction();

    if (*a == 0)
        setZero();
    else if (half_carry)
        setHalfCarry();
}

inline void dec(Uint8* a) {
    bool half_carry = *a & (1 << 4);
    (*a)--;
    half_carry = half_carry && ((*a & (1 << 4)) == 0);

    setSubtraction();

    if (*a == 0)
        setZero();
    else if (half_carry)
        setHalfCarry();
}

inline void and_op(Uint8 a) {
    af.high &= a;
    if (af.high == 0)
        setZero();
    resetCarry();
    resetSubtraction();
    resetHalfCarry();
}

inline void xor_op(Uint8 a) {
    af.high ^= a;
    if (af.high == 0 )
        setZero();
    setHalfCarry();
    resetCarry();
    resetSubtraction();
}

inline void or_op(Uint8 a) {
    af.high |= a;
    if (af.high == 0 )
        setZero();
    setHalfCarry();
    resetCarry();
    resetSubtraction();
}

inline void cp(Uint8 amt) {
    setSubtraction();    
    Uint8 result = af.high - amt;    
    if (af.high & amt & 0x80)
        setCarry();
    if (result == 0)
        setZero();    
    if (af.high & amt & 0x8)
        setHalfCarry();

}
#define ld_a(Uint8, b) ld_half(af, b, true);

int main(int argc, char** argv) {
    printf("%u", af.high);
    return 0; 
}

void execute_cmd(Uint8 instruction) {
    switch (instruction) {
        case 0x0: 
            no_op(); 
            break;
        case 0x1: 

            break;
        case 0x2: 
            ld_half(&memory[bc.combined], af.high);
            break;
        case 0x3: 
            bc.combined++;
            break;
        case 0x4: 
            inc(&bc.high);
            break;
        case 0x5: 
            dec(&bc.high);
            break;
        case 0x6: 
            ld_half(&bc.high, memory[++pc]);
            break;
        case 0x7: 

            break;
        case 0x8: 

            break;
        case 0x9: 
            add_reg(&hl, bc.combined);
            break;
        case 0xa: 
            ld_half(&af.high, memory[bc.combined]);
            break;
        case 0xb: 
            bc.combined--;
            break;
        case 0xc: 
            inc(&bc.low);
            break;
        case 0xd: 
            dec(&bc.low);
            break;
        case 0xe: 
            ld_half(&bc.low, memory[++pc]);

            break;
        case 0xf: 

            break;
        case 0x10: 

            break;
        case 0x11: 

            break;
        case 0x12: 
            ld_half(&memory[de.combined], af.high);
            break;
        case 0x13: 
            de.combined++;
            break;
        case 0x14: 
            inc(&de.high);
            break;
        case 0x15: 
            dec(&de.high);
            break;
        case 0x16: 
            ld_half(&de.high, memory[++pc]);
            break;
        case 0x17: 

            break;
        case 0x18: 

            break;
        case 0x19: 
            add_reg(&hl, de.combined);
            break;
        case 0x1a: 
            ld_half(&af.high, memory[de.combined]);
            break;
        case 0x1b: 
            de.combined--;
            break;
        case 0x1c: 
            inc(&de.low);
            break;
        case 0x1d: 
            dec(&de.low);
            break;
        case 0x1e: 
            ld_half(&de.low, memory[++pc]);
            break;
        case 0x1f: 

            break;
        case 0x20: 

            break;
        case 0x21: 

            break;
        case 0x22: 
            ld_half(&memory[hl.combined++], af.high);
            break;
        case 0x23: 
            hl.combined++;
            break;
        case 0x24: 
            inc(&hl.high);
            break;
        case 0x25: 
            dec(&hl.high);
            break;
        case 0x26: 
            ld_half(&hl.high, memory[++pc]);
            break;
        case 0x27: 

            break;
        case 0x28: 

            break;
        case 0x29: 
            add_reg(&hl, hl.combined);
            break;
        case 0x2a: 
            ld_half(&af.high, memory[hl.combined++]);
            break;
        case 0x2b: 
            hl.combined--;
            break;
        case 0x2c: 
            inc(&hl.low);
            break;
        case 0x2d: 
            dec(&hl.low);
            break;
        case 0x2e: 
            ld_half(&hl.low, memory[++pc]);
            break;
        case 0x2f: 

            break;
        case 0x30: 

            break;
        case 0x31: 

            break;
        case 0x32: 
            ld_half(&memory[hl.combined--], af.high);
            break;
        case 0x33: 
            sp++;
            break;
        case 0x34: 
            inc(&memory[hl.combined]);
            break;
        case 0x35: 
            dec(&memory[hl.combined]);
            break;
        case 0x36: 
            ld_half(&memory[hl.combined], memory[++pc]);
            break;
        case 0x37: 

            break;
        case 0x38: 

            break;
        case 0x39: 
            add_reg(&hl, sp);
            break;
        case 0x3a: 
            ld_half(&af.high, memory[hl.combined--]);
            break;
        case 0x3b: 
            sp--;
            break;
        case 0x3c: 
            inc(&af.high);
            break;
        case 0x3d: 
            dec(&af.high);
            break;
        case 0x3e: 
            ld_half(&af.high, memory[++pc]);
            break;
        case 0x3f: 

            break;
        case 0x40: 
            ld_half(&bc.high, bc.high);
            break;
        case 0x41: 
            ld_half(&bc.high, bc.low);
            break;
        case 0x42: 
            ld_half(&bc.high, de.high);
            break;
        case 0x43: 
            ld_half(&bc.high, de.low);
            break;
        case 0x44: 
            ld_half(&bc.high, hl.high);
            break;
        case 0x45: 
            ld_half(&bc.high, hl.low);
            break;
        case 0x46: 
            ld_half(&bc.high, memory[hl.combined]);
            break;
        case 0x47: 
            ld_half(&bc.high, af.high);
            break;
        case 0x48: 
            ld_half(&bc.low, bc.high);
            break;
        case 0x49: 
            ld_half(&bc.low, bc.low);
            break;
        case 0x4a: 
            ld_half(&bc.low, de.high);
            break;
        case 0x4b: 
            ld_half(&bc.low, de.low);
            break;
        case 0x4c: 
            ld_half(&bc.low, hl.high);
            break;
        case 0x4d: 
            ld_half(&bc.low, hl.low);
            break;
        case 0x4e: 
            ld_half(&bc.low, memory[hl.combined]);
            break;
        case 0x4f: 
            ld_half(&bc.low, af.high);
            break;
        case 0x50: 
            ld_half(&de.high, bc.high);
            break;
        case 0x51: 
            ld_half(&de.high, bc.low);
            break;
        case 0x52: 
            ld_half(&de.high, de.high);
            break;
        case 0x53: 
            ld_half(&de.high, de.low);
            break;
        case 0x54: 
            ld_half(&de.high, hl.high);
            break;
        case 0x55: 
            ld_half(&de.high, hl.low);
            break;
        case 0x56: 
            ld_half(&de.high, memory[hl.combined]);
            break;
        case 0x57: 
            ld_half(&de.high, af.high);
            break;
        case 0x58: 
            ld_half(&de.low, bc.high);
            break;
        case 0x59: 
            ld_half(&de.low, bc.low);
            break;
        case 0x5a: 
            ld_half(&de.low, de.high);
            break;
        case 0x5b: 
            ld_half(&de.low, de.low);
            break;
        case 0x5c: 
            ld_half(&de.low, hl.high);
            break;
        case 0x5d: 
            ld_half(&de.low, hl.low);
            break;
        case 0x5e: 
            ld_half(&de.low, memory[hl.high]);
            break;
        case 0x5f: 
            ld_half(&de.low, af.high);
            break;
        case 0x60: 
            ld_half(&hl.high, bc.high);
            break;
        case 0x61: 
            ld_half(&hl.high, bc.low);
            break;
        case 0x62: 
            ld_half(&hl.high, de.high);
            break;
        case 0x63: 
            ld_half(&hl.high, de.low);
            break;
        case 0x64: 
            ld_half(&hl.high, hl.high);
            break;
        case 0x65: 
            ld_half(&hl.high, hl.low);
            break;
        case 0x66: 
            ld_half(&hl.high, memory[hl.combined]);
            break;
        case 0x67: 
            ld_half(&hl.high, af.high);
            break;
        case 0x68: 
            ld_half(&hl.low, bc.high);
            break;
        case 0x69: 
            ld_half(&hl.low, bc.low);
            break;
        case 0x6a: 
            ld_half(&hl.low, de.high);
            break;
        case 0x6b: 
            ld_half(&hl.low, de.low);
            break;
        case 0x6c: 
            ld_half(&hl.low, hl.high);
            break;
        case 0x6d: 
            ld_half(&hl.low, hl.low);
            break;
        case 0x6e: 
            ld_half(&hl.low, memory[hl.combined]);
            break;
        case 0x6f: 
            ld_half(&hl.low, af.high);
            break;
        case 0x70: 
            ld_half(&memory[hl.combined], bc.high);
            break;
        case 0x71: 
            ld_half(&memory[hl.combined], bc.low);
            break;
        case 0x72: 
            ld_half(&memory[hl.combined], de.high);
            break;
        case 0x73: 
            ld_half(&memory[hl.combined], de.low);
            break;
        case 0x74: 
            ld_half(&memory[hl.combined], hl.high);
            break;
        case 0x75: 
            ld_half(&memory[hl.combined], hl.low);
            break;
        case 0x76: 
            halt();
            break;
        case 0x77: 
            ld_half(&memory[hl.combined], af.high);
            break;
        case 0x78: 
            ld_half(&af.high, bc.high);
            break;
        case 0x79: 
            ld_half(&af.high, bc.low);
            break;
        case 0x7a: 
            ld_half(&af.high, de.high);
            break;
        case 0x7b: 
            ld_half(&af.high, de.low);
            break;
        case 0x7c: 
            ld_half(&af.high, hl.high);
            break;
        case 0x7d: 
            ld_half(&af.high, hl.low);
            break;
        case 0x7e: 
            ld_half(&af.high, memory[hl.combined]);
            break;
        case 0x7f: 
            ld_half(&af.high, af.high);
            break;
        case 0x80: 
            add(bc.high);
            break;
        case 0x81: 
            add(bc.low);
            break;
        case 0x82: 
            add(de.high);
            break;
        case 0x83: 
            add(de.low);
            break;
        case 0x84: 
            add(hl.high);
            break;
        case 0x85: 
            add(hl.low);
            break;
        case 0x86: 
            add(memory[hl.combined]);
            break;
        case 0x87: 
            add(af.high);
            break;
        case 0x88: 
            adc(bc.high);
            break;
        case 0x89: 
            adc(bc.low);
            break;
        case 0x8a: 
            adc(de.high);
            break;
        case 0x8b: 
            adc(de.low);
            break;
        case 0x8c: 
            adc(hl.high);
            break;
        case 0x8d: 
            adc(hl.low);
            break;
        case 0x8e: 
            adc(memory[hl.combined]);
            break;
        case 0x8f: 
            adc(af.high);
            break;
        case 0x90: 

            break;
        case 0x91: 

            break;
        case 0x92: 

            break;
        case 0x93: 

            break;
        case 0x94: 

            break;
        case 0x95: 

            break;
        case 0x96: 

            break;
        case 0x97: 

            break;
        case 0x98: 
            sbc(bc.high);
            break;
        case 0x99: 
            sbc(bc.low);
            break;
        case 0x9a: 
            sbc(de.high);
            break;
        case 0x9b: 
            sbc(de.low);
            break;
        case 0x9c: 
            sbc(hl.high);
            break;
        case 0x9d: 
            sbc(hl.low);
            break;
        case 0x9e: 
            sbc(memory[hl.combined]);
            break;
        case 0x9f: 
            sbc(af.high);
            break;
        case 0xa0: 
            and_op(bc.high);
            break;
        case 0xa1: 
            and_op(bc.low);
            break;
        case 0xa2: 
            and_op(de.high);
            break;
        case 0xa3: 
            and_op(de.low);
            break;
        case 0xa4: 
            and_op(hl.high);
            break;
        case 0xa5: 
            and_op(hl.low);
            break;
        case 0xa6: 
            and_op(memory[hl.combined]);
            break;
        case 0xa7: 
            and_op(af.high);
            break;
        case 0xa8: 
            xor_op(bc.high);
            break;
        case 0xa9: 
            xor_op(bc.low);
            break;
        case 0xaa: 
            xor_op(de.high);
            break;
        case 0xab: 
            xor_op(de.low);
            break;
        case 0xac: 
            xor_op(hl.high);
            break;
        case 0xad: 
            xor_op(hl.low);
            break;
        case 0xae: 
            xor_op(memory[hl.combined]);
            break;
        case 0xaf: 
            xor_op(af.high);
            break;
        case 0xb0: 
            or_op(bc.high);
            break;
        case 0xb1: 
            or_op(bc.low);
            break;
        case 0xb2: 
            or_op(de.high);
            break;
        case 0xb3: 
            or_op(de.low);
            break;
        case 0xb4: 
            or_op(hl.high);
            break;
        case 0xb5: 
            or_op(hl.low);
            break;
        case 0xb6: 
            or_op(memory[hl.combined]);
            break;
        case 0xb7: 
            or_op(af.high);
            break;
        case 0xb8: 
            cp(bc.high);
            break;
        case 0xb9: 
            cp(bc.low);
            break;
        case 0xba: 
            cp(de.high);
            break;
        case 0xbb: 
            cp(de.low);
            break;
        case 0xbc: 
            cp(hl.high);
            break;
        case 0xbd: 
            cp(hl.low);
            break;
        case 0xbe: 
            cp(memory[hl.combined]);
            break;
        case 0xbf: 
            cp(af.high);
            break;
        case 0xc0: 

            break;
        case 0xc1: 

            break;
        case 0xc2: 

            break;
        case 0xc3: 

            break;
        case 0xc4: 

            break;
        case 0xc5: 

            break;
        case 0xc6: 

            break;
        case 0xc7: 

            break;
        case 0xc8: 

            break;
        case 0xc9: 

            break;
        case 0xca: 

            break;
        case 0xcb: 

            break;
        case 0xcc: 

            break;
        case 0xcd: 

            break;
        case 0xce: 

            break;
        case 0xcf: 

            break;
        case 0xd0: 

            break;
        case 0xd1: 

            break;
        case 0xd2: 

            break;
        case 0xd3: 

            break;
        case 0xd4: 

            break;
        case 0xd5: 

            break;
        case 0xd6: 

            break;
        case 0xd7: 

            break;
        case 0xd8: 

            break;
        case 0xd9: 

            break;
        case 0xda: 

            break;
        case 0xdb: 

            break;
        case 0xdc: 

            break;
        case 0xdd: 

            break;
        case 0xde: 

            break;
        case 0xdf: 

            break;
        case 0xe0: 
            break;
        case 0xe1: 

            break;
        case 0xe2: 

            break;
        case 0xe3: 

            break;
        case 0xe4: 

            break;
        case 0xe5: 

            break;
        case 0xe6: 

            break;
        case 0xe7: 

            break;
        case 0xe8: 

            break;
        case 0xe9: 

            break;
        case 0xea: 

            break;
        case 0xeb: 

            break;
        case 0xec: 

            break;
        case 0xed: 

            break;
        case 0xee: 

            break;
        case 0xef: 

            break;
        case 0xf0: 

            break;
        case 0xf1: 

            break;
        case 0xf2: 

            break;
        case 0xf3: 

            break;
        case 0xf4: 

            break;
        case 0xf5: 

            break;
        case 0xf6: 

            break;
        case 0xf7: 

            break;
        case 0xf8: 

            break;
        case 0xf9: 

            break;
        case 0xfa: 

            break;
        case 0xfb: 

            break;
        case 0xfc: 

            break;
        case 0xfd: 

            break;
        case 0xfe: 

            break;
        case 0xff: 

            break;
    }
}
