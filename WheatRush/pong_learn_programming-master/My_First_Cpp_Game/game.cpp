#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input -> buttons[b].is_down && input->buttons[b].changed)

// VARIABLES XXXXXXXXXXXXXXXXXXX
float sized = 30;
bool updated = false;
bool in_planet = false;

int numum = 1;
// FUNCTIONS XXXXXXXXXXXXXXXXXXX
internal bool aabb_vs_aabb(float p1x, float p1y,float hs1x,float hs1y, float p2x, float p2y, float hs2x, float hs2y){
	if (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y) return true;
	else return false;

}
// all randomly affected variables

// GAME XXXXXXXXXXXXXXXXXXXXXXX
internal void simulate_game(Input* input, float dt) {
	clear_screen(0x000000);
	draw_text("HI", -80, -10, 1, 0xaaaaaa);
}