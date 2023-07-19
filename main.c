#include <termmanip.h>
#include <time.h>
#include <stdlib.h>

enum Direction_T {UP, DOWN, LEFT, RIGHT};

struct Snake_Segment {
	int x, y;
	int og_x, og_y;
	struct Snake_Segment* next;
};

struct Snake_Segment* init_snake(int x, int y) {
	struct Snake_Segment* snake = malloc(sizeof(struct Snake_Segment));

	snake->x = x;
	snake->y = y;

	snake->og_x = x;
	snake->og_y = y;

	snake->next = NULL;

	return snake;
}

void add_segment(struct Snake_Segment* head) {
	struct Snake_Segment** last_segment = &head->next;

	while(*last_segment != NULL) {
		last_segment = &(*last_segment)->next;
	}

	*last_segment = init_snake(head->x, head->y);
}

int main(void) {
	tm_init();
	tm_set_title("Kiris Snake");
	
	int scr_cols, scr_rows;
	tm_get_scrsize(&scr_cols, &scr_rows);
	Tm_window* game_border = tm_window(0, 0, scr_cols, scr_rows);
	tm_win_border(game_border);
	Tm_window* game_window = tm_window(1, 1, tm_win_get_columns(game_border) - 2, tm_win_get_rows(game_border) - 2);

	tm_win_parent(game_border, game_window, TM_CHILD_NORMAL);

	int food_x = 15;
	int food_y = 15;

	int score = 0;

	struct Snake_Segment* snake = init_snake(10, 10);

	enum Direction_T dir = DOWN;

	tm_win_flags(game_window, TM_FLAG_INPUTBLOCK | TM_FLAG_ECHO | TM_FLAG_CURSOR_VISIBLE, 0);

	tm_win_update(game_border);

	int c;

	while(1) {
		c = tm_win_input_ch(game_window);
		tm_win_clear(game_window);

		tm_win_print(game_window, "Score %d", score);

		if(c == 'w') {
			dir = UP;
		}

		if(c == 'a') {
			dir = LEFT;
		}

		if(c == 's') {
			dir = DOWN;
		}

		if(c == 'd') {
			dir = RIGHT;
		}

		struct Snake_Segment* segments = snake;

		while(segments->next != NULL) {
			segments->next->x = segments->og_x;
			segments->next->y = segments->og_y;

			segments = segments->next;
		}

		segments = snake;
	
		snake->og_x = snake->x;
		snake->og_y = snake->y;

		while(segments->next != NULL) {
			segments->next->og_x = segments->next->x;
			segments->next->og_y = segments->next->y;

			segments = segments->next;
		}

		if(dir == UP) {
			snake->y--;
			snake->og_y--;
		}

		else if(dir == LEFT) {
			snake->x--;
			snake->og_x--;
		}

		else if(dir == DOWN) {
			snake->y++;
			snake->og_y++;
		}

		else if(dir == RIGHT) {
			snake->x++;
			snake->og_x++;
		}

		tm_win_cursor(game_window, food_x, food_y);
		tm_win_print(game_window, "A");

		if(food_x == snake->x && food_y == snake->y) {
			add_segment(snake);

			food_x = rand() % tm_win_get_columns(game_window);
			food_y = rand() % tm_win_get_rows(game_window);

			score++;
		}

		struct Snake_Segment* render_snake = snake;

		while(render_snake != NULL) {
			tm_win_cursor(game_window, render_snake->x, render_snake->y);
			if(tm_win_print(game_window, "@") == TM_ERROR) {
				tm_print("you lost!");
				tm_input_ch();
				tm_exit();
				return 1;
			}
			render_snake = render_snake->next;
		}

		Sleep(50);
	}

	tm_exit();
}
