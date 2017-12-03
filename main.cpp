/*
 * File: Lab_6.4.4_HuanyouWei.cpp
 * Author: Huanyou Wei
 * November 25, 2017
 * Dr. Noukhovitch
 * ICS4U1-01
 */

/*
 * Error Recreation:
 * e2 to e4
 * b8 to c6
 * e4 to e5
 * c6 to e5
 * e1 to e2
 */

#include <iostream>
#include <cmath>

using namespace std;

class Piece;
class Pawn;
class Rook;
class Bishop;
class Knight;
class Queen;
class King;


struct Pos{
	int x, y;
};


class GameBoard{
private:
	string matrix[8][8] = {{" R", " N", " B", " Q", " K", " B", " N", " R"},
						   {" P", " P", " P", " P", " P", " P", " P", " P"},
						   {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
						   {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
						   {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
						   {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
						   {"*P", "*P", "*P", "*P", "*P", "*P", "*P", "*P"},
						   {"*R", "*N", "*B", "*Q", "*K", "*B", "*N", "*R"}};
public:
	void display(void){
		for (int row = 7; row >= 0; row--){
			cout << " +---+---+---+---+---+---+---+---+" << endl;
			cout << row + 1;
			cout << "|" << matrix[row][0] << " |" << matrix[row][1] << " |" << matrix[row][2] << " |" << matrix[row][3] <<
				   " |" << matrix[row][4] << " |" << matrix[row][5] << " |" << matrix[row][6] << " |" << matrix[row][7] << " |" << endl;
		}
		cout << " +---+---+---+---+---+---+---+---+" << endl;
		cout << "   a   b   c   d   e   f   g   h " << endl;
	}
	string getPiece(int x, int y){
		return matrix[y][x];
	}
	friend class Piece;
	friend class Pawn;
	friend class Rook;
	friend class Bishop;
	friend class Knight;
	friend class Queen;
	friend class King;
};

int player_check(char);
bool xy_check(GameBoard&, Pos, Pos);
bool checkmate(GameBoard&, Pos, char);

class Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		game.matrix[end.y][end.x] = game.matrix[start.y][start.x];
		game.matrix[start.y][start.x] = "  ";
		}
};

class Pawn: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = end.y-start.y;
		if (current != "  "){
			int dir = player_check(current[0]);
			dy *= dir;
			if (((((start.y%5 == 1 && dy == 2) || dy == 1) && dx == 0 && xy_check(game, start, end) && final == "  ")
				|| (final != "  " && final[0] == enemy_id && (dx == 1 && dy == 1))))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

class Rook: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);
		if (current != "  "){
			if (((dx != 0 && dy == 0) || (dx == 0 && dy != 0)) && xy_check(game, start, end)
				&& (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

class Bishop: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);
		if (current != "  "){
			if (dx == dy && xy_check(game, start, end) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

class Knight: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);
		if (current != "  "){
			if (((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

class Queen: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		if (current != "  "){
			if (xy_check(game, start, end) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

class King: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);
		if (current != "  "){
			if ((dx <= 1 && dy <= 1) && (dx != 0 || dy != 0) && checkmate(game, end, current[0])
				&& (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

struct Player{
	Pawn p;
	Rook r;
	Bishop b;
	Knight n;
	Queen q;
	King k;
	char ID;
};

int player_check(char piece_id){
	if (piece_id == '*')
		return -1;
	else
		return 1;
}

bool xy_check(GameBoard &game, Pos start, Pos end){
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int dirx = dx >= 0 ? 1 : -1;
	int diry = dy >= 0 ? 1 : -1;
	dx = abs(dx);
	dy = abs(dy);
	if (dy != 0 || dx != 0){
		if (dy == 0){
			for (int i = 1; i < dx; i++){
				if (game.getPiece(start.x + i*dirx, start.y) != "  ")
					return false;
			}
			return true;
		}
		else if (dx == 0){
			for (int i = 1; i < dy; i++){
				if (game.getPiece(start.x, start.y + i*diry) != "  ")
					return false;
			}
			return true;
		}
		else if (dx == dy){
			for (int i = 1; i < dx; i++){
				if (game.getPiece(start.x + i*dirx, start.y + i*diry) != "  ")
					return false;
			}
			return true;
		}
	}
	return false;
}

bool check_straight(char enemy_id, string piece){
	return (piece[0] == enemy_id && (piece[1] == 'R' || piece[1] == 'Q'));
}
bool check_diagonal(char enemy_id, string piece){
	return (piece[0] == enemy_id && (piece[1] == 'B' || piece[1] == 'Q'));
}
bool check_pawn(GameBoard &game, char enemy_id, Pos pos){
	int dir = player_check(enemy_id);
	string piece1, piece2;
	if (pos.x == 0){
		piece1 = game.getPiece(pos.x + 1, pos.y - dir);
		if (piece1[0] == enemy_id && piece1[1] == 'P')
			return true;
	}
	else if (pos.x == 7){
		piece1 = game.getPiece(pos.x - 1, pos.y - dir);
		if (piece1[0] == enemy_id && piece1[1] == 'P')
			return true;
	}
	else{
		piece1 = game.getPiece(pos.x + 1, pos.y - dir);
		piece2 = game.getPiece(pos.x - 1, pos.y - dir);
		if ((piece1[0] == enemy_id && piece1[1] == 'P') || (piece2[0] == enemy_id && piece2[1] == 'P'))
			return true;
	}
	return false;
}
bool check_piece(char enemy_id, char piece_id, string piece){
	return (piece[0] == enemy_id && piece[1] = piece_id);
}
bool check(GameBoard  &game, char enemy_id, Pos pos){
/*	if (pos.y == 0){
		if (pos.x <= 1){
			p1 = game.getPiece(pos.x + 1, pos.y + 2);
			p2 = game.getPiece(pos.x + 2, pos.y + 1);
			if (pos.x == 0 && pos.y == 0)
				if (check(enemy_id, 'N', p1, p2))//case1
					return true;
			else if (pos.x == 1 && pos.y == 0){
				p3 = game.getPiece(pos.x - 1, pos.y + 2);
				if (check(enemy_id, 'N', p1, p2, p3))//case2
					return true;
			}
			else if (pos.x == 0 && pos.y == 1){

			}
			else if (pos.x == 1 && pos.y == 1){

			}
		}
		else if (pos.x >= 6){
			p1 = game.getPiece(pos.x - 1, pos.y + 2);
			p2 = game.getPiece(pos.x - 2, pos.y + 1);
			p3 = game.getPiece(pos.x + 1, pos.y + 2);
			if (pos.x == 7 && check(enemy_id, 'N', p1, p2))//case3
				return true;
			else if (pos.x == 6 && check(enemy_id, 'N', p1, p2, p3))//case4
				return true;
		}
		else{
			p1 = game.getPiece(pos.x + 1, pos.y + 2);
			p2 = game.getPiece(pos.x + 2, pos.y + 1);
			p3 = game.getPiece(pos.x - 1, pos.y + 2);
			p4 = game.getPiece(pos.x - 2, pos.y + 1);
			if (check(enemy_id, 'N', p1, p2, p3, p4))//case5
				return true;
		}
	}
	else if (pos.y == 7){
		if (pos.x <= 1){
			p1 = game.getPiece(pos.x + 1, pos.y - 2);
			p2 = game.getPiece(pos.x + 2, pos.y - 1);
			p3 = game.getPiece(pos.x - 1, pos.y - 2);
			if (pos.x == 0 && check(enemy_id, 'N', p1, p2))//case6
				return true;
			else if (pos.x == 1 && check(enemy_id, 'N', p1, p2, p3))//case7
				return true;
		}
		else if (pos.x >= 6){
			p1 = game.getPiece(pos.x - 1, pos.y - 2);
			p2 = game.getPiece(pos.x - 2, pos.y - 1);
			p3 = game.getPiece(pos.x + 1, pos.y - 2);
			if (pos.x == 7 && check(enemy_id, 'N', p1, p2))//case8
				return true;
			else if (pos.x == 6 && check(enemy_id, 'N', p1, p2, p3))//case9
				return true;
		}
		else{
			p1 = game.getPiece(pos.x + 1, pos.y - 2);
			p2 = game.getPiece(pos.x + 2, pos.y - 1);
			p3 = game.getPiece(pos.x - 1, pos.y - 2);
			p4 = game.getPiece(pos.x - 2, pos.y - 1);
			if (check(enemy_id, 'N', p1, p2, p3, p4))//case10
				return true;
		}
	}
	else if (pos.x <= 1){
		p1 = game.getPiece(pos.x + 1, pos.y + 2);
		p2 = game.getPiece(pos.x + 2, pos.y + 1);
		p3 = game.getPiece(pos.x + 1, pos.y - 2);
		p4 = game.getPiece(pos.x + 2, pos.y - 1);
		if (check(enemy_id, 'N', p1, p2, p3, p4))
			return true;
	}
	else if (pos.x >= 6){
		p1 = game.getPiece(pos.x - 1, pos.y + 2);
		p2 = game.getPiece(pos.x - 2, pos.y + 1);
		p3 = game.getPiece(pos.x - 1, pos.y - 2);
		p4 = game.getPiece(pos.x - 2, pos.y - 1);
		if (check(enemy_id, 'N', p1, p2, p3, p4))
			return true;
	}
	else{
		cout << "5" << endl;
		p1 = game.getPiece(pos.x + 1, pos.y + 2);
		p2 = game.getPiece(pos.x + 2, pos.y + 1);
		p3 = game.getPiece(pos.x - 1, pos.y + 2);
		p4 = game.getPiece(pos.x - 2, pos.y + 1);
		p5 = game.getPiece(pos.x - 1, pos.y + 2);
		p6 = game.getPiece(pos.x - 2, pos.y + 1);
		p7 = game.getPiece(pos.x - 1, pos.y - 2);
		p8 = game.getPiece(pos.x - 2, pos.y - 1);
		if (check(enemy_id, 'N', p1, p2, p3, p4, p5, p6, p7, p8))
			return true;
	}
	return false;
}
bool check_king(GameBoard &game, char enemy_id, Pos pos){
	string p1, p2, p3, p4, p5, p6, p7, p8;
	if (pos.y == 0){
		p1 = game.getPiece(pos.x, pos.y + 1);
		if (pos.x == 0){
			p2 = game.getPiece(pos.x + 1, pos.y);
			p3 = game.getPiece(pos.x + 1, pos.y + 1);
			if ((p1[0] == enemy_id && p1[1] == 'K')
			 || (p2[0] == enemy_id && p2[1] == 'K')
			 || (p3[0] == enemy_id && p3[1] == 'K'))
				return true;
		}
		else if (pos.x == 7){
			p2 = game.getPiece(pos.x - 1, pos.y);
			p3 = game.getPiece(pos.x - 1, pos.y + 1);
			if ((p1[0] == enemy_id && p1[1] == 'K')
			 || (p2[0] == enemy_id && p2[1] == 'K')
			 || (p3[0] == enemy_id && p3[1] == 'K'))
				return true;
		}
	}
	else if (pos.y == 7){
		p1 = game.getPiece(pos.x, pos.y - 1);
		if (pos.x == 0){
			p2 = game.getPiece(pos.x + 1, pos.y);
			p3 = game.getPiece(pos.x + 1, pos.y - 1);
			if ((p1[0] == enemy_id && p1[1] == 'K')
			 || (p2[0] == enemy_id && p2[1] == 'K')
			 || (p3[0] == enemy_id && p3[1] == 'K'))
				return true;
		}
		else if (pos.x == 7){
			p2 = game.getPiece(pos.x - 1, pos.y);
			p3 = game.getPiece(pos.x - 1, pos.y - 1);
			if ((p1[0] == enemy_id && p1[1] == 'K')
			 || (p2[0] == enemy_id && p2[1] == 'K')
			 || (p3[0] == enemy_id && p3[1] == 'K'))
				return true;
		}
	}
	else if (pos.x == 0){
		p1 = game.getPiece(pos.x, pos.y+1);
		p2 = game.getPiece(pos.x, pos.y-1);
		p3 = game.getPiece(pos.x + 1, pos.y);
		p4 = game.getPiece(pos.x + 1, pos.y + 1);
		p5 = game.getPiece(pos.x + 1, pos.y - 1);
		if ((p1[0] == enemy_id && p1[1] == 'K')
		 || (p2[0] == enemy_id && p2[1] == 'K')
		 || (p3[0] == enemy_id && p3[1] == 'K')
		 || (p4[0] == enemy_id && p4[1] == 'K')
		 || (p5[0] == enemy_id && p5[1] == 'K'))
			return true;
	}
	else if (pos.x == 7){
		p1 = game.getPiece(pos.x, pos.y+1);
		p2 = game.getPiece(pos.x, pos.y-1);
		p3 = game.getPiece(pos.x - 1, pos.y);
		p4 = game.getPiece(pos.x - 1, pos.y + 1);
		p5 = game.getPiece(pos.x - 1, pos.y - 1);
		if ((p1[0] == enemy_id && p1[1] == 'K')
		 || (p2[0] == enemy_id && p2[1] == 'K')
		 || (p3[0] == enemy_id && p3[1] == 'K')
		 || (p4[0] == enemy_id && p4[1] == 'K')
		 || (p5[0] == enemy_id && p5[1] == 'K'))
			return true;
	}
	else{
		p1 = game.getPiece(pos.x, pos.y+1);
		p2 = game.getPiece(pos.x, pos.y-1);
		p3 = game.getPiece(pos.x+1, pos.y);
		p4 = game.getPiece(pos.x-1, pos.y);
		p5 = game.getPiece(pos.x+1, pos.y+1);
		p6 = game.getPiece(pos.x+1, pos.y-1);
		p7 = game.getPiece(pos.x-1, pos.y+1);
		p8 = game.getPiece(pos.x-1, pos.y-1);
		if ((p1[0] == enemy_id && p1[1] == 'K')
		 || (p2[0] == enemy_id && p2[1] == 'K')
		 || (p3[0] == enemy_id && p3[1] == 'K')
		 || (p4[0] == enemy_id && p4[1] == 'K')
		 || (p5[0] == enemy_id && p5[1] == 'K')
		 || (p6[0] == enemy_id && p6[1] == 'K')
		 || (p7[0] == enemy_id && p7[1] == 'K')
		 || (p8[0] == enemy_id && p8[1] == 'K'))
			return true;
	}
	return false;*/
}

bool checkmate(GameBoard &game, Pos pos, char ID){
	char enemy_id = ID + 10*player_check(ID);
	int x = pos.x, y = pos.y;
	int x2 = 7 - x;
	int y2 = 7 - y;

	int diag1 = (x < y) ? x : y;
	int diag2 = (x < y2) ? x : y2;
	int diag3 = (x2 < y2) ? x2 : y2;
	int diag4 = (x2 < y) ? x2 : y;

	int greater_x = (x > x2) ? x : x2;
	int greater_y = (y > y2) ? y : y2;
	int greater = (greater_x > greater_y) ? greater_x : greater_y;

	for (int i = 1; i <= greater; i++){
		string piece;
		Pos end;
		if ((x - i) >= 0){
			piece = game.getPiece(x - i, y);
			end = {x-i, y};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((x2 - i) >= 0){
			piece = game.getPiece(x + i, y);
			end = {x+i, y};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((y - i) >= 0){
			piece = game.getPiece(x, y - i);
			end = {x, y-i};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((y2 - i) >= 0){
			piece = game.getPiece(x, y + i);
			end = {x, y+i};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((diag1 - i) >= 0){
			piece = game.getPiece(x - i, y - i);
			end = {x-i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((diag2 - i) >= 0){
			piece = game.getPiece(x - i, y + i);
			end = {x-i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((diag3 - i) >= 0){
			piece = game.getPiece(x + i, y + i);
			end = {x+i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
		if ((diag4 - i) >= 0){
			piece = game.getPiece(x + i, y - i);
			end = {x+i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return false;
		}
	}
	if (check_pawn(game, enemy_id, pos))
		return false;
	else if (check_horse(game, enemy_id, pos))
		return false;
	else if (check_king(game, enemy_id, pos))
		return false;
	return true;
}

bool validateFormat(string move){
	if (move.length() != 8)
		return false;

	string x_coords = "abcdefgh";
	string y_coords = "12345678";
	string to = move.substr(3, 2);

	if (x_coords.find(move[0]) == -1)
		return false;

	if (y_coords.find(move[1]) == -1)
		return false;

	if (to.compare("to") != 0)
		return false;

	if (x_coords.find(move[6]) == -1)
		return false;

	if (y_coords.find(move[7]) == -1)
		return false;

	return true;
}

int main(void){
	GameBoard game;
	game.display();
	bool play = true;
	string action;
	Player white;
	Player black;
	char turn = ' ';

	while (play){
		if (turn == ' ')
			cout << "White's turn: " << endl;
		else
			cout << "Black's turn: " << endl;
		cout << "Enter a move: ";
		getline(cin, action);
		action.erase(action.length() - 1);
		if (validateFormat(action)){
			int first_space = action.find(" ");
			string coord1 = action.substr(0, first_space);
			Pos pos1 = {(coord1[0] - 'a'), (coord1[1] - '1')};

			int next_coord = action.find(" ", first_space+1) + 1;
			string coord2 = action.substr(next_coord, (action.length() - next_coord));
			Pos pos2 = {(coord2[0] - 'a'), (coord2[1] - '1')};

			cout << "Start: " << pos1.x << ", " << pos1.y << endl;
			cout << "End: " << pos2.x << ", " << pos2.y << endl;

			string piece = game.getPiece(pos1.x, pos1.y);
			char ID = piece[0];

			if (piece[1] == 'P' && turn == ID){
				(turn == ' ') ? white.p.move(game, pos1, pos2) : black.p.move(game, pos1, pos2);
			}
			else if (piece[1] == 'R' && turn == ID){
				(turn == ' ') ? white.r.move(game, pos1, pos2) : black.r.move(game, pos1, pos2);
			}
			else if (piece[1] == 'B' && turn == ID){
				(turn == ' ') ? white.b.move(game, pos1, pos2) : black.b.move(game, pos1, pos2);
			}
			else if (piece[1] == 'N' && turn == ID){
				(turn == ' ') ? white.n.move(game, pos1, pos2) : black.n.move(game, pos1, pos2);
			}
			else if (piece[1] == 'Q' && turn == ID){
				(turn == ' ') ? white.q.move(game, pos1, pos2) : black.q.move(game, pos1, pos2);
			}
			else if (piece[1] == 'K' && turn == ID){
				(turn == ' ') ? white.k.move(game, pos1, pos2) : black.k.move(game, pos1, pos2);
			}
			else
				cout << "Invalid move" << endl;

			if (game.getPiece(pos1.x, pos1.y) == "  ")
				turn = (turn == ' ') ? '*' : ' ';

			game.display();
		}
		else if (action.compare("quit") == 0){
			play = false;
			cout << "quitting..." << endl;
		}
		else{
			cout << "Invalid format. Please re-enter a valid move." << endl;
		}
	}
}
