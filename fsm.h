#pragma once
#include <cstdint>

#define DIE return NULL
#define RETURN_STATE(X) return (fsm::state_t) X
#define AS_STATE(X) ((fsm::state_t) X)
#define STATE_EQ(X,Y) (AS_STATE(X) == AS_STATE(Y))

namespace fsm {
    typedef uintptr_t (*state_t)();

    state_t run(state_t (*init_state)()) {

        state_t state     = (state_t) init_state;
        state_t tmp_state = state;

        for(;;) {
            tmp_state = (state_t) state();
            
            if (!tmp_state) break;
            else state = tmp_state;
        }

        return state;
    }
}
