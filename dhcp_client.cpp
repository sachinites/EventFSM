#include <stdlib.h>
#include "fsm.h"

/*sample Event FSM example */
typedef enum dhcp_events_ {

    /* Event to initialize DHCP client FSM*/
    DHCP_CLIENT_EVENT_INIT,
    /* Event to trigger sending of discover msg*/
    DHCP_CLIENT_EVENT_SEND_DISCOVER,
    /* Event when timeout happens*/
    DHCP_CLIENT_EVENT_ST_DISCOVER_TIMEOUT,
    /* Event when client recv Offer Msg*/
    DHCP_CLIENT_EVENT_RECV_OFFER,
    /* Event to trigger sending of  req msg*/
    DHCP_CLIENT_EVENT_SEND_REQUEST,
    /* Event when timeout happens*/
    DHCP_CLIENT_EVENT_ST_REQUEST_TIMEOUT,
     /* Event when client recv ACK Msg*/
    DHCP_CLIENT_EVENT_RECV_ACK,
    DHCP_CLIENT_EVENT_MAX
} dhcp_event_t ;

typedef enum dhcp_client_state_ {

    DHCP_CLIENT_ST_INIT,
    DHCP_CLIENT_ST_DISCOVERING,
    DHCP_CLIENT_ST_REQUESTING,
    DHCP_CLIENT_ST_CONFIRMED,
    DHCP_CLIENT_ST_MAX
} dhcp_client_state_t;

const char *
dhcp_client_state_tostring(dhcp_client_state_t state_enum)
{
    switch (state_enum)
    {
    case DHCP_CLIENT_ST_INIT:
        return "DHCP_CLIENT_ST_INIT";
    case DHCP_CLIENT_ST_DISCOVERING:
        return "DHCP_CLIENT_ST_DISCOVERING";
    case DHCP_CLIENT_ST_REQUESTING:
        return "DHCP_CLIENT_ST_REQUESTING";
    case DHCP_CLIENT_ST_CONFIRMED:
        return "DHCP_CLIENT_ST_CONFIRMED";
    case DHCP_CLIENT_ST_MAX:
        return NULL;
    default:;
    }
    return NULL;
}

static void dhcp_client_default_state_entry_fn(efsm_t *efsm) {}
static void dhcp_client_default_state_exit_fn(efsm_t *efsm){};


/* DHCP States*/
static efsm_state_t *dhcp_client_states[] = {

    efsm_create_new_state(DHCP_CLIENT_ST_INIT, false, dhcp_client_default_state_entry_fn, dhcp_client_default_state_exit_fn, NULL),

    efsm_create_new_state(DHCP_CLIENT_ST_DISCOVERING, false, dhcp_client_default_state_entry_fn, dhcp_client_default_state_exit_fn, NULL),

    efsm_create_new_state(DHCP_CLIENT_ST_REQUESTING, false, dhcp_client_default_state_entry_fn, dhcp_client_default_state_exit_fn, NULL),

    efsm_create_new_state(DHCP_CLIENT_ST_CONFIRMED, true, dhcp_client_default_state_entry_fn, dhcp_client_default_state_exit_fn, NULL)
};

static bool
dhcp_client_fn_process_offer(efsm_t *efsm) {

    return true;
}
#define FSM_ACTION_DHCP_PROCESS_OFFER   ((action_fn)dhcp_client_fn_process_offer)

static bool
dhcp_client_fn_process_ack(efsm_t *efsm) {

    return true;
}
#define FSM_ACTION_DHCP_PROCESS_ACK   ((action_fn)dhcp_client_fn_process_ack)

static bool
dhcp_client_fn_send_discover(efsm_t *efsm) {

    return true;
}
#define FSM_ACTION_DHCP_SEND_DISCOVER   ((action_fn)dhcp_client_fn_send_discover)

static bool
dhcp_client_fn_send_request(efsm_t *efsm) {

    return true;
}
#define FSM_ACTION_DHCP_SEND_REQUEST   ((action_fn)dhcp_client_fn_send_request)

static bool 
dhcp_client_default_state_cancel_fn(efsm_t *efsm){ 
    
    return true ; 
}
#define FSM_ACTION_DHCP_ABORT_STATE   ((action_fn)dhcp_client_default_state_cancel_fn)


/* DHCP client transition table */
static transition_table_entry_t trans_table_dhcp_state_init[] = {

                // DHCP_CLIENT_EVENT_INIT
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                // DHCP_CLIENT_EVENT_SEND_DISCOVER
                STATE_EVENT_TT_ENTRY(
                    FSM_ACTION_DHCP_SEND_DISCOVER, dhcp_client_states[DHCP_CLIENT_ST_DISCOVERING]),
                // DHCP_CLIENT_EVENT_ST_DISCOVER_TIMEOUT
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),
                // DHCP_CLIENT_EVENT_RECV_OFFER
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),            
                // DHCP_CLIENT_EVENT_SEND_REQUEST
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),
                // DHCP_CLIENT_EVENT_ST_REQUEST_TIMEOUT
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),
                // DHCP_CLIENT_EVENT_RECV_ACK
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),

                /* Extras*/
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION)
};

static transition_table_entry_t trans_table_dhcp_state_discovering[] = {

                // DHCP_CLIENT_EVENT_INIT
                STATE_EVENT_TT_ENTRY(
                                            FSM_ACTION_DHCP_ABORT_STATE, 
                                            dhcp_client_states[DHCP_CLIENT_EVENT_INIT]),

                // DHCP_CLIENT_EVENT_SEND_DISCOVER
                STATE_EVENT_TT_ENTRY(
                                            FSM_NO_ACTION,
                                            FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_ST_DISCOVER_TIMEOUT
                STATE_EVENT_TT_ENTRY(
                        FSM_ACTION_DHCP_ABORT_STATE,
                        dhcp_client_states[DHCP_CLIENT_EVENT_INIT]),

                // DHCP_CLIENT_EVENT_RECV_OFFER
                STATE_EVENT_TT_ENTRY(FSM_ACTION_DHCP_PROCESS_OFFER,
                        dhcp_client_states[DHCP_CLIENT_ST_REQUESTING]),

                // DHCP_CLIENT_EVENT_SEND_REQUEST
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),


                // DHCP_CLIENT_EVENT_ST_REQUEST_TIMEOUT
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),
                // DHCP_CLIENT_EVENT_RECV_ACK
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),

                /* Extras*/
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION)
};

static transition_table_entry_t trans_table_dhcp_state_requesting[] = {

                // DHCP_CLIENT_EVENT_INIT
                STATE_EVENT_TT_ENTRY(
                                            FSM_ACTION_DHCP_ABORT_STATE, 
                                            dhcp_client_states[DHCP_CLIENT_EVENT_INIT]),

                // DHCP_CLIENT_EVENT_SEND_DISCOVER
                STATE_EVENT_TT_ENTRY(
                                            FSM_NO_ACTION,
                                            FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_ST_DISCOVER_TIMEOUT
                STATE_EVENT_TT_ENTRY(
                        FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_RECV_OFFER
                STATE_EVENT_TT_ENTRY(  
                        FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_SEND_REQUEST
                STATE_EVENT_TT_ENTRY(FSM_ACTION_DHCP_SEND_REQUEST, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_ST_REQUEST_TIMEOUT
                STATE_EVENT_TT_ENTRY(
                        FSM_ACTION_DHCP_ABORT_STATE, dhcp_client_states[DHCP_CLIENT_EVENT_INIT]),

                // DHCP_CLIENT_EVENT_RECV_ACK
                STATE_EVENT_TT_ENTRY(
                    FSM_ACTION_DHCP_PROCESS_ACK, dhcp_client_states[DHCP_CLIENT_ST_CONFIRMED]),

                /* Extras*/
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION)
};

static transition_table_entry_t trans_table_dhcp_state_confirmed[] = {

                // DHCP_CLIENT_EVENT_INIT
                STATE_EVENT_TT_ENTRY(
                                            FSM_ACTION_DHCP_ABORT_STATE, 
                                            dhcp_client_states[DHCP_CLIENT_EVENT_INIT]),

                // DHCP_CLIENT_EVENT_SEND_DISCOVER
                STATE_EVENT_TT_ENTRY(
                                            FSM_NO_ACTION,
                                            FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_ST_DISCOVER_TIMEOUT
                STATE_EVENT_TT_ENTRY(
                        FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_RECV_OFFER
                STATE_EVENT_TT_ENTRY(
                        FSM_NO_ACTION,
                        FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_SEND_REQUEST
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_ST_REQUEST_TIMEOUT
                STATE_EVENT_TT_ENTRY(FSM_INVAID_ACTION, FSM_NO_STATE_TRANSITION),

                // DHCP_CLIENT_EVENT_RECV_ACK
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),

                /* Extras*/
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION),
                STATE_EVENT_TT_ENTRY(FSM_NO_ACTION, FSM_NO_STATE_TRANSITION)
};

efsm_t *dhcp_client_new_efsm();

efsm_t *
dhcp_client_new_efsm() {

    dhcp_client_states[DHCP_CLIENT_ST_INIT]->trans_table.tte_array = 
        &trans_table_dhcp_state_init;
    dhcp_client_states[DHCP_CLIENT_ST_DISCOVERING]->trans_table.tte_array = 
        &trans_table_dhcp_state_discovering;
    dhcp_client_states[DHCP_CLIENT_ST_REQUESTING]->trans_table.tte_array = 
        &trans_table_dhcp_state_requesting;
    dhcp_client_states[DHCP_CLIENT_ST_CONFIRMED]->trans_table.tte_array = 
        &trans_table_dhcp_state_confirmed;

    efsm_t *efsm = efsm_new(NULL);
    efsm->initial_state = dhcp_client_states[DHCP_CLIENT_ST_INIT];
    return efsm;
}

int main(int argc, char **argv) {

    efsm_t *efsm = dhcp_client_new_efsm();
    efsm_execute(efsm, DHCP_CLIENT_EVENT_SEND_DISCOVER);
    return 0;
}