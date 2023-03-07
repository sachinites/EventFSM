#ifndef __E_FSM__
#define __E_FSM__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct wheel_timer_ wheel_timer_t;
typedef struct wheel_timer_elem_ wheel_timer_elem_t;
typedef uint8_t state_id;
typedef uint8_t fsm_event_id;

#define TT_TABLE_MAX_SIZE   12
#define FSM_STATE_EVENT_TIMER_EXPIRY (TT_TABLE_MAX_SIZE -1)

typedef struct efsm_state_ efsm_state_t;
typedef struct efsm_ efsm_t;

typedef bool (*action_fn)(efsm_t *);
#define FSM_NO_ACTION ((action_fn)NULL)
#define FSM_NO_STATE_TRANSITION ((efsm_state_t *)NULL)
#define STATE_EVENT_TT_ENTRY(action_fn, next_state) {action_fn, next_state}

static bool 
fsm_invalid_action(efsm_t *efsm) {
    
    assert(0);
    return true;
}

#define FSM_INVAID_ACTION   ((action_fn)fsm_invalid_action)

typedef struct transition_table_entry_ {

    action_fn action_fn_cbk;
    efsm_state_t *next_state;
}  transition_table_entry_t; 

typedef struct transition_table_ {

    transition_table_entry_t (*tte_array)[TT_TABLE_MAX_SIZE];

} transition_table_t;

typedef struct efsm_state_timer_ {

    wheel_timer_elem_t *expiry_timer;
    wheel_timer_elem_t *other_timer;
} efsm_state_timer_t;

struct efsm_state_ {

    state_id id;
    bool final_state;
    bool (*entry_fn)(efsm_t *);
    bool (*exit_fn)(efsm_t *);
    efsm_state_timer_t *state_timers;
    transition_table_t trans_table;
};

struct efsm_ {

    efsm_state_t *initial_state;
    efsm_state_t *old_state;
    efsm_state_t *current_state;

    void *user_data;
    wheel_timer_t *wt;

    /* Helper Callbacks for logging */
    const char * (*state_print)(state_id);
    const char *(*event_print)(int event);
};

/* APIs */
efsm_t *efsm_new (void *user_data);
void efsm_destroy (efsm_t *fsm);

efsm_state_t *
efsm_create_new_state (state_id id, 
                                        bool is_final,
                                        bool (*entry_fn)(efsm_t *),
                                        bool (*exit_fn)(efsm_t *), 
                                        transition_table_t *trans_table);

typedef enum efsm_state_timer_op_ {

    EFSM_STATE_TIMER_START,
    EFSM_STATE_TIMER_STOP,
    EFSM_STATE_TIMER_RESTART,
    EFSM_STATE_TIMER_DESTROY
} efsm_state_timer_op_t;

void 
efsm_state_timer_operation (wheel_timer_elem_t **timer, efsm_state_timer_op_t op);

void
efsm_execute (efsm_t *efsm, int event);

#endif 