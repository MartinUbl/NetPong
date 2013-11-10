#include "network.h"
#include "controller.h"

t_Network Network;

t_Network::t_Network()
{
}

t_Network::~t_Network()
{
}

void t_Network::Init()
{
    m_ready = false;
    m_server = true;
    oponnent = NULL;
}

void t_Network::StartServer()
{
    m_ready = false;
    m_server = true;

    WORD version = MAKEWORD(1,1);
    WSADATA data;
    if (WSAStartup(version, &data) != 0)
        return;

    if ((m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        return;

    m_sockName.sin_family = AF_INET;
    m_sockName.sin_port = htons(NETWORK_PORT);
    m_sockName.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_socket, (sockaddr*)&m_sockName, sizeof(m_sockName)) == -1)
        return;

    u_long arg = 1;
    if (ioctlsocket(m_socket, FIONBIO, &arg) == SOCKET_ERROR)
        return;

    int res = listen(m_socket, 10);
    if (res == -1)
    {
        return;
    }

    m_ready = true;
}

void t_Network::Connect(std::string target)
{
    m_ready = false;
    m_server = false;

    WORD wVersionRequested = MAKEWORD(1,1);
    WSADATA data;

    if (WSAStartup(wVersionRequested, &data) != 0)
        return;

    if ((m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        return;

    if ((m_hostent = gethostbyname(target.c_str())) == NULL)
        return;

    m_sockName.sin_family = AF_INET;
    m_sockName.sin_port = htons(NETWORK_PORT);
    memcpy(&(m_sockName.sin_addr), m_hostent->h_addr, m_hostent->h_length);
    if (connect(m_socket, (sockaddr*)&m_sockName, sizeof(m_sockName)) == -1)
        return;

    u_long arg = 1;
    if (ioctlsocket(m_socket, FIONBIO, &arg) == SOCKET_ERROR)
        return;

    oponnent = new Client;
    oponnent->m_socket = m_socket;

    m_ready = true;
}

void t_Network::Listen()
{
    if (m_ready && m_server && !oponnent)
    {
        int error;

        sockaddr_in sockInfo;
        int addrlen = sizeof(sockInfo);
        SOCKET result;

        result = accept(m_socket, (sockaddr*)&sockInfo, &addrlen);
        error = WSAGetLastError();

        if (result == INVALID_SOCKET && error == WSAEWOULDBLOCK)
        {
            //failed to accept
            //TODO: some error message
        }
        else if (result == INVALID_SOCKET && error != WSAEWOULDBLOCK)
        {
            //invalid socket
            //TODO: some error message
        }
        else if (result != INVALID_SOCKET && error == ERROR_SUCCESS)
        {
            oponnent = new Client;
            oponnent->m_sockInfo = sockInfo;
            oponnent->m_socket = result;
            oponnent->m_addrLen = sizeof(sockInfo);

            SendTextPacket(1, "Ahoj");
        }

        return;
    }
    else if (m_ready)
    {
        char *buffer = new char[512];

        int result = recv(oponnent->m_socket, buffer, 512, 0);
        if (result > 0)
        {
            char *plaintext = ReadTextPacket(buffer);
            HandlePacket(buffer[0], plaintext);
        }

        delete buffer;
    }
}

uint8* t_Network::BuildBlankPacket(uint8 opcode, uint8 followingsize)
{
    uint8* packet = new uint8[2+followingsize];
    packet[0] = opcode;
    packet[1] = followingsize;

    return packet;
}

uint8* t_Network::BuildTextPacket(uint8 opcode, const char* text)
{
    uint8* packet = BuildBlankPacket(opcode, strlen(text)+1);

    memcpy(packet+2, text, strlen(text));
    packet[2 + strlen(text)] = '\0';

    return packet;
}

void t_Network::SendTextPacket(uint8 opcode, const char* text)
{
    uint16 size = 2 + strlen(text) + 1;
    uint8* packet = BuildTextPacket(opcode, text);

    send(oponnent->m_socket, (char*)packet, size, 0);
}

char* t_Network::ReadTextPacket(char *buffer)
{
    // pozice 1 = dylka textu
    char* text = new char[buffer[1]+1];
    memcpy(text, &buffer[2], buffer[1]);
    buffer[buffer[1]] = '\0';

    return text;
}

void t_Network::HandlePacket(uint8 opcode, char *buffer)
{
    switch (opcode)
    {
        // sync packet, hello
        case 1:
            // "Ahoj" vysila server
            if (strncmp(buffer, "Ahoj\0", 5) == 0)
            {
                SendTextPacket(1, "AhojTaky");
            }
            // "AhojTaky" vysila klient
            else if (strncmp(buffer, "AhojTaky\0", 9) == 0)
            {
                Controller.StartBall();
            }
            break;
        // predani micku
        case 2:
        {
            float get_y = 0;
            float angle = 0;
            char *tmp = new char[128];

            for (int i = 0; i < strlen(buffer); i++)
            {
                if (buffer[i] == ' ')
                {
                    strncpy(tmp, &buffer[0], i+1-1);
                    get_y = atof(tmp);
                    angle = atof(&buffer[i+1]);
                    Controller.RetainBall(get_y, angle);
                    break;
                }
            }

            delete tmp;

            break;
        }
    }
}
