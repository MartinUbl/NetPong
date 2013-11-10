#ifndef PONG_CONTROLLER_H
#define PONG_CONTROLLER_H

#include "game_inc.h"
#include <SimplyFlat.h>

#define BALL_TAN_TIME_BASE 1000
#define BALL_STRAIGHT_DIST 500.0f

class t_Controller
{
    public:
        t_Controller();
        ~t_Controller();

        void Init();

        void RetainCmdLine(const char* args);

        void Update();

        void StartBall();
        void BounceBallVertical();
        void BounceBallHorizontal();
        void RetainBall(float y, float angle);

        bool IsServer() { return m_server; };

        bool IsBallOnMySide() { return myball; };
        float GetBallX() { return b_x; };
        float GetBallY() { return b_y; };

        float GetDestX() { return bd_x; };
        float GetDestY() { return bd_y; };

        void ChangeFace();
        uint8 GetFace() { return face; };

    private:
        bool m_server;
        std::string server_ip;

        bool myball;
        float bs_x, bs_y, bd_x, bd_y, b_x, b_y;
        clock_t start_time, dest_time;

        uint8 face;
};

extern t_Controller Controller;

#endif
