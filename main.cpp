/*
 * File: Lab_6.4.4_HuanyouWei.cpp
 * Author: Huanyou Wei
 * November 25, 2017
 * Dr. Noukhovitch
 * ICS4U1-01
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

struct Player{
	Pawn *p;
	Rook *r;
	Bishop *b;
	Knight *n;
	Queen *q;
	King *k;
	Pos king;
	bool k_moved = false;
	bool r1_moved = false;
	bool r2_moved = false;
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
			cout << "  +---+---+---+---+---+---+---+---+" << endl;
			cout << row + 1;
			cout <<" |" << matrix[row][0] << " |" << matrix[row][1] << " |" << matrix[row][2] << " |" << matrix[row][3] <<
				   " |" << matrix[row][4] << " |" << matrix[row][5] << " |" << matrix[row][6] << " |" << matrix[row][7] << " |" << endl;
		}
		cout << "  +---+---+---+---+---+---+---+---+" << endl;
		cout << "    a   b   c   d   e   f   g   h " << endl;
	}
	string getPiece(int x, int y){
		return matrix[y][x];
	}
	friend class Piece;
	friend class Pawn;
};

int player_check(char);
bool xy_check(GameBoard&, Pos, Pos);
bool checkmate(GameBoard&, Pos, char);
bool checkmate_3(GameBoard&, Pos, Pos, Pos, char);
bool check_move(GameBoard&, Pos, Pos);

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
				|| (final != "  " && final[0] == enemy_id && (dx == 1 && dy == 1)))){
				Piece::move(game, start, end);
				if (end.y % 7 == 0){
					promote(current[0], final);
					game.matrix[end.y][end.x] = final;
				}
			}
			else
				cout << "Invalid move" << endl;
		}
	}
	void promote(char ID, string &final){
		final = ID;
		final += 'Q';
	}
};

class Rook: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end, Player &player){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);
		char enemy_id = current[0] + 10*player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);
		if (current != "  "){
			if (((dx != 0 && dy == 0) || (dx == 0 && dy != 0)) && xy_check(game, start, end)
				&& (final[0] == enemy_id || final == "  ")){
				Piece::move(game, start, end);
				if (start.x == 0)
					player.r1_moved = true;
				else if (start.y == 7)
					player.r2_moved = true;
			}
			else
				cout << "Invalid move" << endl;
		}
	}
};

class Bishop: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);
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
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);
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
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);
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
	void move(GameBoard &game, Pos start, Pos end, Player &player){
		string current = game.getPiece(start.x, start.y);
		char id = current[0];
		int dx = abs(end.x-start.x);
		if (current != "  "){
			if (check_move(game, start, end)){
				Piece::move(game, start, end);
				player.king = end;
				player.k_moved = true;
				return;
			}
			else if (!player.k_moved && dx == 2){
				Pos pos2, pos3;
				if (end.x == start.x - 2 && !player.r1_moved){
					pos2 = {start.x - 1, start.y};
					pos3 = {start.x - 2, start.y};
					if (xy_check(game, start, {0, 0}) && checkmate_3(game, start, pos2, pos3, id)){
						Piece::move(game, start, end);
						Piece::move(game, {0, 0}, pos2);
						player.king = end;
						player.k_moved = true;
						return;
					}
				}
				else if (end.x == start.x + 2 && !player.r2_moved){
					pos2 = {start.x + 1, start.y};
					pos3 = {start.x + 2, start.y};
					if (xy_check(game, start, {7, 0}) && checkmate_3(game, start, pos2, pos3, id)){
						Piece::move(game, start, end);
						Piece::move(game, {7, 0}, pos2);
						player.king = end;
						player.k_moved = true;
						return;
					}
				}
			}
			cout << "Invalid move" << endl;
		}
	}
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
	return (piece[0] == enemy_id && piece[1] == piece_id);
}
bool check_cases(GameBoard &game, char enemy_id, char piece_id, Pos arr[8]){
	for (int i = 0; i < 8; i++){
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			string piece = game.getPiece(arr[i].x, arr[i].y);
			if (check_piece(enemy_id, piece_id, piece))
				return true;
		}
	}
	return false;
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

	Pos king_arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	Pos horse_arr[8] = {{x+2, y+1}, {x+1, y+2}, {x-1, y+2}, {x-2, y+1}, {x-2, y-1}, {x-1, y-2}, {x+1, y-2}, {x+2, y-1}};

	for (int i = 1; i <= greater; i++){
		string piece;
		Pos end;
		if ((x - i) >= 0){
			piece = game.getPiece(x - i, y);
			end = {x-i, y};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((x2 - i) >= 0){
			piece = game.getPiece(x + i, y);
			end = {x+i, y};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((y - i) >= 0){
			piece = game.getPiece(x, y - i);
			end = {x, y-i};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((y2 - i) >= 0){
			piece = game.getPiece(x, y + i);
			end = {x, y+i};
			if (check_straight(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((diag1 - i) >= 0){
			piece = game.getPiece(x - i, y - i);
			end = {x-i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((diag2 - i) >= 0){
			piece = game.getPiece(x - i, y + i);
			end = {x-i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((diag3 - i) >= 0){
			piece = game.getPiece(x + i, y + i);
			end = {x+i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
		if ((diag4 - i) >= 0){
			piece = game.getPiece(x + i, y - i);
			end = {x+i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(game, pos, end))
				return true;
		}
	}
	if (check_pawn(game, enemy_id, pos))
		return true;
	else if (check_cases(game, enemy_id, 'N', horse_arr))
		return true;
	else if (check_cases(game, enemy_id, 'K', king_arr))
		return true;
	return false;
}
bool checkmate_3(GameBoard &game, Pos start, Pos pos2, Pos pos3, char id){
	return (!checkmate(game, start, id) && !checkmate(game, pos2, id) && !checkmate(game, pos3, id));
}
bool checkmate_all(GameBoard &game, Pos pos, char ID){
	int x = pos.x, y = pos.y;
	Pos arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	for (int i = 0; i < 8; i++){
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			if (!checkmate(game, arr[i], ID))//No checkmate for this pos
				return false;
		}
	}
	return true;
}
bool king_is_alone(GameBoard &game, char ID){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			string p = game.getPiece(i, j);
			if (p[0] == ID && (p[1] == 'P' || p[1] == 'R' || p[1] == 'B' || p[1] == 'N' || p[1] == 'Q'))
				return false;
		}
	}
	return true;
}
bool check_move(GameBoard &game, Pos start, Pos end){
	string current = game.getPiece(start.x, start.y);
	string final = game.getPiece(end.x, end.y);
	char id = current[0];
	char enemy_id = id + 10*player_check(id);
	int dx = abs(end.x-start.x);
	int dy = abs(end.y-start.y);
	return ((dx <= 1 && dy <= 1) && (dx != 0 || dy != 0) && !checkmate(game, end, id)
			&& (final[0] == enemy_id || final == "  "));
}
bool king_is_stuck(GameBoard &game, Pos pos){
	int x = pos.x, y = pos.y;
	Pos arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	for (int i = 0; i < 8; i++){
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			if (check_move(game, pos, arr[i]))//No checkmate for this pos
				return false;
		}
	}
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

	cout << "Welcome to Text-Based Chess!\n" << endl;
	cout << "Moves should be inputted in the form: x# to x#" << endl;
	cout << "The range of x is a-g & the range of # is 1-8\n" << endl;
	cout << "Please enter valid moves according to the rules of chess" << endl;
	cout << "Otherwise, the program will not accept the input\n" << endl;
	cout << "If you would like to exit the game, " << endl;
	cout << "enter 'quit' into the input\n" << endl;
	cout << "Enjoy!\n" << endl;

	GameBoard game;
	game.display();
	bool play = true;
	string action;
	Player white, black;
	white.king = {4, 0}, black.king = {4, 7};
	char turn = ' ';

	while (play){
		if (turn == ' '){
			if (checkmate(game, black.king, '*')){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else if (king_is_alone(game, ' ') && checkmate_all(game, white.king, ' ')){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else if (checkmate(game, white.king, ' ') && king_is_stuck(game, white.king)){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else
				cout << "White's turn: " << endl;
		}
		else{
			if (checkmate(game, white.king, ' ')){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else if (king_is_alone(game, '*') && checkmate_all(game, black.king, '*')){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else if (checkmate(game, black.king, '*') && king_is_stuck(game, black.king)){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else
				cout << "Black's turn: " << endl;
		}
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

			string piece = game.getPiece(pos1.x, pos1.y);
			char player_ID = piece[0], piece_ID = piece[1];

			if (piece_ID == 'P' && turn == player_ID){
				(turn == ' ') ? (*white.p).move(game, pos1, pos2) : (*black.p).move(game, pos1, pos2);
			}
			else if (piece_ID == 'R' && turn == player_ID){
				(turn == ' ') ? (*white.r).move(game, pos1, pos2, white) : (*black.r).move(game, pos1, pos2, black);
			}
			else if (piece_ID == 'B' && turn == player_ID){
				(turn == ' ') ? (*white.b).move(game, pos1, pos2) : (*black.b).move(game, pos1, pos2);
			}
			else if (piece_ID == 'N' && turn == player_ID){
				(turn == ' ') ? (*white.n).move(game, pos1, pos2) : (*black.n).move(game, pos1, pos2);
			}
			else if (piece_ID == 'Q' && turn == player_ID){
				(turn == ' ') ? (*white.q).move(game, pos1, pos2) : (*black.q).move(game, pos1, pos2);
			}
			else if (piece_ID == 'K' && turn == player_ID){
				(turn == ' ') ? (*white.k).move(game, pos1, pos2, white) : (*black.k).move(game, pos1, pos2, black);
			}
			else
				cout << "Invalid move" << endl;

			if (game.getPiece(pos1.x, pos1.y) == "  " && piece != "  ")
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
