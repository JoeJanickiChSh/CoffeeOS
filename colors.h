#define BLACK 0x0
#define DARK_BLUE 0x1
#define DARK_GREEN 0x2
#define DARK_CYAN 0x3
#define DARK_RED 0x4
#define DARK_MAGENTA 0x5
#define DARK_YELLOW 0x6
#define DARK_WHITE 0x7
#define GRAY 0x8
#define BLUE 0x9
#define GREEN 0xa
#define CYAN 0xb
#define RED 0xc
#define MAGENTA 0xd
#define YELLOW 0xe
#define WHITE 0xf
#define BGFG(b, f) ( ((b & 0b0111) << 4) + f)