#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"

void 
efsm_state_timer_operation (efsm_state_t *state, efsm_state_timer_op_t op) {

}

static void
efsm_state_exit (efsm_t *efsm, efsm_state_t *state) {

    if (state->expiry_timer) {
        efsm_state_timer_operation (state, EFSM_STATE_TIMER_STOP);
    }
    efsm->current_state = NULL;
}

static void
efsm_state_enter (efsm_t *efsm, efsm_state_t *state) {

    if (state->expiry_timer) {
        efsm_state_timer_operation (state, EFSM_STATE_TIMER_START);
    }
    efsm->current_state = state;
}

efsm_state_t *
efsm_create_new_state (state_id id, 
                                        bool is_final,
                                        void (*entry_fn)(efsm_t *),
                                        void (*exit_fn)(efsm_t *), 
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

    if (!efsm->current_state) {
        efsm->current_state = efsm->initial_state;
    }

    if (efsm->current_state->trans_table.tte_array[event]->action_fn_cbk) {
        efsm->current_state->trans_table.tte_array[event]->action_fn_cbk(efsm);
        efsm_state_exit (efsm, efsm->current_state);
        efsm_state_enter (efsm, efsm->current_state->trans_table.tte_array[event]->next_state);
    }

}