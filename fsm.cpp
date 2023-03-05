#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"

void 
efsm_state_timer_operation (efsm_state_t *state, efsm_state_timer_op_t op) {

}

static bool
efsm_state_exit (efsm_t *efsm) {

    /* Executethe default action to  be performed on state exit */
    if (efsm->current_state->expiry_timer) {
        efsm_state_timer_operation (efsm->current_state, EFSM_STATE_TIMER_STOP);
    }

    /* Execute theuser defined action to be performed on state exit */
    if (efsm->current_state->exit_fn) {
        efsm->current_state->exit_fn(efsm);
    }

    efsm->current_state = NULL;
    return true;
}

static bool
efsm_state_enter (efsm_t *efsm, efsm_state_t *state) {

    /* Executethe default action to  be performed on state entry */
    if (state->expiry_timer) {
        efsm_state_timer_operation (state, EFSM_STATE_TIMER_START);
    }

    /* Execute theuser defined action to be performed on state entry */
    if (state->entry_fn) {
        state->entry_fn(efsm);
    }

    efsm->current_state = state;
    return true;
}

efsm_state_t *
efsm_create_new_state (state_id id, 
                                        bool is_final,
                                        bool (*entry_fn)(efsm_t *),
                                        bool (*exit_fn)(efsm_t *), 
                                        transition_table_t *trans_table) {

    efsm_state_t *state = (efsm_state_t *)calloc (1, sizeof(efsm_state_t));
    state->id = id;
    state->final_state = is_final;
    state->entry_fn = entry_fn;
    state->exit_fn = exit_fn;
    state->trans_table.tte_array = NULL;
    return state;
}

efsm_t *
efsm_new (void *user_data) {

    efsm_t *efsm = (efsm_t *)calloc (1, sizeof(efsm_t));
    return efsm;
}

void
efsm_execute (efsm_t *efsm, int event) {

    bool rc;
    efsm_state_t *current_state;

    if (!efsm->current_state) {
        efsm->current_state = efsm->initial_state;
    }

    if (efsm->current_state->trans_table.tte_array[0][event].action_fn_cbk) {
        rc = efsm->current_state->trans_table.tte_array[0][event].action_fn_cbk(efsm);
        if (!rc) return;
        current_state = efsm->current_state;
        efsm_state_exit (efsm);
        efsm_state_enter (efsm, current_state->trans_table.tte_array[0][event].next_state);
    }

}