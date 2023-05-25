#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>


int get_stat(long long int* nonidle, long long int* idle)
{
	int ch;
	FILE* tmp_file;
	int i=0;
	char s_nonidle[20], s_idle[20];
	
	if((tmp_file = popen("cat /proc/stat | awk 'NR==1' | awk -F \" \" '{print $5+$6,$2+$3+$4+$7+$8+$9 }'","r")) == NULL ) {
      perror( "popen error" );
      return 1;
    }
	
	while( (ch=fgetc(tmp_file)) != ' ')
		s_idle[i++]=ch;
	s_idle[i]='\0'; 
    i=0;
	while( (ch=fgetc(tmp_file)) != EOF)
		s_nonidle[i++]=ch;
	s_nonidle[i]='\0';
	
	*nonidle=atoll(s_nonidle);
	*idle=atoll(s_idle);
	pclose(tmp_file);
	
	return 0;
}	

float get_cpu_usage()
{
	long long int nonidle1, idle1, nonidle2, idle2, total1, total2, totald, idled;
	
	get_stat(&nonidle1, &idle1);
	sleep(1);
	get_stat(&nonidle2, &idle2);
	
	total1 = nonidle1+idle1;
	total2 = nonidle2+idle2;
	totald = total2 - total1;
	idled = idle2 - idle1;
	if (totald-idled==0) return 0;
	return(((float)totald - (float)idled) * 100 / (float)totald);
}

int get_cpu_name(char* cpu_name)
{
	int ch;
	FILE* tmp_file;
	int i=0;
	
	if((tmp_file = popen("cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk -F \": \" '{print $2}'", "r")) == NULL ) {
      perror( "popen error" );
      exit(EXIT_FAILURE);
    }
	
	while( (ch=fgetc(tmp_file)) != EOF && i<=98)
		cpu_name[i++]=ch;
	cpu_name[i-1]='\0'; 
    pclose(tmp_file);
	return 0;
}

int main (int argc, char * argv[])
{
	int server_fd, new_socket; long valread;
	int port;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
    char cpuname[100], hostname[100];
	char usage[10];
	
	if (argc != 2){
		printf("Usage: ./hinfosvc [port]\n");
		return 1;
	}
	port = atoi(argv[1]);
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

	if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) 
	{ 
		perror("bind failed"); 
		return 1; 
	}

	if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

	while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        if (memcmp("GET /hostname ", buffer, 14) == 0)
		{
			gethostname(hostname, 100);
			write(new_socket , "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n" , strlen("HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"));
			write(new_socket , hostname , strlen(hostname));
		}
		else
		{
			if (memcmp("GET /cpu-name ", buffer, 14) == 0)
			{
				get_cpu_name(cpuname);
				write(new_socket , "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n" , strlen("HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"));
				write(new_socket , cpuname , strlen(cpuname));
			}
			else
			{
				if (memcmp("GET /load ", buffer, 10) == 0)
				{
					float f_u=get_cpu_usage();
					f_u=round(f_u);
					gcvt(f_u,10,usage);
					strcat(usage,"%");
					write(new_socket , "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n" , strlen("HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"));
					write(new_socket , usage , strlen(usage));
				}
				else write(new_socket , "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\nBad request" , strlen("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\nBad request"));
			}
		}
		close(new_socket);
    }
	return 0;
}
