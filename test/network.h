#ifndef PONG_NETWORK_H
#define PONG_NETWORK_H

#include "game_inc.h"
#include <SimplyFlat.h>

#define NETWORK_PORT 6660

struct Client
{
    SOCKET m_socket;
    hostent* m_host;
    sockaddr_in m_sockInfo;
    int m_addrLen;
};

class t_Network
{
    public:
        t_Network();
        ~t_Network();

        void Init();
        void StartServer();
        void Connect(std::string target);
        void Listen();
        uint8* BuildBlankPacket(uint8 opcode, uint8 followingsize);
        uint8* BuildTextPacket(uint8 opcode, const char* text);
        void SendTextPacket(uint8 opcode, const char* text);
        char* ReadTextPacket(char* buffer);

        void HandlePacket(uint8 opcode, char* buffer);

    private:
        bool m_ready;
        bool m_server;
        Client* oponnent;
        SOCKET m_socket;
        hostent* m_hostent;
        sockaddr_in m_sockName;
};

extern t_Network Network;

#endif
