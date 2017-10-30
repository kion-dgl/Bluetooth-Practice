#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char *argv[]) {

	struct sockaddr_rc addr = { 0 };
	int s, status;
	char dest[18] = "58:BD:A3:4A:C6:1E";

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba(dest, &addr.rc_bdaddr);

	status = connect(s, (struct sockaddr*)&addr, sizeof(addr));

	if(status < 0) {
		
		fprintf(stderr, "Unable to connect");

	}else if(status == 0) {
		
		printf("Connected!!");

	}

	close(s);
	return 0;

}
