#ifndef D3MH_UI_
#define D3MH_UI_

#include "types.h"

namespace D3 {

// SizeOf = 0x208

struct UIReference {
    UINT64 x000_Hash;
    char x008_Name[0x200];
};

}

#endif // D3MH_UI_

