#include "game_inc.h"
#include "drawing.h"
#include "controller.h"
#include <SimplyFlat.h>

void t_MyDraw::Draw()
{
    SFDrawing->ClearColor(0,0,0);
    //SFDrawing->DrawRectangle(10,10,200,200,COLOR(255, 0, 0));
    if (Controller.IsBallOnMySide())
    {
        SFDrawing->DrawCircle(Controller.GetBallX(),Controller.GetBallY(), 10, COLOR(255, 255, 0));
    }

    float d_y = Controller.GetDestY();
    if (d_y > 480)
        d_y = 480 - (d_y - 480);
    else if (d_y < 0)
        d_y = -d_y;

    if (Controller.IsServer())
        SFDrawing->DrawRectangle(500+10, (d_y>20)?(d_y-20):0, 40, 10, COLOR(255,255,255));
    else
        SFDrawing->DrawRectangle(140-10, (d_y>20)?(d_y-20):0, 40, 10, COLOR(255,255,255));

    ///////

    uint32 left_b = 60;
    if (Controller.IsServer())
        left_b = 580;

    // kolecko
    SFDrawing->DrawCircle(left_b, 240, 35, COLOR(255,255,255));
    SFDrawing->DrawCircle(left_b, 240, 30, COLOR(0,0,0));

    // oci
    SFDrawing->DrawCircle(left_b-10, 230, 5, COLOR(255,255,255));
    SFDrawing->DrawCircle(left_b+10, 230, 5, COLOR(255,255,255));

    // pusa
    switch (Controller.GetFace())
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            SFDrawing->DrawCircle(left_b, 252, 5+Controller.GetFace()*1.5f, COLOR(255,255,255));
            SFDrawing->DrawCircle(left_b, 252, 3+Controller.GetFace()*1.5f, COLOR(0,0,0));
            break;
        /*case 1:
            SFDrawing->DrawRectangle(250, 565, 10, 30, COLOR(255,255,255));
            break;
        case 2:
            glBegin(GL_QUADS);
              glColor3ub(255, 255, 255);
                glVertex2d(250, 565);
                glVertex2d(250, 565+30);
                glVertex2d(260+10, 565+30);
                glVertex2d(260+10, 565);
            glEnd();
            break;*/
    }
}
