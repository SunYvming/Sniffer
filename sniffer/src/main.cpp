#include <iostream>
#include <pcap.h>

using namespace std;

int main(int argc, char *argv[])
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];

	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
        cout << "Couldn't find default device: " << errbuf << endl;
		return(2);
	}
    cout << "Device: " << dev << endl;

	pcap_t *handle;
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL){
		cout << "Couldn't open device " << dev << ": " << errbuf << endl;
		return(2);
	}
	return(0);
}