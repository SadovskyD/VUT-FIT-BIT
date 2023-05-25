/* 
   VUT FIT IPP 2022
   Sniffer paketů
   Dmytro Sadovskyi
   xsadov06
*/

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <pcap.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>

#define SIZE_ETHERNET 14


using namespace std;

class Interface
{
    public:
    char *device;
    bpf_u_int32 net_addr;
    bpf_u_int32 mask;
    pcap_t* descriptor;
    bpf_program  bpf;

    void open(const char* dev, bool tcp, bool udp, bool arp, bool icmp, string port)
    {
        char errbuf[PCAP_ERRBUF_SIZE];
        
        device = (char*)dev;

        if ((descriptor = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf)) == NULL)
        {
            printf("%s\n", errbuf);
            exit(1);
        }

        string fltr = get_filter(tcp, udp, arp, icmp, port);

        if (pcap_compile(descriptor, &bpf, (char *) fltr.c_str(), 0, mask))
        {
            printf("%s\n", pcap_geterr(descriptor));
            exit(1);
        }

        if (pcap_setfilter(descriptor, &bpf) < 0)
        {
            printf("%s\n", pcap_geterr(descriptor));
            exit(1);
        }
    }

    void sniff_packets(int count)
    {
        int link_type;
        if ((link_type = pcap_datalink(descriptor)) < 0)
        {
            printf("%s\n", pcap_geterr(descriptor));
            exit(1);
        }
        if (link_type != DLT_EN10MB)
        {
            printf("Device doesn't provide Ethernet headers - not supported\n");
	        exit(1);
        }
        if (pcap_loop(descriptor, count, (pcap_handler) get_packet, 0) < 0) {
            printf("%s\n", pcap_geterr(descriptor));
            exit(1);
        }
    }

    void clear()
    {
        struct pcap_stat stats;

        if (pcap_stats(descriptor, &stats) >= 0) {
            printf("%d packets received\n", stats.ps_recv);
            printf("%d packets dropped\n", stats.ps_drop);
        }
        pcap_close(descriptor);
        pcap_freecode(&bpf);
    }

    private:
    string get_filter(bool tcp, bool udp, bool arp, bool icmp, string port)
    {
    	bool multi=false;
        string filter = "";
        if (tcp)
        {
            multi = true;
        	filter += "(tcp";
        	if (stoi(port) != 0) filter = filter + " port " + port;
        	filter += ") ";
        }
        if (udp)
        {
        	if (multi) filter +=" or ";
        	filter += "(udp";
        	if (stoi(port) != 0) filter = filter + " port " + port;
			filter += ") ";
        	multi = true;
        }
        if (arp)
        {
        	if (multi) filter +=" or ";
        	filter += "arp ";
			multi = true;
        }
        if (icmp)
        {	
        	if (multi) filter +=" or ";
        	filter += " icmp or icmp6";
        }
        return filter;
    }

    static void get_packet(u_char *user, pcap_pkthdr *packethdr, u_char *packetdata)
    {
    	ether_header* ether_d = (ether_header*)(packetdata);
        ip* ip_d;
        ip6_hdr* ip6_d;
        tcphdr* tcp_d;
        udphdr* udp_d;
        ether_arp* arp_d;
        ip6_hdr* ipv6_d;
       	
        u_int size_ip;
        u_int size_tcp;
        u_char *btp;
        string src_addr;
        string dst_addr;
        string dmac;
        string smac;
        ether_addr dst_mac, src_mac;
        
        tm * time_s = gmtime(&packethdr->ts.tv_sec);
        string corr_t = timetostr(time_s);
        
        cout << "timestamp: "<< corr_t << "." << packethdr->ts.tv_usec << "Z" << endl;
		
		memcpy(&dst_mac, ether_d->ether_dhost, sizeof(dst_mac));
        memcpy(&src_mac, ether_d->ether_shost, sizeof(src_mac));
        dmac = ether_ntoa(&src_mac);
        smac = ether_ntoa(&dst_mac);
        cout << "src MAC: "<< smac << endl;
        cout << "dst MAC: "<< dmac << endl;
        cout << "frame length: "<< packethdr->len << endl;
		
        if (ntohs(ether_d->ether_type) == ETHERTYPE_IP) 
        {
        	ip_d = (ip*)(packetdata + SIZE_ETHERNET);
        	size_ip = ip_d->ip_hl*4;

            src_addr = inet_ntoa(ip_d->ip_src);
			dst_addr = inet_ntoa(ip_d->ip_dst);
			
			cout << "src IP: "<< src_addr << endl;
            cout << "dst IP: "<< dst_addr << endl;

            if(ip_d->ip_p == 6)
            {
            	tcp_d = (tcphdr *) (packetdata + SIZE_ETHERNET + size_ip);

            	cout << "src port: "<< ntohs(tcp_d->th_sport) << endl;
            	cout << "dst port: "<< ntohs(tcp_d->th_dport) << endl;
            }
            else if(ip_d->ip_p == 17) 
            {
            	udp_d = (udphdr *) (packetdata + SIZE_ETHERNET + size_ip + 8);

            	cout << "src port: "<< ntohs(udp_d->uh_sport) << endl;
            	cout << "dst port: "<< ntohs(udp_d->uh_dport) << endl;
            }
            else if(ip_d->ip_p == 1)
            {
            //?
            }
        }
        else if (ntohs(ether_d->ether_type) == ETHERTYPE_ARP)
        {
        	arp_d = (ether_arp*)(packetdata + SIZE_ETHERNET);

            cout << "frame length: "<< packethdr->len << endl;
            printf("Sender IP: %d.%d.%d.%d\n",arp_d->arp_spa[0], arp_d->arp_spa[1], arp_d->arp_spa[2], arp_d->arp_spa[3]);
            printf("Target IP: %d.%d.%d.%d\n",arp_d->arp_tpa[0], arp_d->arp_tpa[1], arp_d->arp_tpa[2], arp_d->arp_tpa[3]);
			
			printf("Sender MAC: %02x.%02x.%02x.%02x.%02x.%02x",arp_d->arp_sha[0], arp_d->arp_sha[1], arp_d->arp_sha[2], arp_d->arp_sha[3], arp_d->arp_sha[4], arp_d->arp_sha[5]);
            printf("Target MAC: %02x.%02x.%02x.%02x.%02x.%02x",arp_d->arp_tha[0], arp_d->arp_tha[1], arp_d->arp_tha[2], arp_d->arp_tha[3], arp_d->arp_tha[4], arp_d->arp_tha[5]);
        	
        }
        else if (ntohs(ether_d->ether_type) == ETHERTYPE_IPV6)
        {
        	ipv6_d = (ip6_hdr*)(packetdata + SIZE_ETHERNET);	
        	uint8_t protocol;
        	char ipv6src[INET6_ADDRSTRLEN];
        	char ipv6dst[INET6_ADDRSTRLEN];
        	uint16_t port_p;
        	inet_ntop(AF_INET6, &ipv6_d->ip6_src, ipv6src, INET6_ADDRSTRLEN);
        	cout << "1111"<<endl;
        	inet_ntop(AF_INET6, &ipv6_d->ip6_dst, ipv6dst, INET6_ADDRSTRLEN);
        	cout << "src IP: "<< ipv6src << endl;
            cout << "dst IP: "<< ipv6dst << endl;
        	 
        	if ((protocol= ipv6_d->ip6_ctlun.ip6_un1.ip6_un1_nxt) != 0)
        	{
        	
        	if (protocol == 6)
        	{
        		port_p = *(uint16_t*)(packetdata+54);
        		cout << "src port: "<< ntohs(port_p) << endl;
            	port_p = *(uint16_t*)(packetdata+56);
            	cout << "dst port: "<< ntohs(port_p) << endl;
        	}
        	else if (protocol == 17)
        	{
        		port_p = *(uint16_t*)(packetdata+54);
        		cout << "src port: "<< ntohs(port_p) << endl;
            	port_p = *(uint16_t*)(packetdata+56);
            	cout << "dst port: "<< ntohs(port_p) << endl;
        	}
        	else if (protocol == 58)
        	{

        	}
        	}
        	
        }
        btp = (u_char *)packetdata;
        print_bytes(btp, packethdr->len);
    }

    
    static string timetostr(tm * time_s)
    {
        string mounth, day, hour, min, sec;
        mounth = to_string(time_s->tm_mon + 1);
        day = to_string(time_s->tm_mday);
        hour = to_string(time_s->tm_hour);
        min = to_string(time_s->tm_min);
        sec = to_string(time_s->tm_sec);
        if (time_s->tm_mon + 1 < 10) mounth = "0" + mounth;
        if (time_s->tm_mday < 10) day = "0" + day;
        if (time_s->tm_hour < 10) hour = "0" + hour;
        if (time_s->tm_min < 10) min = "0" + min;
        if (time_s->tm_sec < 10) sec = "0" +  sec;
        return to_string(time_s->tm_year + 1900) + "-" + mounth + "-" + day + "T" + hour + ":" + min + ":" + sec;
    }
    
    static void print_bytes(u_char *btp, int size)
    {
    	int i = 0;
    	printf("\n");
    	for (; i < size - 16; i += 16)
    	{	
        	printf("0x%04x ", i);
        	print_hex(btp, i, 16 + i);
        	print_ascii(btp, i, 16 + i);
        	printf("\n");
    	}
		printf("0x%04x ", i);
    	print_hex(btp, i, size);
    	print_ascii(btp, i, size);
    	printf("\n\n");
    }
    
    static void print_hex(u_char *data, int start, int end) {
	    printf(" ");
	    for (int i = start; i < end; i++)
	    {
	        printf("%02x ", data[i]);
	    }
    	int rest = end - start;
    	if (rest < 16)
    	{
        	for (int i = 0; i < 16 - rest; i++)
        	{
            	printf("   ");
        	}
    	}
    	printf(" ");
	}

	static void print_ascii(u_char *data, int start, int end)
	{
    	for(int i = start; i < end; i++)
    	{
    	    if(isprint(data[i])) printf("%c", data[i]);
    	    else printf(".");
    	}
	}
};

class Arguments
{   
    public:
    string interface; 
    bool tcp;
    bool udp;
    bool arp;
    bool icmp;
    string port;
    string num;

    void getargs(int argc, char *argv[])
    {
        interface=string();
        tcp=false;
        udp=false;
        arp=false;
        icmp=false;
        port="0";
        num="1";

        const char* shortopts = "i:p:n:tu";

        const struct option lopts[] = {
            {.name = "interface", .has_arg = 1, .val = 'i'},
            {.name = "tcp",       .has_arg = 0, .val = 't'},
            {.name = "udp",       .has_arg = 0, .val = 'u'},
            {.name = "arp",       .has_arg = 0, .val = 'a'},
            {.name = "icmp",      .has_arg = 0, .val = 'c'},
            { NULL, 0, NULL, 0}
        };

        int argmnt;

        while ((argmnt = getopt_long(argc, argv, shortopts, lopts, NULL)) != -1)
        {
            switch (argmnt)
            {
                case 'i':
                    interface=string(optarg);
                    break;
                case 'p':
                    port = string(optarg);
                    break;
                case 't':
                    tcp = true;
                    break;
                case 'u':
                    udp = true;
                    break;
                case 'a':
                    arp = true;
                    break;
                case 'c':
                    icmp = true;
                    break;
                case 'n':
                    num = string(optarg);
                    break;
            }
        }

        if (interface == ""){
            print_all_interfaces();
            exit(0);
        }

        if(!(tcp || udp || arp || icmp)) tcp = udp = arp = icmp =true; 

    }

    private:
    void print_all_interfaces()
    {
        char error_buffer[PCAP_ERRBUF_SIZE];
        pcap_if_t *interfaces;
        
        if (pcap_findalldevs(&interfaces, error_buffer) < 0)
        {
            printf("pcap_findalldevs error: %s", error_buffer);
            exit(1);
        }
        
        printf("INTERFACES: \n");
        for (pcap_if_t *i = interfaces; i; i = i->next)
        {
            printf("\t%s\n", i->name);
        }
    }

};

int main(int argc, char *argv[])
{
    Arguments args;
    args.getargs(argc,argv);

    Interface interf;
    interf.open(args.interface.c_str(), args.tcp, args.udp, args.arp, args.icmp, args.port);
    interf.sniff_packets(atoi(args.num.c_str()));
    interf.clear();

    return 0;
}