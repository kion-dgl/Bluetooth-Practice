#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>


#define BT_ADDRESS "58:BD:A3:4A:C6:1E"

int main(int argc, char *argv[]) {

	struct sockaddr_l2 addr = { 0 };
	int outSocket, status;
	
	outSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	
	if(outSocket < 2){
		fprintf(stderr, "Could not open out socket\n");
		return 1;
	}

	addr.l2_family = AF_BLUETOOTH;
	addr.l2_psm = htobs(17);
	str2ba(BT_ADDRESS, &addr.l2_bdaddr);
	
	printf("Attempting to connect...\n");
	status = connect(outSocket, (struct sockaddr*)&addr, sizeof(addr));

	if(status < 0){
		fprintf(stderr, "Could not connect to output\n");
		return 1;
	}

	if(status == 0){
		printf("Successfulll!!\n");
	}

	close(outSocket);
	return 0;

}
