#ifndef PACKETPARSER_HPP
#define PACKETPARSER_HPP

#include <vector>
#include <string>
#include <sstream>

#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace PacketParser
{
using namespace std;
using Buffer = vector<char>;

enum IPV4_Proto
{
    TCP = 6,
    UDP = 17
};

/// Структура IP заголовка пакета
struct IPV4_Header
{
    /// [4-bit] Header length in DWORD, not bytes.
    uint8_t ip_header_len: 4;
    /// [4-bit] IPv4 version.
    uint8_t ip_version : 4;
    /// [8-bit] IP type of service.
    uint8_t ip_tos;
    /// [16-bit] Total length.
    uint16_t ip_total_length;
    /// [16-bit] Unique identifier.
    uint16_t ip_id;
    /// [5-bit] Fragment offset field.
    uint8_t ip_frag_offset :5;
    /// [1-bit] More fragment field.
    uint8_t ip_more_fragment : 1;
    /// [1-bit] Don't fragment field.
    uint8_t ip_dont_fragment : 1;
    /// [1-bit] Reserved zero fragment field.
    uint8_t ip_reserved_zero : 1;
    /// [8-bit] Fragment offset.
    uint8_t ip_frag_offset1;
    /// [8-bit] Time to live.
    uint8_t ip_ttl;
    /// [8-bit] IP Protocol (TCP, UDP).
    uint8_t ip_protocol;
    /// [16-bit] IP checksum.
    uint16_t ip_checksum;
    /// [32-bit] Source address.
    uint32_t ip_srcaddr;
    /// [32-bit] Destination address.
    uint32_t ip_destaddr;
};

/// TCP header
struct TCP_Header
{
    /// [16-bit] Source port.
    uint16_t source_port;
    /// [16-bit] Destination port.
    uint16_t dest_port;
    /// [32-bit] Sequence number.
    uint32_t sequence;
    /// [32-bit] Acknowledgement number.
    uint32_t acknowledge;

    /// [1-bit] Nonce Sum Flag Added in RFC 3540.
    uint8_t ns : 1;
    /// [3-bit] According to rfc.
    uint8_t reserved_part1 : 3;

    /// [32-bit] The number of 32-bit words in the TCP header.
    /// This indicates where the data begins.
    uint8_t data_offset : 4;

    /// [1-bit] Finish Flag.
    uint8_t fin : 1;
    /// [1-bit] Synchronise Flag.
    uint8_t syn : 1;
    /// [1-bit] Reset Flag.
    uint8_t rst : 1;
    /// [1-bit] Push Flag.
    uint8_t psh : 1;
    /// [1-bit] Acknowledgement Flag.
    uint8_t ack : 1;
    /// [1-bit] Urgent Flag.
    uint8_t urg : 1;
    /// [1-bit] ECN-Echo Flag.
    uint8_t ecn :1;
    /// [1-bit] Congestion Window Reduced Flag.
    uint8_t cwr :1;

    /// [16-bit] Window.
    uint16_t window;
    /// [16-bit] Checksum.
    uint16_t checksum;
    /// [16-bit] Urgent pointer.
    uint16_t urgent_pointer;
};

struct UDP_Header
{
    /// Source port
    uint16_t source_port;
    /// Destination port.
    uint16_t dest_port;
    /// UDP packet length.
    uint16_t udp_length;
    /// UDP checksum (optional).
    uint16_t udp_checksum;
};

string parseIpHeader(const IPV4_Header &hdr)
{
    ostringstream ss;
    ss << " -> [IP header]" << endl;
    ss << "\tIP version: " << uint32_t(hdr.ip_version) << endl;
    ss << "\tIP header length: " << uint32_t(hdr.ip_header_len)*4 << " bytes" << endl;
    ss << "\tType of service: " << uint32_t(hdr.ip_tos) << endl;
    ss << "\tIP total length: " << ntohs(hdr.ip_total_length) << " bytes" << endl;
    ss << "\tIdentification: " << ntohs(hdr.ip_id) << endl;
    ss << "\tReserved ZERO field: " << uint32_t(hdr.ip_reserved_zero) << endl;
    ss << "\tDon't fragment field: " << uint32_t(hdr.ip_dont_fragment) << endl;
    ss << "\tMore fragment field: " << uint32_t(hdr.ip_more_fragment) << endl;
    ss << "\tTTL: " << uint32_t(hdr.ip_ttl) << endl;
    ss << "\tProtocol: " << uint32_t(hdr.ip_protocol) << endl;
    ss << "\tChecksum: " << ntohs(hdr.ip_checksum) << endl;

    in_addr source {};
    source.s_addr = hdr.ip_srcaddr;
    ss << "\tSource IP: " << inet_ntoa(source) << endl;

    in_addr dest{};
    dest.s_addr = hdr.ip_destaddr;
    ss << "\tDestination IP: " << inet_ntoa(dest) << endl;

    return ss.str();
}

string parseTcpPacket(const Buffer &buf)
{
    ostringstream ss;

    auto ipHeader = reinterpret_cast<const IPV4_Header*>(buf.data());
    auto ipHeaderLen = ipHeader->ip_header_len * 4;

    ss << "> [TCP]" << endl;
    ss << parseIpHeader(*ipHeader);

    auto tcpHeader = reinterpret_cast<const TCP_Header*>(buf.data() + ipHeaderLen);

    ss << " -> [TCP Header]" << endl;
    ss << "\tSource port: " << ntohs(tcpHeader->source_port) << endl;
    ss << "\tDestination port: " << ntohs(tcpHeader->dest_port) << endl;
    ss << "\tSequence number: " << ntohl(tcpHeader->sequence) << endl;
    ss << "\tAcknowledge number: " << ntohl(tcpHeader->acknowledge) << endl;
    ss << "\tHeader length: " << uint32_t(tcpHeader->data_offset) * 4 << endl;
    ss << "\tCWR Flag: " << uint32_t(tcpHeader->cwr) << endl;
    ss << "\tECN Flag: " << uint32_t(tcpHeader->ecn) << endl;
    ss << "\tUrgent Flag: " << uint32_t(tcpHeader->urg) << endl;
    ss << "\tAcknowledgement Flag: " << uint32_t(tcpHeader->ack) << endl;
    ss << "\tPush Flag: " << uint32_t(tcpHeader->psh) << endl;
    ss << "\tReset Flag: " << uint32_t(tcpHeader->rst) << endl;
    ss << "\tSynchronise Flag: " << uint32_t(tcpHeader->syn) << endl;
    ss << "\tFinish Flag: " << uint32_t(tcpHeader->fin) << endl;
    ss << "\tWindow: " << ntohs(tcpHeader->window) << endl;
    ss << "\tChecksum: " << ntohs(tcpHeader->checksum) << endl;
    ss << "\tUrgent Pointer: " << tcpHeader->urgent_pointer << endl;

    return ss.str();
}

string parseUdpPacket(const Buffer &buf)
{
    ostringstream ss;

    auto ipHeader = reinterpret_cast<const IPV4_Header*>(buf.data());
    auto ipHeaderLen = ipHeader->ip_header_len * 4;

    ss << "> [UDP]" << endl;
    ss << parseIpHeader(*ipHeader);

    auto udpHeader = reinterpret_cast<const UDP_Header*>(buf.data() + ipHeaderLen);

    ss << " -> [UDP Header]" << endl;
    ss << "\tSource port: " << ntohs(udpHeader->source_port) << endl;
    ss << "\tDestination port: " << ntohs(udpHeader->dest_port) << endl;
    ss << "\tUDP length: " << ntohs(udpHeader->udp_length) << endl;
    ss << "\tUDP checksum: " << ntohs(udpHeader->udp_checksum) << endl;

    return ss.str();
}

string parsePacket(const Buffer &buf)
{
    auto ipHeader = reinterpret_cast<const IPV4_Header*>(buf.data());

    switch (ipHeader->ip_protocol)
    {

    case IPV4_Proto::TCP:
        return parseTcpPacket(buf);
    case IPV4_Proto::UDP:
        return parseUdpPacket(buf);
    }

    return "other";
}

}

#endif // PACKETPARSER_HPP
