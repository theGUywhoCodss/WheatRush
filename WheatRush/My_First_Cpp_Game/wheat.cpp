class plant
{
    // size is 20,20
    u32 color=0x000000;
    int leaves[4];
public:
    float x;
    float y;
    bool digged;
    plant() = default;
    plant(float x, float y)
    {
        //this -> variable so it can assign it
        this->x = x;
        this->y = y;
        this->digged = false;
        if (rand() % 2 == 0) {
            color = 0xD4AF37;
        }
        else {
            color = 0xFFD700;
        }
        for (int i = 0; i < sizeof(leaves) / sizeof(leaves[0]); i++) {
            leaves[i] = (rand() % 11)-5;
        }
    }
    void drawing(float offx,float offy) {
        //if (type) {
        //    draw_rect(x, y, 2, 10, 0xD4AF37);
        //}else {
        //    draw_rect(x, y, 2, 10, 0xFFD700);
        //}
        float diffx = x - offx;
        float diffy = y - offy;
        if (!digged) {
            draw_rect(diffx + 2, leaves[0] + diffy, 2, 2, 0x7D7D24);
            draw_rect(diffx + 6, diffy, 2, 10, color);
            draw_rect(diffx + 10, leaves[1] + diffy, 2, 2, 0x7D7D24);
            draw_rect(diffx - 2, leaves[2] + diffy, 2, 2, 0x7D7D24);
            draw_rect(diffx - 6, diffy, 2, 10, color);
            draw_rect(diffx - 10, leaves[3] + diffy, 2, 2, 0x7D7D24);
        }
        else {
            draw_rect(diffx,diffy, 2, 2, 0x7D7D24);
        }
    }
};