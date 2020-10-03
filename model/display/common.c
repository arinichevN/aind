#include "common.h"

const static uint8_t asciiToSegment [] = {
	0b00000000, // 32
	0b00000110,
	0b00100010,
	0b01110110,
	0b01101101,
	0b00100100,
	0b01111111,
	0b00100000,
	0b00111001,
	0b00001111,
	0b01011100,
	0b01010000,
	0b00010000,
	0b01000000,
	0b00001000,
	0b00000110,
	0b00111111, // 048 0
	0b00000110, // 049 1
	0b01011011, // 050 2
	0b01001111, // 051 3
	0b01100110, // 052 4
	0b01101101, // 053 5
	0b01111101, // 054 6
	0b00000111, // 055 7
	0b01111111, // 056 8
	0b01101111, // 057 9
	0b00000000, // 058 :
	0b00000000, // 059 ;
	0b00000000, // 060 <
	0b01001000, // 061 =
	0b00000000, // 062 >
	0b00000000, // 063 ?
	0b00000000, // 064 @
	0b01110111, // 065 A
	0b01111100, // 066 B
	0b00111001, // 067 C
	0b01011110, // 068 D
	0b01111001, // 069 E
	0b01110001, // 070 F
	0b00111101, // 071 G
	0b01110110, // 072 H
	0b00000110, // 073 I
	0b00011110, // 074 J
	0b01110110, // 075 K
	0b00111000, // 076 L
	0b00010101, // 077 M
	0b00110111, // 078 N
	0b00111111, // 079 O
	0b01110011, // 080 P
	0b01100111, // 081 Q
	0b00110001, // 082 R
	0b01101101, // 083 S
	0b01111000, // 084 T
	0b00111110, // 085 U
	0b00011100, // 086 V
	0b00101010, // 087 W
	0b01110110, // 088 X
	0b01101110, // 089 Y
	0b01011011, // 090 Z
	0b00000000, // 091 [
	0b01100100, // 092 (backslash)
	0b00000000, // 093 ]
	0b00000000, // 094 ^
	0b00000000, // 095 _
	0b00000000, // 096 `
	0b01110111, // 097 a
	0b01111100, // 098 b
	0b01011000, // 099 c
	0b01011110, // 100 d
	0b01111001, // 101 e
	0b01110001, // 102 f
	0b01101111, // 103 g
	0b01110100, // 104 h
	0b00000100, // 105 i
	0b00011110, // 106 j
	0b01110110, // 107 k
	0b00011000, // 108 l
	0b00010101, // 109 m
	0b01010100, // 110 n
	0b01011100, // 111 o
	0b01110011, // 112 p
	0b01100111, // 113 q
	0b01010000, // 114 r
	0b01101101, // 115 s
	0b01111000, // 116 t
	0b00011100, // 117 u
	0b00011100, // 118 v
	0b00101010, // 119 w
	0b01110110, // 120 x
	0b01101110, // 121 y
	0b01011011 // 122 z
};

static uint8_t swapSignBits (uint8_t v){
	uint8_t out = 0;
	for(int i = 0; i < 7; i++){
		 if(v & (1 << i)){
			 out |= (0b01000000 >> i);
		 }
	}
	return out;
}

uint8_t encodeASCII(uint8_t chr){
  if(chr > DISPLAY_LAST_CHAR || chr < DISPLAY_FIRST_CHAR) return 0;
	return asciiToSegment[chr - DISPLAY_FIRST_CHAR];
}

uint8_t encodeASCIISwap(int chr){
  if(chr > DISPLAY_LAST_CHAR || chr < DISPLAY_FIRST_CHAR) return 0;
	return swapSignBits(asciiToSegment[chr - DISPLAY_FIRST_CHAR]);
}

size_t dstrlen(const char *str){
	size_t c = 0;
	size_t slen = strlen(str);
	for (size_t i = 0; i < slen; i++) {
		if(i < slen-1){
			if(str[i+1] == '.'){//next is dot
				if(str[i] != '.'){//this is not dot
					i++;//skip next char
				}
			}
		}
		c++;
	}
	return c;
}

uint8_t encodeSegmentFromStrSwap(const char *str, size_t slen, size_t *ind){
	size_t i = *ind;
	uint8_t out = encodeASCIISwap(str[i]);
	if(str[i] == '.'){
		out |= 0b10000000;
	}
	if(i < slen-1){
		if(str[i+1] == '.'){//next is dot
			if(str[i] != '.'){//this is not dot
				out |= 0b10000000;//adhere next to this
				(*ind)++;//skip next char
			}
		}
	}
	return out;
}
