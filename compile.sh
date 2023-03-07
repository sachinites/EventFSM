rm *o
rm *exe
g++ -g -c fsm.cpp -o fsm.o
g++ -g -c dhcp_client_pkt.cpp -o dhcp_client_pkt.o
g++ -g -c dhcp_client_fsm.cpp -o dhcp_client_fsm.o
g++ -g fsm.o dhcp_client_pkt.o dhcp_client_fsm.o -o dhcp_client_fsm.exe