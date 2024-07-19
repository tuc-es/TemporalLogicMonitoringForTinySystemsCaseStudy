#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int monitor(uint8_t r1,uint8_t y1,uint8_t g1,uint8_t r2,uint8_t y2,uint8_t g2,uint8_t a1,uint8_t a2);

#define CHECKMON(STEP,VAL) if (failed!=VAL) { printf("Monitor result differs from expected result in step %d\n",STEP); return 1; }

int main() {
  int failed = 0;
  failed = monitor(1,0,0,1,0,0,0,0);
  CHECKMON(1,0)
  failed = monitor(1,1,0,1,0,0,0,0);
  CHECKMON(2,0)
  failed = monitor(0,0,1,1,0,0,0,0);
  CHECKMON(3,0)
  failed = monitor(0,1,0,1,0,0,0,0);
  CHECKMON(4,0)
  failed = monitor(1,0,0,1,1,0,0,0);
  CHECKMON(5,0)
  failed = monitor(1,0,0,0,0,1,0,0);
  CHECKMON(5,0)
  failed = monitor(0,0,1,0,0,1,0,0);
  CHECKMON(6,1)
  return 0;
}
