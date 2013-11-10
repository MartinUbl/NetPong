#include "game_inc.h"
#include "controller.h"
#include "network.h"
#include <SimplyFlat.h>

t_Controller Controller;

t_Controller::t_Controller()
{
    b_x = 0;
    b_y = 0;
    bs_x = 0;
    bs_y = 0;
    bd_x = 0;
    bd_y = 0;
}

t_Controller::~t_Controller()
{
}

void t_Controller::Update()
{
    if (myball)
    {
        if (clock() >= dest_time)
        {
            b_x = bd_x;
            b_y = bd_y;

            // zmizeni z obrazovky
            if ((m_server && bs_x > bd_x) || (!m_server && bs_x < bd_x))
            {
                // uhel od kolmice na pravy okraj (levy okraj), (-90°;90°)
                float angle = 0.0f;
                if (m_server)
                {
                    // server je napravo, bs_x > bd_x
                    if (bs_y > bd_y)
                        angle = atan((bs_y-bd_y)/(bs_x-bd_x));
                    else
                        angle = -atan((bd_y-bs_y)/(bs_x-bd_x));
                }
                else
                {
                    // klient je nalevo, bd_x > bs_x
                    if (bs_y > bd_y)
                        angle = atan((bs_y-bd_y)/(bd_x-bs_x));
                    else
                        angle = -atan((bd_y-bs_y)/(bd_x-bs_x));
                }

                char* packettext = new char[256];
                sprintf(packettext, "%f %f\0", b_y, angle);

                myball = false;
                Network.SendTextPacket(2, packettext);
            }
            // odraz od nasi leve / prave steny
            else
            {
                BounceBallVertical();
            }
        }
        else
        {
            b_x = bs_x + ( float(bd_x-bs_x) * (float(clock() - start_time)/float(dest_time - start_time)) );
            b_y = bs_y + ( float(bd_y-bs_y) * (float(clock() - start_time)/float(dest_time - start_time)) );

            if (b_y >= 480 || b_y < 0)
                BounceBallHorizontal();
        }
    }
}

void t_Controller::RetainCmdLine(const char *args)
{
    if (strncmp(args, "--server", 8) == 0)
    {
        m_server = true;
    }
    else if (strncmp(args, "--connect ", 10) == 0)
    {
        char tmp[100];
        strcpy(&tmp[0], args+10);

        server_ip = tmp;
    }
}

void t_Controller::Init()
{
    if (m_server)
    {
        Network.StartServer();
    }
    else
    {
        Network.Connect(server_ip.c_str());
    }
}

void t_Controller::StartBall()
{
    myball = true;
    if (m_server)
        b_x = 500;
    else
        b_x = 140;

    b_y = 240;

    BounceBallVertical();
}

void t_Controller::BounceBallVertical()
{
    bs_x = b_x;
    bs_y = b_y;

    if (m_server)
        bd_x = 0;
    else
        bd_x = 640;

    bd_y = 40.0f + rand()%400;

    start_time = clock();
    dest_time = start_time + clock_t((BALL_TAN_TIME_BASE/BALL_STRAIGHT_DIST) * sqrt( pow(float(bs_x - bd_x),2) + pow(float(bs_y - bd_y),2) ));

    ChangeFace();
}

void t_Controller::BounceBallHorizontal()
{
    //bd_x = bd_x;
    //bs_x = bs_x;

    // odraz od horniho kraje
    if (bd_y <= 0)
    {
        bd_y = -bd_y;
        bs_y = -bs_y;
    }
    // odraz od spodniho kraje
    else if (bd_y >= 480)
    {
        bd_y = 480 - (bd_y - 480);
        bs_y = 480 + (480 - bs_y);
    }
}

void t_Controller::RetainBall(float y, float angle)
{
    if (m_server)
    {
        b_x = 0;
        bs_x = 0;
        bd_x = 500;
    }
    else
    {
        b_x = 640;
        bs_x = 640;
        bd_x = 140;
    }

    b_y = y;
    bs_y = y;
    myball = true;

    // kladny uhel je nahoru od kolmice
    if (angle > 0)
        bd_y = y - 500*tan(angle);
    else
        bd_y = y - 500*tan(angle);

    start_time = clock();
    dest_time = start_time + clock_t((BALL_TAN_TIME_BASE/BALL_STRAIGHT_DIST) * sqrt( pow(float(bs_x - bd_x),2) + pow(float(bs_y - bd_y),2) ));
}

void t_Controller::ChangeFace()
{
    face = rand()%6;
}
