#ifndef HEAVYBRAKING_H
#define HEAVYBRAKING_H

#include <stdint.h>

#define BRAKE   --
#define X_AXIS  34
#define Y_AXIS  39
#define Z_AXIS  36

extern void setupHeavyBraking();
extern bool detectHeavyBraking();

#endif /* HEAVYBRAKING_H */