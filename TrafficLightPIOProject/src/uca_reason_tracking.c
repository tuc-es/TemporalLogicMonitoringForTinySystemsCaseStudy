/* Temporal Logic Runtime Monitor Code */
#include <stdint.h>
#include <stdio.h>
/* State storage information */
uint8_t inState0 = 1;
volatile uint32_t state0history = 0;
typedef struct {
} state0apValuesType;
volatile state0apValuesType state0apValues;
uint8_t inState1 = 0;
volatile uint32_t state1history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
  uint8_t  vals9;
  uint8_t  vals10;
} state1apValuesType;
volatile state1apValuesType state1apValues;
uint8_t inState2 = 0;
volatile uint32_t state2history = 0;
typedef struct {
  uint8_t  vals0;
} state2apValuesType;
volatile state2apValuesType state2apValues;
uint8_t inState3 = 0;
volatile uint32_t state3history = 0;
typedef struct {
  uint8_t  vals0;
} state3apValuesType;
volatile state3apValuesType state3apValues;
uint8_t inState4 = 0;
volatile uint32_t state4history = 0;
typedef struct {
  uint8_t  vals0;
} state4apValuesType;
volatile state4apValuesType state4apValues;
uint8_t inState5 = 0;
volatile uint32_t state5history = 0;
typedef struct {
  uint8_t  vals0;
} state5apValuesType;
volatile state5apValuesType state5apValues;
uint8_t inState6 = 0;
volatile uint32_t state6history = 0;
typedef struct {
  uint8_t  vals0;
} state6apValuesType;
volatile state6apValuesType state6apValues;
uint8_t inState7 = 0;
volatile uint32_t state7history = 0;
typedef struct {
  uint8_t  vals0;
} state7apValuesType;
volatile state7apValuesType state7apValues;
uint8_t inState8 = 0;
volatile uint32_t state8history = 0;
typedef struct {
  uint8_t  vals0;
} state8apValuesType;
volatile state8apValuesType state8apValues;
uint8_t inState9 = 0;
volatile uint32_t state9history = 0;
typedef struct {
  uint8_t  vals0;
} state9apValuesType;
volatile state9apValuesType state9apValues;
uint8_t inState10 = 0;
volatile uint32_t state10history = 0;
typedef struct {
  uint8_t  vals0;
} state10apValuesType;
volatile state10apValuesType state10apValues;
uint8_t inState11 = 0;
volatile uint32_t state11history = 0;
typedef struct {
  uint8_t  vals0;
} state11apValuesType;
volatile state11apValuesType state11apValues;
uint8_t inState12 = 0;
volatile uint32_t state12history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
} state12apValuesType;
volatile state12apValuesType state12apValues;
uint8_t inState13 = 0;
volatile uint32_t state13history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
} state13apValuesType;
volatile state13apValuesType state13apValues;
uint8_t inState14 = 0;
volatile uint32_t state14history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
} state14apValuesType;
volatile state14apValuesType state14apValues;
uint8_t inState15 = 0;
volatile uint32_t state15history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
} state15apValuesType;
volatile state15apValuesType state15apValues;
uint8_t inState16 = 0;
volatile uint32_t state16history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
} state16apValuesType;
volatile state16apValuesType state16apValues;
uint8_t inState17 = 0;
volatile uint32_t state17history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
} state17apValuesType;
volatile state17apValuesType state17apValues;
uint8_t inState18 = 0;
volatile uint32_t state18history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
} state18apValuesType;
volatile state18apValuesType state18apValues;
uint8_t inState19 = 0;
volatile uint32_t state19history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
} state19apValuesType;
volatile state19apValuesType state19apValues;
uint8_t inState20 = 0;
volatile uint32_t state20history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
  uint8_t  vals9;
} state20apValuesType;
volatile state20apValuesType state20apValues;
uint8_t inState21 = 0;
volatile uint32_t state21history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
} state21apValuesType;
volatile state21apValuesType state21apValues;
uint8_t inState22 = 0;
volatile uint32_t state22history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
} state22apValuesType;
volatile state22apValuesType state22apValues;
uint8_t inState23 = 0;
volatile uint32_t state23history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
} state23apValuesType;
volatile state23apValuesType state23apValues;
uint8_t inState24 = 0;
volatile uint32_t state24history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
} state24apValuesType;
volatile state24apValuesType state24apValues;
uint8_t inState25 = 0;
volatile uint32_t state25history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
} state25apValuesType;
volatile state25apValuesType state25apValues;
uint8_t inState26 = 0;
volatile uint32_t state26history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
} state26apValuesType;
volatile state26apValuesType state26apValues;
uint8_t inState27 = 0;
volatile uint32_t state27history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
} state27apValuesType;
volatile state27apValuesType state27apValues;
uint8_t inState28 = 0;
volatile uint32_t state28history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
} state28apValuesType;
volatile state28apValuesType state28apValues;
uint8_t inState29 = 0;
volatile uint32_t state29history = 0;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
  uint8_t  vals9;
} state29apValuesType;
volatile state29apValuesType state29apValues;
#define CONSTRUCTTRACEELEMENT ((r1>0?1:0)+(y1>0?2:0)+(g1>0?4:0)+(r2>0?8:0)+(y2>0?16:0)+(g2>0?32:0)+(a1>0?64:0)+(a2>0?128:0))

/* Monitor step/update function */
int monitor(uint8_t r1,uint8_t y1,uint8_t g1,uint8_t r2,uint8_t y2,uint8_t g2,uint8_t a1,uint8_t a2) {
  uint8_t nextState1 = 0;
  uint8_t nextState20 = 0;
  uint8_t nextState29 = 0;
  uint8_t nextState19 = 0;
  uint8_t nextState28 = 0;
  uint8_t nextState18 = 0;
  uint8_t nextState27 = 0;
  uint8_t nextState17 = 0;
  uint8_t nextState26 = 0;
  uint8_t nextState16 = 0;
  uint8_t nextState25 = 0;
  uint8_t nextState15 = 0;
  uint8_t nextState24 = 0;
  uint8_t nextState14 = 0;
  uint8_t nextState23 = 0;
  uint8_t nextState13 = 0;
  uint8_t nextState22 = 0;
  uint8_t nextState12 = 0;
  uint8_t nextState21 = 0;
  uint8_t nextState2 = 0;
  uint8_t nextState3 = 0;
  uint8_t nextState4 = 0;
  uint8_t nextState5 = 0;
  uint8_t nextState6 = 0;
  uint8_t nextState7 = 0;
  uint8_t nextState8 = 0;
  uint8_t nextState9 = 0;
  uint8_t nextState10 = 0;
  uint8_t nextState11 = 0;
  uint8_t nextState0 = 0;
  nextState1 = inState1 && (1);
  if (inState0 && ((g1&&g2)||(g1&&y2)||(y1&&g2)||(y1&&y2))) {
    nextState1 = 1;
    *((state0apValuesType*)&state1apValues) = state0apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: 0&2 | 0&3 | 1&2 | 1&3*/
     state1history = state0history * 12 + 0;
  }
  if (inState2 && ((!g1&&!y1)||(!g1&&!g2&&!y2))) {
    nextState1 = 1;
    *((state2apValuesType*)&state1apValues) = state2apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 2: !0&!1 | !0&!2&!3*/
     state1history = state2history * 12 + 2;
  }
  if (inState3 && ((!g1&&!y1&&!g2)||(!g2&&!y2))) {
    nextState1 = 1;
    *((state3apValuesType*)&state1apValues) = state3apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 3: !0&!1&!2 | !2&!3*/
     state1history = state3history * 12 + 3;
  }
  if (inState4 && ((!g1&&!y1&&!r1)||(y1&&!g2&&!y2)||(!g2&&!y2&&!r1))) {
    nextState1 = 1;
    *((state4apValuesType*)&state1apValues) = state4apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 4: !0&!1&!4 | 1&!2&!3 | !2&!3&!4*/
     state1history = state4history * 12 + 4;
  }
  if (inState5 && ((!g1&&!y1&&y2)||(!g1&&!y1&&!r2)||(!g2&&!y2&&!r2))) {
    nextState1 = 1;
    *((state5apValuesType*)&state1apValues) = state5apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 5: !0&!1&3 | !0&!1&!5 | !2&!3&!5*/
     state1history = state5history * 12 + 5;
  }
  if (inState6 && (!g1&&!y1)) {
    nextState1 = 1;
    *((state6apValuesType*)&state1apValues) = state6apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 6: !0&!1*/
     state1history = state6history * 12 + 6;
  }
  if (inState7 && ((!g1&&!y1&&!r1)||(!y1&&!g2&&!y2&&!r1))) {
    nextState1 = 1;
    *((state7apValuesType*)&state1apValues) = state7apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 7: !0&!1&!4 | !1&!2&!3&!4*/
     state1history = state7history * 12 + 7;
  }
  if (inState8 && (!g2&&!y2)) {
    nextState1 = 1;
    *((state8apValuesType*)&state1apValues) = state8apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 8: !2&!3*/
     state1history = state8history * 12 + 8;
  }
  if (inState9 && ((!g1&&!y1&&!y2&&!r2)||(!g2&&!y2&&!r2))) {
    nextState1 = 1;
    *((state9apValuesType*)&state1apValues) = state9apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 9: !0&!1&!3&!5 | !2&!3&!5*/
     state1history = state9history * 12 + 9;
  }
  if (inState20 && (!g1&&a1)) {
    nextState1 = 1;
    *((state20apValuesType*)&state1apValues) = state20apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 20: !0&6*/
     state1history = state20history * 12 + 10;
  }
  if (inState29 && (!g2&&a2)) {
    nextState1 = 1;
    *((state29apValuesType*)&state1apValues) = state29apValues;
     state1apValues.vals10 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 29: !2&7*/
     state1history = state29history * 12 + 11;
  }
  nextState20 = inState20 && (g1&&a1);
  if (inState19 && (a1)) {
    nextState20 = 1;
    *((state19apValuesType*)&state20apValues) = state19apValues;
     state20apValues.vals9 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 19: 6*/
     state20history = state19history * 2 + 0;
  }
  nextState29 = inState29 && (g2&&a2);
  if (inState28 && (a2)) {
    nextState29 = 1;
    *((state28apValuesType*)&state29apValues) = state28apValues;
     state29apValues.vals9 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 28: 7*/
     state29history = state28history * 2 + 0;
  }
  if (inState18 && (a1)) {
    nextState19 = 1;
    *((state18apValuesType*)&state19apValues) = state18apValues;
     state19apValues.vals8 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 18: 6*/
     state19history = state18history * 1 + 0;
  }
  if (inState27 && (a2)) {
    nextState28 = 1;
    *((state27apValuesType*)&state28apValues) = state27apValues;
     state28apValues.vals8 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 27: 7*/
     state28history = state27history * 1 + 0;
  }
  if (inState17 && (a1)) {
    nextState18 = 1;
    *((state17apValuesType*)&state18apValues) = state17apValues;
     state18apValues.vals7 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 17: 6*/
     state18history = state17history * 1 + 0;
  }
  if (inState26 && (a2)) {
    nextState27 = 1;
    *((state26apValuesType*)&state27apValues) = state26apValues;
     state27apValues.vals7 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 26: 7*/
     state27history = state26history * 1 + 0;
  }
  if (inState16 && (a1)) {
    nextState17 = 1;
    *((state16apValuesType*)&state17apValues) = state16apValues;
     state17apValues.vals6 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 16: 6*/
     state17history = state16history * 1 + 0;
  }
  if (inState25 && (a2)) {
    nextState26 = 1;
    *((state25apValuesType*)&state26apValues) = state25apValues;
     state26apValues.vals6 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 25: 7*/
     state26history = state25history * 1 + 0;
  }
  if (inState15 && (a1)) {
    nextState16 = 1;
    *((state15apValuesType*)&state16apValues) = state15apValues;
     state16apValues.vals5 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 15: 6*/
     state16history = state15history * 1 + 0;
  }
  if (inState24 && (a2)) {
    nextState25 = 1;
    *((state24apValuesType*)&state25apValues) = state24apValues;
     state25apValues.vals5 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 24: 7*/
     state25history = state24history * 1 + 0;
  }
  if (inState14 && (a1)) {
    nextState15 = 1;
    *((state14apValuesType*)&state15apValues) = state14apValues;
     state15apValues.vals4 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 14: 6*/
     state15history = state14history * 1 + 0;
  }
  if (inState23 && (a2)) {
    nextState24 = 1;
    *((state23apValuesType*)&state24apValues) = state23apValues;
     state24apValues.vals4 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 23: 7*/
     state24history = state23history * 1 + 0;
  }
  if (inState13 && (a1)) {
    nextState14 = 1;
    *((state13apValuesType*)&state14apValues) = state13apValues;
     state14apValues.vals3 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 13: 6*/
     state14history = state13history * 1 + 0;
  }
  if (inState22 && (a2)) {
    nextState23 = 1;
    *((state22apValuesType*)&state23apValues) = state22apValues;
     state23apValues.vals3 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 22: 7*/
     state23history = state22history * 1 + 0;
  }
  if (inState12 && (a1)) {
    nextState13 = 1;
    *((state12apValuesType*)&state13apValues) = state12apValues;
     state13apValues.vals2 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 12: 6*/
     state13history = state12history * 1 + 0;
  }
  if (inState21 && (a2)) {
    nextState22 = 1;
    *((state21apValuesType*)&state22apValues) = state21apValues;
     state22apValues.vals2 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 21: 7*/
     state22history = state21history * 1 + 0;
  }
  if (inState10 && (a1)) {
    nextState12 = 1;
    *((state10apValuesType*)&state12apValues) = state10apValues;
     state12apValues.vals1 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 10: 6*/
     state12history = state10history * 1 + 0;
  }
  if (inState11 && (a2)) {
    nextState21 = 1;
    *((state11apValuesType*)&state21apValues) = state11apValues;
     state21apValues.vals1 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 11: 7*/
     state21history = state11history * 1 + 0;
  }
  if (inState0 && (y1&&!g2&&!y2&&r1)) {
    nextState2 = 1;
    *((state0apValuesType*)&state2apValues) = state0apValues;
     state2apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: 1&!2&!3&4*/
     state2history = state0history * 1 + 0;
  }
  if (inState0 && (!g1&&!y1&&y2&&r2)) {
    nextState3 = 1;
    *((state0apValuesType*)&state3apValues) = state0apValues;
     state3apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&3&5*/
     state3history = state0history * 1 + 0;
  }
  if (inState0 && (y1&&!g2&&!y2&&!r1)) {
    nextState4 = 1;
    *((state0apValuesType*)&state4apValues) = state0apValues;
     state4apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: 1&!2&!3&!4*/
     state4history = state0history * 1 + 0;
  }
  if (inState0 && (!g1&&!y1&&y2&&!r2)) {
    nextState5 = 1;
    *((state0apValuesType*)&state5apValues) = state0apValues;
     state5apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&3&!5*/
     state5history = state0history * 1 + 0;
  }
  if (inState0 && ((g1&&!y1&&!g2&&!y2)||(g1&&!g2&&!y2&&!r1))) {
    nextState6 = 1;
    *((state0apValuesType*)&state6apValues) = state0apValues;
     state6apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: 0&!1&!2&!3 | 0&!2&!3&!4*/
     state6history = state0history * 1 + 0;
  }
  if (inState0 && ((!g1&&!y1&&r1)||(!y1&&!g2&&!y2&&r1))) {
    nextState7 = 1;
    *((state0apValuesType*)&state7apValues) = state0apValues;
     state7apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&4 | !1&!2&!3&4*/
     state7history = state0history * 1 + 0;
  }
  if (inState0 && ((!g1&&!y1&&g2&&!y2)||(!g1&&!y1&&g2&&!r2))) {
    nextState8 = 1;
    *((state0apValuesType*)&state8apValues) = state0apValues;
     state8apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&2&!3 | !0&!1&2&!5*/
     state8history = state0history * 1 + 0;
  }
  if (inState0 && ((!g1&&!y1&&!y2&&r2)||(!g2&&!y2&&r2))) {
    nextState9 = 1;
    *((state0apValuesType*)&state9apValues) = state0apValues;
     state9apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&!3&5 | !2&!3&5*/
     state9history = state0history * 1 + 0;
  }
  if (inState0 && ((!g1&&!y1&&a1)||(!g2&&!y2&&a1))) {
    nextState10 = 1;
    *((state0apValuesType*)&state10apValues) = state0apValues;
     state10apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&6 | !2&!3&6*/
     state10history = state0history * 1 + 0;
  }
  if (inState0 && ((!g1&&!y1&&a2)||(!g2&&!y2&&a2))) {
    nextState11 = 1;
    *((state0apValuesType*)&state11apValues) = state0apValues;
     state11apValues.vals0 = CONSTRUCTTRACEELEMENT;
     /* Transition from state 0: !0&!1&7 | !2&!3&7*/
     state11history = state0history * 1 + 0;
  }
  nextState0 = inState0 && ((!g1&&!y1)||(!g2&&!y2));
  inState1 = nextState1;
  inState20 = nextState20;
  inState29 = nextState29;
  inState19 = nextState19;
  inState28 = nextState28;
  inState18 = nextState18;
  inState27 = nextState27;
  inState17 = nextState17;
  inState26 = nextState26;
  inState16 = nextState16;
  inState25 = nextState25;
  inState15 = nextState15;
  inState24 = nextState24;
  inState14 = nextState14;
  inState23 = nextState23;
  inState13 = nextState13;
  inState22 = nextState22;
  inState12 = nextState12;
  inState21 = nextState21;
  inState2 = nextState2;
  inState3 = nextState3;
  inState4 = nextState4;
  inState5 = nextState5;
  inState6 = nextState6;
  inState7 = nextState7;
  inState8 = nextState8;
  inState9 = nextState9;
  inState10 = nextState10;
  inState11 = nextState11;
  inState0 = nextState0;
  return inState1;
}
#undef CONSTRUCTTRACEELEMENT

/* Function for obtaining trace information from stored values. Uses statically reserved space */char violatingTraceInformation[1209];
void buildViolatingTraceInformation() {
  int selector;
  uint32_t history = state1history;
  int state=1;
  int ptr=snprintf(violatingTraceInformation,1208,"Violating trace (before entering the rejecting state):\n");
  while ((state!=0)) {
    switch(state) {
      case 1:
        selector = (int[]){0,1,2,3,4,5,6,7,8,9,20,29}[history % 12];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals10 & (1 << 0))?1:0, (state1apValues.vals10 & (1 << 1))?1:0, (state1apValues.vals10 & (1 << 2))?1:0, (state1apValues.vals10 & (1 << 3))?1:0, (state1apValues.vals10 & (1 << 4))?1:0, (state1apValues.vals10 & (1 << 5))?1:0, (state1apValues.vals10 & (1 << 6))?1:0, (state1apValues.vals10 & (1 << 7))?1:0);
        history =  history / 12;
        state = selector;
        break;
      case 2:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 3:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 4:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 5:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 6:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 7:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 8:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 9:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 10:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 11:
        selector = (int[]){0}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals0 & (1 << 0))?1:0, (state1apValues.vals0 & (1 << 1))?1:0, (state1apValues.vals0 & (1 << 2))?1:0, (state1apValues.vals0 & (1 << 3))?1:0, (state1apValues.vals0 & (1 << 4))?1:0, (state1apValues.vals0 & (1 << 5))?1:0, (state1apValues.vals0 & (1 << 6))?1:0, (state1apValues.vals0 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 12:
        selector = (int[]){10}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals1 & (1 << 0))?1:0, (state1apValues.vals1 & (1 << 1))?1:0, (state1apValues.vals1 & (1 << 2))?1:0, (state1apValues.vals1 & (1 << 3))?1:0, (state1apValues.vals1 & (1 << 4))?1:0, (state1apValues.vals1 & (1 << 5))?1:0, (state1apValues.vals1 & (1 << 6))?1:0, (state1apValues.vals1 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 13:
        selector = (int[]){12}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals2 & (1 << 0))?1:0, (state1apValues.vals2 & (1 << 1))?1:0, (state1apValues.vals2 & (1 << 2))?1:0, (state1apValues.vals2 & (1 << 3))?1:0, (state1apValues.vals2 & (1 << 4))?1:0, (state1apValues.vals2 & (1 << 5))?1:0, (state1apValues.vals2 & (1 << 6))?1:0, (state1apValues.vals2 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 14:
        selector = (int[]){13}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals3 & (1 << 0))?1:0, (state1apValues.vals3 & (1 << 1))?1:0, (state1apValues.vals3 & (1 << 2))?1:0, (state1apValues.vals3 & (1 << 3))?1:0, (state1apValues.vals3 & (1 << 4))?1:0, (state1apValues.vals3 & (1 << 5))?1:0, (state1apValues.vals3 & (1 << 6))?1:0, (state1apValues.vals3 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 15:
        selector = (int[]){14}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals4 & (1 << 0))?1:0, (state1apValues.vals4 & (1 << 1))?1:0, (state1apValues.vals4 & (1 << 2))?1:0, (state1apValues.vals4 & (1 << 3))?1:0, (state1apValues.vals4 & (1 << 4))?1:0, (state1apValues.vals4 & (1 << 5))?1:0, (state1apValues.vals4 & (1 << 6))?1:0, (state1apValues.vals4 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 16:
        selector = (int[]){15}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals5 & (1 << 0))?1:0, (state1apValues.vals5 & (1 << 1))?1:0, (state1apValues.vals5 & (1 << 2))?1:0, (state1apValues.vals5 & (1 << 3))?1:0, (state1apValues.vals5 & (1 << 4))?1:0, (state1apValues.vals5 & (1 << 5))?1:0, (state1apValues.vals5 & (1 << 6))?1:0, (state1apValues.vals5 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 17:
        selector = (int[]){16}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals6 & (1 << 0))?1:0, (state1apValues.vals6 & (1 << 1))?1:0, (state1apValues.vals6 & (1 << 2))?1:0, (state1apValues.vals6 & (1 << 3))?1:0, (state1apValues.vals6 & (1 << 4))?1:0, (state1apValues.vals6 & (1 << 5))?1:0, (state1apValues.vals6 & (1 << 6))?1:0, (state1apValues.vals6 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 18:
        selector = (int[]){17}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals7 & (1 << 0))?1:0, (state1apValues.vals7 & (1 << 1))?1:0, (state1apValues.vals7 & (1 << 2))?1:0, (state1apValues.vals7 & (1 << 3))?1:0, (state1apValues.vals7 & (1 << 4))?1:0, (state1apValues.vals7 & (1 << 5))?1:0, (state1apValues.vals7 & (1 << 6))?1:0, (state1apValues.vals7 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 19:
        selector = (int[]){18}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals8 & (1 << 0))?1:0, (state1apValues.vals8 & (1 << 1))?1:0, (state1apValues.vals8 & (1 << 2))?1:0, (state1apValues.vals8 & (1 << 3))?1:0, (state1apValues.vals8 & (1 << 4))?1:0, (state1apValues.vals8 & (1 << 5))?1:0, (state1apValues.vals8 & (1 << 6))?1:0, (state1apValues.vals8 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 20:
        selector = (int[]){19,20}[history % 2];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals9 & (1 << 0))?1:0, (state1apValues.vals9 & (1 << 1))?1:0, (state1apValues.vals9 & (1 << 2))?1:0, (state1apValues.vals9 & (1 << 3))?1:0, (state1apValues.vals9 & (1 << 4))?1:0, (state1apValues.vals9 & (1 << 5))?1:0, (state1apValues.vals9 & (1 << 6))?1:0, (state1apValues.vals9 & (1 << 7))?1:0);
        history =  history / 2;
        state = selector;
        break;
      case 21:
        selector = (int[]){11}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals1 & (1 << 0))?1:0, (state1apValues.vals1 & (1 << 1))?1:0, (state1apValues.vals1 & (1 << 2))?1:0, (state1apValues.vals1 & (1 << 3))?1:0, (state1apValues.vals1 & (1 << 4))?1:0, (state1apValues.vals1 & (1 << 5))?1:0, (state1apValues.vals1 & (1 << 6))?1:0, (state1apValues.vals1 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 22:
        selector = (int[]){21}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals2 & (1 << 0))?1:0, (state1apValues.vals2 & (1 << 1))?1:0, (state1apValues.vals2 & (1 << 2))?1:0, (state1apValues.vals2 & (1 << 3))?1:0, (state1apValues.vals2 & (1 << 4))?1:0, (state1apValues.vals2 & (1 << 5))?1:0, (state1apValues.vals2 & (1 << 6))?1:0, (state1apValues.vals2 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 23:
        selector = (int[]){22}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals3 & (1 << 0))?1:0, (state1apValues.vals3 & (1 << 1))?1:0, (state1apValues.vals3 & (1 << 2))?1:0, (state1apValues.vals3 & (1 << 3))?1:0, (state1apValues.vals3 & (1 << 4))?1:0, (state1apValues.vals3 & (1 << 5))?1:0, (state1apValues.vals3 & (1 << 6))?1:0, (state1apValues.vals3 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 24:
        selector = (int[]){23}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals4 & (1 << 0))?1:0, (state1apValues.vals4 & (1 << 1))?1:0, (state1apValues.vals4 & (1 << 2))?1:0, (state1apValues.vals4 & (1 << 3))?1:0, (state1apValues.vals4 & (1 << 4))?1:0, (state1apValues.vals4 & (1 << 5))?1:0, (state1apValues.vals4 & (1 << 6))?1:0, (state1apValues.vals4 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 25:
        selector = (int[]){24}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals5 & (1 << 0))?1:0, (state1apValues.vals5 & (1 << 1))?1:0, (state1apValues.vals5 & (1 << 2))?1:0, (state1apValues.vals5 & (1 << 3))?1:0, (state1apValues.vals5 & (1 << 4))?1:0, (state1apValues.vals5 & (1 << 5))?1:0, (state1apValues.vals5 & (1 << 6))?1:0, (state1apValues.vals5 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 26:
        selector = (int[]){25}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals6 & (1 << 0))?1:0, (state1apValues.vals6 & (1 << 1))?1:0, (state1apValues.vals6 & (1 << 2))?1:0, (state1apValues.vals6 & (1 << 3))?1:0, (state1apValues.vals6 & (1 << 4))?1:0, (state1apValues.vals6 & (1 << 5))?1:0, (state1apValues.vals6 & (1 << 6))?1:0, (state1apValues.vals6 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 27:
        selector = (int[]){26}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals7 & (1 << 0))?1:0, (state1apValues.vals7 & (1 << 1))?1:0, (state1apValues.vals7 & (1 << 2))?1:0, (state1apValues.vals7 & (1 << 3))?1:0, (state1apValues.vals7 & (1 << 4))?1:0, (state1apValues.vals7 & (1 << 5))?1:0, (state1apValues.vals7 & (1 << 6))?1:0, (state1apValues.vals7 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 28:
        selector = (int[]){27}[history % 1];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals8 & (1 << 0))?1:0, (state1apValues.vals8 & (1 << 1))?1:0, (state1apValues.vals8 & (1 << 2))?1:0, (state1apValues.vals8 & (1 << 3))?1:0, (state1apValues.vals8 & (1 << 4))?1:0, (state1apValues.vals8 & (1 << 5))?1:0, (state1apValues.vals8 & (1 << 6))?1:0, (state1apValues.vals8 & (1 << 7))?1:0);
        history =  history / 1;
        state = selector;
        break;
      case 29:
        selector = (int[]){28,29}[history % 2];
        ptr += snprintf(violatingTraceInformation+ptr,1208-ptr,"- Transition from state %d with prop. values: r1=%d, y1=%d, g1=%d, r2=%d, y2=%d, g2=%d, a1=%d, a2=%d\n",selector, (state1apValues.vals9 & (1 << 0))?1:0, (state1apValues.vals9 & (1 << 1))?1:0, (state1apValues.vals9 & (1 << 2))?1:0, (state1apValues.vals9 & (1 << 3))?1:0, (state1apValues.vals9 & (1 << 4))?1:0, (state1apValues.vals9 & (1 << 5))?1:0, (state1apValues.vals9 & (1 << 6))?1:0, (state1apValues.vals9 & (1 << 7))?1:0);
        history =  history / 2;
        state = selector;
        break;
    }
  }
}
