#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"

void 
efsm_state_timer_operation (wheel_timer_elem_t **timer, efsm_state_timer_op_t op) {

}

static void
efsm_state_exit (efsm_t *efsm) {

    if (efsm->state_print) {
        printf ("Exiting state %s\n", efsm->state_print(efsm->current_state->id));
    }
    else {
        printf ("Exiting state %d\n", efsm->current_state->id);
    }

    /* Executethe default action to  be performed on state exit */
    if (efsm->current_state->state_timers) {
        if (efsm->current_state->state_timers->expiry_timer) {
            efsm_state_timer_operation (&efsm->current_state->state_timers->expiry_timer, EFSM_STATE_TIMER_STOP);
        }
        if (efsm->current_state->state_timers->other_timer) {
            efsm_state_timer_operation (&efsm->current_state->state_timers->other_timer, EFSM_STATE_TIMER_STOP);
        }
    }

    /* Execute theuser defined action to be performed on state exit */
    if (efsm->current_state->exit_fn) {
        efsm->current_state->exit_fn(efsm);
    }

    efsm->current_state = NULL;
}

static void
efsm_state_enter (efsm_t *efsm, efsm_state_t *state) {

    assert(!efsm->current_state);

    if (efsm->state_print) {
        printf ("Entering state %s\n", efsm->state_print(state->id));
    }
    else {
        printf ("Entering state %d\n", state->id);
    }

    /* Execute theuser defined action to be performed on state entry */
    if (state->entry_fn) {
        state->entry_fn(efsm);
    }

    efsm->current_state = state;
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

    efsm_state_t *next_state;
    action_fn action_fn_cbk;

    if (!efsm->current_state) {
        efsm->current_state = efsm->initial_state;
    }

   efsm->old_state = efsm->current_state ;
   action_fn_cbk = efsm->current_state->trans_table.tte_array[0][event].action_fn_cbk;
   next_state = efsm->old_state->trans_table.tte_array[0][event].next_state;
   if (action_fn_cbk) {
        action_fn_cbk(efsm);
   }
   if (next_state) {
        efsm_state_exit (efsm);
        efsm_state_enter (efsm, efsm->old_state->trans_table.tte_array[0][event].next_state);
    }
}