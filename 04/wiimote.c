#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#define MAX_WIIMOTES = 4;

int wiiScanRemotes();

int main(int argc, char *argv[]) {


	wiiScanRemotes();
	
	return 0;

}

int wiiScanRemotes() {

	int deviceId;
	int deviceSocket;
	int foundDevices;
	int foundWiimotes;
	
	int scanTimeout = 8;
	int scanDeviceLimit = 10;
	
	char btAddress[19] = {0};
	char btDeviceName[255] = {0};

	deviceId = hci_get_route(NULL);
	if(deviceId < 0) {
		perror("Could not get device");
		return -1;
	}

	deviceSocket = hci_open_dev(deviceId);
	if(deviceSocket < 0) {
		perror("Could not open bluetooth socket");
		return -1;
	}
	
	inquiry_info* scanInfoArray = (inquiry_info*)malloc(scanDeviceLimit * sizeof(inquiry_info));

	printf("Scanning for bluetooth devices...\n");

	foundDevices = hci_inquiry(deviceId, 
		scanTimeout, 
		scanDeviceLimit, 
		NULL, 
		&scanInfoArray, 
		IREQ_CACHE_FLUSH);

	if(foundDevices < 0) {
		perror("Bluetooth scan error");
		return -1;
	}

	if(foundDevices == 0){
		fprintf(stderr, "No bluetooth devices found\n");
		return -1;
	}

	printf("Found (%d) devices.\n", foundDevices);

	int i;
	for(i = 0; i < foundDevices; i++) {
		
		memset(btDeviceName, 0, sizeof(btDeviceName));

		ba2str(&scanInfoArray[i].bdaddr, btAddress);
	
		if(hci_read_remote_name(deviceSocket, 
			&scanInfoArray[i].bdaddr, 
			sizeof(btDeviceName), 
			btDeviceName, 0) < 0){
			strcpy(btDeviceName, "[Unkown]");
		}

		printf("%d) %s %s\n", i, btAddress, btDeviceName);
		printf("%d %d %d\n", 
			scanInfoArray[i].dev_class[0], 
			scanInfoArray[i].dev_class[1], 
			scanInfoArray[i].dev_class[2]);
	}

	return;

	/*
	 * Connect to device
	 */
	
	int status;
	struct sockaddr_l2 addr;
	memset(&addr, 0, sizeof(addr));
	
	str2ba( btAddress, &addr.l2_bdaddr );
	int inSocket, outSocket;

	outSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

	if(outSocket == -1){
		perror("Could not open out socket");
		return -1;
	}

	addr.l2_psm = htobs(0x11);

	status = connect(outSocket, (struct sockaddr*)&addr, sizeof(addr));

	if(status < 0){
		perror("Could not connect to output socket");
		return -1;
	}


}
