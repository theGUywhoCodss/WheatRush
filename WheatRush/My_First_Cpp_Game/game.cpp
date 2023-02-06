#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

//Credits
//https://youtube.com/watch?v=vRrMhL5fzPY&feature=shares // FPS!
//https://youtube.com/watch?v=s3aYw54R5KY&feature=shares // INFINITE GENERATION!
//https://youtube.com/watch?v=luuyjjOxnUI&feature=shares // GAME ENGINE VERY COOL!

//Mouse
int mouseX;
int mouseY;
bool mouseClick=false;
bool debug = false;
// game
int money=0;
float game_timer = 30;
int quota = 30;
float quota_rate = 1.5f;
int money_rate = 1;
int prices[] = {10, 10, 10};
//Game
enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_SHOP,
	GM_OVER,
	GM_CREDITS,
};
struct Render_Math {
	int width = (render_state.width / 2.f) / (render_state.height * render_scale);
	int height = (render_state.height / 2.f) / (render_state.height * render_scale);
};

Gamemode current_gamemode;
Render_Math DisplaySize;

bool loaded = false;
//Assets
player person = player(0,0);
vector<plant> wheats;

int chunk_size = 3;
map<string, vector<plant>> game_wheats;

internal void
reset_game(bool game_over) {
	if (game_over) {
		money = 0;
		game_timer = 30;
		quota = 30;
		game_wheats.clear();
		person = player(0, 0);
		money_rate = 1;
		prices[0] = 10;
		prices[1] = 10;
		prices[2] = 10;
	}else {
		game_timer = 30;
		game_wheats.clear();
	}
}

internal void
load_all() {
	int wheatY = 0;
	Render_Math yes;
	DisplaySize = yes;
	for (int i = 0; i < 35; i++) {
		if (!loaded) {
			wheats.push_back(plant((i % 7 * 26) - (DisplaySize.width - 11), (wheatY * -20) + (DisplaySize.height - 10)));
		}
		else {
			wheats[i] = plant((i % 7 * 26) - (DisplaySize.width - 11), (wheatY * -20) + (DisplaySize.height - 10));
		}
		if ((i+1)%7 == 0) {
			wheatY++;
		}
	}
}
//internal void
//make_chunk(int access) {
//	int chunksize = 3;
//	int wheatY = 0;
//	for (int i = 0; i < 35; i++) {
//		game_wheats[access] = plant((i % 7 * 26) - (DisplaySize.width - 11), (wheatY * -20) + (DisplaySize.height - 10));
//		if ((i + 1) % chunksize == 0) {
//			wheatY++;
//		}
//	}
//}
internal void 
infinite_wheat(float offsetx,float offsety) {
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	//bool right = p1x + hs1x > p2x - hs2x;
	//bool left = p1x - hs1x < p2x + hs2x;
	//bool up = p1y + hs1y > p2y - hs2y;
	//bool down = p1y - hs1y < p2y + hs2y;
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}



internal void
simulate_game(Input* input, float dt) {
	if (!loaded) {
		load_all();
		loaded = true;
	}
	if (current_gamemode == GM_GAMEPLAY) {

		float offsetx = person.x;
		float offsety = person.y;
		clear_screen(0x964B00);
		// input
		if (is_down(BUTTON_UP)) {
			person.move(0, 1, dt);
		}
		if (is_down(BUTTON_DOWN)) {
			person.move(0, -1, dt);
		}
		if (is_down(BUTTON_LEFT)) {
			person.move(-1, 0, dt);
		}
		if (is_down(BUTTON_RIGHT)) {
			person.move(1, 0, dt);
		}
		// infinite generation
		int last_chunkx;
		int last_chunky;
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				bool found = false;
				float dirtsx = 26;
				float dirtsy = 20;
				int target_x = x - 2 + int(round(offsetx / (chunk_size * dirtsx)));
				int target_y = y - 2 + int(round(offsety / (chunk_size * dirtsy)));
				string target_chunk = to_string(target_x) + ";" + to_string(target_y);
				last_chunkx = target_x;
				last_chunky = target_y;
				//find and draw
				if (game_wheats.find(target_chunk) == game_wheats.end()) {
					vector<plant> chunk_wheats;
					// if not found
					for (int x1 = 0; x1 < chunk_size; x1++) {
						for (int y1 = 0; y1 < chunk_size; y1++) {
							chunk_wheats.push_back(plant(target_x * (dirtsx * chunk_size) + (x1 * dirtsx), target_y * (dirtsy * chunk_size) + (y1 * dirtsy)));
						}
					}
					game_wheats[target_chunk] = chunk_wheats;

				}
				else {
					found = true;
					for (int i = 0; i < game_wheats[target_chunk].size(); i++) {
						game_wheats[target_chunk][i].drawing(offsetx, offsety);
						//collision
						if (!game_wheats[target_chunk][i].digged && aabb_vs_aabb(person.x, person.y, person.size, person.size, game_wheats[target_chunk][i].x, game_wheats[target_chunk][i].y, 11, 10)) {
							game_wheats[target_chunk][i].digged = true;
							i--;
							money+= money_rate;
						}
					}
				}
				//wheats[i].drawing(offsetx, offsety);
				// wheat collision
			}
		}
		// drawing
		person.drawing();
		draw_number(money, 0, 0, 1, 0xFFFFFF);
		draw_number(game_timer, 0, DisplaySize.height - 10, 1, 0x000000);
		// time
		if (game_timer < 0) {
			if (money < quota) {
				reset_game(true);
				current_gamemode = GM_OVER;
			}
			else {
				current_gamemode = GM_SHOP;
				quota = round(quota*quota_rate);
				reset_game(false);
			}
		}
		else {
			game_timer -= dt;
		}
		draw_number(quota, -DisplaySize.width + 10, DisplaySize.height - 10, 1, 0xFF0000);
		draw_text("QUOTA", -DisplaySize.width + 5, DisplaySize.height, 1, 0xFF0000);
		//draw_number(last_chunky, 30, 30, 1, 0xFFFFFF);
	}else if (current_gamemode == GM_OVER) {
		clear_screen(0x964B00);
		for (int i = 0; i < wheats.size(); i++) {
			wheats[i].drawing(0, 0);
		}
		draw_rect(0, 0, 50, 40, 0xBA8C63);
		draw_text("GAME OVER", -45, 35, 1.5, 0xFF0000);
		draw_rect(0, 0, 18, 8, 0x996515);
		draw_text("BACK TO", -15, 6, .7, 0x000000);
		draw_text("MENU", -10, 0, .7, 0x000000);
		if (mouseClick) {
			if (aabb_vs_aabb(mouseX, mouseY, .1, .1, 0,0, 18, 8)) {
				current_gamemode = GM_MENU;

			}
			mouseClick = false;
		}
	}else if (current_gamemode == GM_SHOP) {
		clear_screen(0x63666A);
		draw_rect(-30, 0, 50, 40, 0xBA8C63);
		draw_rect(-30,25,45,10, 0x63462d);
		draw_text("SHOP", -70, 25, .8, 0x000000);
		// SPEED
		draw_rect(-60,-5,10,15, 0x63462d);
		draw_rect(-60, -30, 10, 5, 0x00FF00);
		draw_text("SPEED", -65, -5, .5, 0x000000);
		draw_number(person.speed, -60, -12, 1, 0xFF0000);
		draw_number(-prices[0], -55, -30, 1, 0x000000);
		// MONEY
		draw_rect(-30, -5, 10, 15, 0x63462d);
		draw_rect(-30, -30, 10, 5, 0x00FF00);
		draw_text("MONEY", -35, 0, .5, 0x000000);
		draw_text("RATE", -35, -5, .5, 0x000000);
		draw_number(money_rate, -30, -12, 1, 0xFF0000);
		draw_number(-prices[1], -25, -30, 1, 0x000000);
		// RANGE
		draw_rect(0, -5, 10, 15, 0x63462d);
		draw_rect(0, -30, 10, 5, 0x00FF00);
		draw_text("RANGE", -5, -5, .5, 0x000000);
		draw_number(person.size, 0, -12, 1, 0xFF0000);
		draw_number(-prices[2], 5, -30, 1, 0x000000);
		// MONEY
		draw_number(money, 0, DisplaySize.height - 15, 1, 0x00FF00);
		draw_text("MONEY", -10, DisplaySize.height-5, 1, 0x00FF00);
		// Click detection
		// Resume
		draw_rect(55, -DisplaySize.height + 10, 18, 8, 0x996515);
		draw_text("START ANOTHER", 40, -DisplaySize.height+14, .4, 0x000000);
		draw_text("SHIFT", 50, -DisplaySize.height + 8, .4, 0x000000);
		if (mouseClick) {
			if (aabb_vs_aabb(mouseX, mouseY, .1, .1, 55, -DisplaySize.height + 10, 18, 8)) {
				current_gamemode = GM_GAMEPLAY;
				money = 0;
			}
			// SPEED
			if (aabb_vs_aabb(mouseX, mouseY, .1, .1, -60, -30, 10, 5)) {
				if (prices[0] <= money) {
					money -= prices[0];
					prices[0] *= 1.25;
					person.speed +=2;
				}
			}
			//MONEY
			if (aabb_vs_aabb(mouseX, mouseY, .1, .1, -30, -30, 10, 5)) {
				if (prices[1] <= money) {
					money -= prices[1];
					prices[1] +=150;
					money_rate += 1;
				}
			}
			//RANGE
			if (aabb_vs_aabb(mouseX, mouseY, .1, .1, 0, -30, 10, 5)) {
				if (prices[2] <= money) {
					money -= prices[2];
					prices[2] *= 1.25;
					person.size += 1;
				}
			}
			mouseClick = false;
		}
		draw_number(quota, -DisplaySize.width + 10, DisplaySize.height - 10, 1, 0xFF0000);
		draw_text("QUOTA", -DisplaySize.width + 5, DisplaySize.height, 1, 0xFF0000);
	}
	else if (current_gamemode == GM_CREDITS) {
		clear_screen(0x964B00);
		for (int i = 0; i < wheats.size(); i++) {
			wheats[i].drawing(0, 0);
		}
		draw_rect(0, 0, 80, 10, 0xFFD700);
		draw_text("WHEAT RUSH BY HELMET BABY", -75, 0, 1, 0x000000);
	}else {
		clear_screen(0x964B00);
		for (int i = 0; i < wheats.size(); i++) {
			wheats[i].drawing(0,0);
		}
		draw_rect(0, 35, 80,10, 0xFFD700);
		draw_text("WHEAT RUSH BY HELMET BABY", -75, 35, 1, 0x000000);
		draw_rect(-60, 4, 20, 10, 0xFFD700);

		draw_rect(-60, 4, 18, 8, 0x996515);
		draw_text("START IT",-75,6, .7, 0x000000);
		//draw_rect(-60, -15, 18, 8, 0x996515);
		//draw_text("SETTINGS", -75, -13, .7, 0x000000);
		//draw_rect(-60, -15, 18, 8, 0x996515);
		//draw_text("CREDITS", -75, -13, .7, 0x000000);
		// set up buttons
		if (mouseClick) {
			if (aabb_vs_aabb( mouseX, mouseY, .1, .1, - 60, 4, 18, 8)) {
				current_gamemode = GM_GAMEPLAY;
				
			}
			//if (aabb_vs_aabb(mouseX, mouseY, .1, .1, -60, -15, 18, 8)) {
			//	current_gamemode = GM_CREDITS;

			//}
			mouseClick = false;
		}
		/*if (pressed(BUTTON_DOWN)) {
			load_all();
		}*/
	}
}