class player
{
    // size is 20,20
public:
    float speed = 25;
    float x;
    float y;
    int size=4;
    player() = default;
    player(int x, int y)
    {
        //this -> variable so it can assign it
        this->x = x;
        this->y = y;
    }
    void drawing() {
        draw_rect(0, 0, 4, 4, 0x000000);
    }
    void move(int hor,int ver,float dt) {
        x += (hor*speed)*dt;
        y += (ver * speed) * dt;
    }
};