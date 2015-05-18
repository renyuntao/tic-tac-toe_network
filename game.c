#include"game.h"



TTTPiece ttt_check_win(TTTBoard *board)
{
	TTTPiece *p = board->p;

	if (p[0] == p[1] && p[1] == p[2]) return p[2];
	if (p[3] == p[4] && p[4] == p[5]) return p[5];
	if (p[6] == p[7] && p[7] == p[8]) return p[8];
	if (p[0] == p[3] && p[3] == p[6]) return p[6];
	if (p[1] == p[4] && p[4] == p[7]) return p[7];
	if (p[2] == p[5] && p[5] == p[8]) return p[8];
	if (p[0] == p[4] && p[4] == p[8]) return p[8];
	if (p[2] == p[4] && p[4] == p[6]) return p[6];

	return MARK_NONE;
}
void ttt_clear_board(TTTBoard *board)
{
	int i;

	for (i = 0; i < 9; i++) {
		board->p[i] = MARK_NONE;
	}
}
void ttt_copy_board(TTTBoard *dest, TTTBoard *src)
{
	*dest = *src;
}
int ttt_board_full(TTTBoard *board)
{
	int i;

	for(i = 0; i < 9; i++) {
		if (board->p[i] == MARK_NONE) return 0;
	}

	return 1;
}
void ttt_print_board(TTTBoard *board)
{
	int i;
	printf("-----\n");
	for (i = 0; i < 9; i++) {
		char c;
		switch (board->p[i]) {
			case MARK_X: c = 'X'; break;
			case MARK_O: c = 'O'; break;
			default: c = '.';
		}
		printf("%c", c);
		if ((i+1)%3 == 0) {
			printf("    %d %d %d\n", i-2, i-1, i);
		} else {
			printf(" ");
		}
	}
	printf("-----\n");
}
int ttt_choose_random_move(TTTBoard *board)
{
	int candidatecount = 0;
	int candidate[9];
	int i;

	for (i = 0; i < 9; i++) {
		if (board->p[i] == MARK_NONE) {
			candidate[candidatecount++] = i;
		}
	}

	if (candidatecount == 0) return -1;

	return candidate[rand()%candidatecount];
}

int ttt_run_random_games(TTTBoard *board, TTTPiece currealplayer, int count)
{
	int i, pos, firstmove;
	int wincount[9], maxwins;
	int candidatecount = 0;
	int candidate[9];
	TTTPiece winner, player;
	TTTBoard tmp;
	int hasfreespace = 0;

	for (i = 0; i < 9; i++) {
		if (board->p[i] == MARK_NONE) {
			hasfreespace = 1;
			wincount[i] = 0;  //*wincount[] mark where can go,0 is can go
		} else {
			wincount[i] = INT_MIN; // don't go where marks already are
		}
	}

	if (!hasfreespace) { return -1; } // there are no best moves--cat's game

	// play ball!
	for (i = 0; i < count; i++) {
		ttt_copy_board(&tmp, board);
		player = currealplayer;
		firstmove = -1;
		while ((winner = ttt_check_win(&tmp)) == MARK_NONE) {
			pos = ttt_choose_random_move(&tmp);
			if (pos == -1) {
				winner = MARK_NONE; // cat's game
				break;
			}
			assert(tmp.p[pos] == MARK_NONE);
			tmp.p[pos] = player;

			//ttt_print_board(&tmp);
			if (firstmove == -1) { firstmove = pos; } 

			player = (player == MARK_X)? MARK_O: MARK_X;
		}

		// see if we won on this move:
		if (winner != MARK_NONE) {
			if (winner == currealplayer) {
				wincount[firstmove] += WIN_POINTS;
			} else {
				wincount[firstmove] += LOSS_POINTS;
			}
		} else {
			wincount[firstmove] += DRAW_POINTS;
		}
	}

	// now find the most common winning move (or randomly choose on
	// ties)
	for (maxwins = INT_MIN, i = 0; i < 9; i++) {
		if (wincount[i] > maxwins) { maxwins = wincount[i]; }
	}

	for (i = 0; i < 9; i++) {
		if (wincount[i] == maxwins) {
			candidate[candidatecount++] = i;
		}
	}

	assert(candidatecount > 0);
	printf("[scores:");
	for(i = 0; i < 9; i++) {
		if (wincount[i] == INT_MIN) {
			printf(" --");
		} else {
			printf(" %d", wincount[i]);
		}
	}
	printf("]\n");
	printf("[best AI move(s):");
	for (i = 0; i < candidatecount; i++) { printf(" %d", candidate[i]); }
	printf("]\n");

	return candidate[rand()%candidatecount];
}

void game()
{
	char line[512];
	int move, validmove;
	TTTBoard cur;
	TTTPiece winner, curplayer;
	TTTPiece humanplayer = HUMAN_PLAYER;
	int bestpos;

	ttt_clear_board(&cur);

	srand(time(NULL)+getpid());

	curplayer = MARK_X;

	// run a game
	while ((winner = ttt_check_win(&cur)) == MARK_NONE) {    //判断是否分出胜负
		if (ttt_board_full(&cur)) {         //检查棋盘是否下满
			winner = MARK_NONE; // cat's game--no where else to play
			break;
		}

		ttt_print_board(&cur);

		if (curplayer == humanplayer) {   //*human player    humanplayer is MARK_X
			validmove = 0;
			do {
				printf("Enter a move (0-8): ");
				fflush(stdout);
				fgets(line, sizeof line, stdin);
				if (sscanf(line, " %d", &move) != 1 || move < 0 || move > 8 ||
						cur.p[move] != MARK_NONE) {
					printf("Invalid move!\n");
				} else {
					validmove = 1;
				}
			} while (!validmove);
			cur.p[move] = curplayer;    //*curplayer is MARK_X
		} else { // computer player
			bestpos = ttt_run_random_games(&cur, curplayer, ROUNDS);

			assert(cur.p[bestpos] == MARK_NONE);
			printf("Computer plays: %d\n", bestpos);
			cur.p[bestpos] = curplayer;    //将该位置标记为已下棋
		}

		curplayer = (curplayer == MARK_X)? MARK_O: MARK_X;   //通过这种方法使双方交替下棋
	}

	ttt_print_board(&cur);

	if (winner == MARK_NONE) {
		printf("Cat's game.\n");
	} else {
		if (winner == MARK_X) {
			printf("X wins!\n");
		} else {
			printf("O wins!\n");
		}
	}

}
