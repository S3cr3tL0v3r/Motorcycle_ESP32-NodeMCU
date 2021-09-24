#ifndef HEAVYBRAKING_H
#define HEAVYBRAKING_H

#include <stdint.h>

#define X_AXIS  34
#define Y_AXIS  39
#define Z_AXIS  36

extern bool heavyBraking;

extern void setupHeavyBraking();
extern void readAxis();
extern void detectHeavyBraking();

#endif /* HEAVYBRAKING_H */