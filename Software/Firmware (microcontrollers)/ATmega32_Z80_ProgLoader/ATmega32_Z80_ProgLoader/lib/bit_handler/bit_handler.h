#ifndef BIT_HANDLER_H_
#define BIT_HANDLER_H_

// BIT HANDLING CONSTANTS:
#define TOP_NIBBLE_MASK 0xF0
#define BOT_NIBBLE_MASK 0x0F
#define BYTE_MASK TOP_NIBBLE_MASK | BOT_NIBBLE_MASK

#define port_set(port, data, mask) (port = data & mask)
#define bit_get(p,m) ((p) & (1<<m))
#define bit_set(p,m) ((p) |= (1<<m))
#define bit_clear(p,m) ((p) &= ~(1<<m))
#define bit_flip(p,m) ((p) ^= (1<<m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#define CLK(PORT,PIN)    bit_set(PORT, PIN); bit_clear(PORT,PIN)
#define INVCLK(PORT,PIN) bit_clear(PORT,PIN); bit_set(PORT,PIN)
#define INVCLK_TU(PORT,PIN,TIMEOUT) bit_clear(PORT,PIN); delayMicroseconds(TIMEOUT); bit_set(PORT,PIN)
#define INVCLK_TM(PORT,PIN,TIMEOUT) bit_clear(PORT,PIN); delay(TIMEOUT); bit_set(PORT,PIN)
#define INVCLK_TU_IN(PORT,INPUT_PORT,PIN,TIMEOUT,INTO_VAR) bit_clear(PORT,PIN); delayMicroseconds(TIMEOUT); INTO_VAR = INPUT_PORT; bit_set(PORT,PIN)

#endif