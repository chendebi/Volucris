#include "Volucris.h"

VOLUCRIS_DECLARE_LOG(Engine, Info)

static_assert(sizeof(int) == 4, "Error: 'int' is not 4 bytes on this platform!");
static_assert(sizeof(size_t) == 8, "Error: 'size_t' is not 8 bytes on this platform!");