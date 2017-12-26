/*
 * File: Lab_6.4.4_HuanyouWei.cpp
 * Author: Huanyou Wei
 * November 20, 2017
 * Dr. Noukhovitch
 * ICS4U1-01
 */

/*
 * Summary: This program creates a Text Based Chess game using inheritance
 */

//////////////////////////////////////////////////////////////////////////////
//Dr. Noukhovitch. If this program doesn't work, please comment out line 950//
//Or, comment out the line which contains: action.erase(action.length()-1); //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

using namespace std;

//Declare the Piece classes to be used in the Player struct
class Piece;
class Pawn;
class Rook;
class Bishop;
class Knight;
class Queen;
class King;

//Create a Pos struct to hold the coordinates of a piece
struct Pos{
	int x, y;
};

//Create a Player struct to hold a Player's pieces, the king's position (used for checkmate),
//and bool statements to determine whether or not a king or rook has moved (used for castling)
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

/*
 * class GameBoard - creates a GameBoard class that holds a matrix of the chess board and its pieces
 * 				  	Fields:
 * 				  		   matrix - an array with 8 rows and 8 columns to represent a
 * 				  		   			board and chess pieces (string matrix[8][8])
 * 				  	Friends:
 * 				  			Piece - modifies the string matrix in the move method
 * 				  			Pawn - modifies teh string matrix to promote the Pawn
 */
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
	void display(void);
	string getPiece(int x, int y);
	int player_check(char);
	bool xy_check(Pos, Pos);
	bool check_straight(char, string);
	bool check_diagonal(char, string);
	bool check_pawn(char, Pos);
	bool check_piece(char, char, string);
	bool check_cases(char, char, Pos[8]);
	bool checkmate(Pos, char);
	bool checkmate_3(Pos, Pos, Pos, char);
	bool checkmate_all(Pos, char);
	bool check_move(Pos, Pos);
	bool king_is_alone(char);
	bool king_is_stuck(Pos);

	friend class Piece;
	friend class Pawn;
};

/*
 * GameBoard::display - displays the chess board
 */
void GameBoard::display(void){
	for (int row = 7; row >= 0; row--){
		cout << "  +---+---+---+---+---+---+---+---+" << endl;
		cout << row + 1;
		cout <<" |" << matrix[row][0] << " |" << matrix[row][1] << " |" << matrix[row][2] << " |" << matrix[row][3] <<
			   " |" << matrix[row][4] << " |" << matrix[row][5] << " |" << matrix[row][6] << " |" << matrix[row][7] << " |" << endl;
	}
	cout << "  +---+---+---+---+---+---+---+---+" << endl;
	cout << "    a   b   c   d   e   f   g   h " << endl;
}

/*
 * GameBoard::getPiece - gets the string at a pos in the chess board matrix
 *
 * @return matrix[y][x] - the string at a desired position
 */
string GameBoard::getPiece(int x, int y){
	return matrix[y][x];
}

/*
 * GameBoard::play_check - checks the player_id of piece
 * 			  - If it's '*', return -1 (represents down, the direction of movement)
 * 			  - Otherwise, if it's ' ', return 1 (means up, the direction of movement)
 * @param player_id - the id of the player ('*' or ' ')
 * @return -1 - id is '*', direction down
 * @return 1 - id is ' ', direction up
 */
int GameBoard::player_check(char player_id){
	if (player_id == '*')
		return -1;
	else
		return 1;
}

/*
 * GameBoard::xy_check - checks if there are any pieces in between a piece and it's destination
 *
 * @param start - the initial position of the piece
 * @param end - the destination of the piece
 * @return false - there are pieces between the start and the end OR
 * 				 - the start and end positions are the same
 * @return true - there are no pieces between the start and the end
 */
bool GameBoard::xy_check(Pos start, Pos end){
	//Get the difference in the start and end x and y coords
	int dx = end.x - start.x;
	int dy = end.y - start.y;

	//Get the direction of movement in the x and y directions
	//If dx >= 0, the direction is right (1)
	//Otherwise the direction is left (-1)
	int dirx = dx >= 0 ? 1 : -1;
	int diry = dy >= 0 ? 1 : -1;

	//Make the differences positive with the abs() function
	dx = abs(dx);
	dy = abs(dy);

	//Make sure that the start and end position aren't the same (dy and dx cannot both equal to 0)
	if (dy != 0 || dx != 0){
		//If the dy == 0, then the piece is trying to move horizontally
		if (dy == 0){
			//Create a loop that iterates (dx - 1) times
			//(Exclude the final destination since pieces can capture other)
			for (int i = 1; i < dx; i++){
				//Check if there are pieces in the tiles between the start and end pos
				if (getPiece(start.x + i*dirx, start.y) != "  ")
					return false;
			}
			return true;
		}
		//Else if dx == 0, then the piece is trying to move vertically
		else if (dx == 0){
			for (int i = 1; i < dy; i++){
				if (getPiece(start.x, start.y + i*diry) != "  ")
					return false;
			}
			return true;
		}
		//Else if dx == dy, then the piece is trying to move diagonally
		else if (dx == dy){
			for (int i = 1; i < dx; i++){
				if (getPiece(start.x + i*dirx, start.y + i*diry) != "  ")
					return false;
			}
			return true;
		}
	}
	return false;
}

/*
 * GameBoard::check_straight - checks if there are any enemy Rooks or Queens in a tile
 * 				  - (used for horizontal and vertical checkmate checks)
 *
 * @param enemy_id - the id of the enemy player (ie. if player_id is ' ', enemy_id is '*')
 * @param piece - the tile to be checked
 * @return false - there is a Rook or Queen in the tile
 * @return true - there is a Rook or Queen in the tile
 */
bool GameBoard::check_straight(char enemy_id, string piece){
	return (piece[0] == enemy_id && (piece[1] == 'R' || piece[1] == 'Q'));
}

/*
 * GameBoard::check_diagonal - checks if there are any enemy Bishops or Queens in a tile
 * 				  - (used for diagonal checkmate checks)
 *
 * @param enemy_id - the id of the enemy player
 * @param piece - the tile to be checked
 * @return false - there is a Bishop or Queen in the tile
 * @return true - there is a Bishop or Queen in the tile
 */
bool GameBoard::check_diagonal(char enemy_id, string piece){
	return (piece[0] == enemy_id && (piece[1] == 'B' || piece[1] == 'Q'));
}

/*
 * GameBoard::check_pawn - checks if there are enemy pawns checkmating the king
 * 				(Usually one tile diagonally forward or backwards depending on
 * 				the direction of the pawn's movement)
 *
 * @param enemy_id - the id of the enemy player
 * @param pos - the king's position
 * @return false - there are no pawn's checkmating the king
 * @return true - there are pawns checkmating the king
 */
bool GameBoard::check_pawn(char enemy_id, Pos pos){
	//Get the enemy's direction (determines pawn's direction)
	int dir = player_check(enemy_id);
	//Declare 2 string vars to hold the string of the pawn locations relative to the king's pos
	string piece1, piece2;

	//If the king is at the left side of the board
	if (pos.x == 0){
		//Get the string of the tile diagonally right by 1 and check if it's an enemy pawn
		piece1 = getPiece(pos.x + 1, pos.y - dir);
		if (piece1[0] == enemy_id && piece1[1] == 'P')
			return true;
	}
	//If the king is at the right side of the board
	else if (pos.x == 7){
		//Get the string of the tile diagonally left by 1 and check if it's an enemy pawn
		piece1 = getPiece(pos.x - 1, pos.y - dir);
		if (piece1[0] == enemy_id && piece1[1] == 'P')
			return true;
	}
	//Otherwise, if the king is in the middle of the board
	else{
		//Get the strings of the tiles diagonally left and right by 1 and check if one is an enemy pawn
		piece1 = getPiece(pos.x + 1, pos.y - dir);
		piece2 = getPiece(pos.x - 1, pos.y - dir);
		if ((piece1[0] == enemy_id && piece1[1] == 'P') || (piece2[0] == enemy_id && piece2[1] == 'P'))
			return true;
	}
	return false;
}

/*
 * GameBoard::check_piece - check if a piece is an enemy and if it's equal to the given piece
 *
 * @param enemy_id - the id of the enemy player
 * @param piece_id - the id to compare the piece's id to (piece string)
 * @param piece - the string of the piece
 * @return false - piece[1] != to the piece_id OR
 * 			     - piece[0] != to the enemy_id
 * @return true - the piece matches the enemy_id and piece_id
 */
bool GameBoard::check_piece(char enemy_id, char piece_id, string piece){
	return (piece[0] == enemy_id && piece[1] == piece_id);
}

/*
 * GameBoard::check_cases - checks possible cases (max of 8) of checkmate a king may be subject to by a Knight or King
 *
 * @param enemy_id - the id of the enemy player
 * @param piece_id - the id to compare the piece's id to (piece string)
 * @param arr[8] - an array of the possible positions the enemy pieces can be in
 * @return false - none of the possible positions have the specified pieces
 * @return true - one of the positions has a specified piece
 */
bool GameBoard::check_cases(char enemy_id, char piece_id, Pos arr[8]){
	//Create a loop that iterates 8 times
	for (int i = 0; i < 8; i++){
		//Check if the position is within the range of the board
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			string piece = getPiece(arr[i].x, arr[i].y);
			//Check if the piece corresponds to the enemy_id and the piece_id
			if (check_piece(enemy_id, piece_id, piece))
				return true;
		}
	}
	return false;
}

/*
 * GameBoard::checkmate - checks if a tile is being checkmated
 *
 * @param pos - the position to be checked
 * @param ID - the id of the piece to be checked
 * @return false - the piece is not being checkmated
 * @return true - the piece is being checkmated
 */
bool GameBoard::checkmate(Pos pos, char ID){
	//Get the enemy id
	char enemy_id = ID + 10*player_check(ID);

	//Assign pos.x to x and pos.y to y to improve readability
	int x = pos.x, y = pos.y;

	//Assign the length of x from pos.x to the left side of the board to x2
	//Assign the height of y from pos.y to the top of the board to y2
	int x2 = 7 - x;
	int y2 = 7 - y;

	//Get the diagonal lengths from the sides of the board to the piece
	//Corresponds to smallest x or y in a pair
	int diag1 = (x < y) ? x : y;
	int diag2 = (x < y2) ? x : y2;
	int diag3 = (x2 < y2) ? x2 : y2;
	int diag4 = (x2 < y) ? x2 : y;

	//Get the greater x and y and compare them to get the greatest difference from the pos to the board edge
	int greater_x = (x > x2) ? x : x2;
	int greater_y = (y > y2) ? y : y2;
	int greater = (greater_x > greater_y) ? greater_x : greater_y;

	//Create 2 array of the King and Knight checkmate cases from the pos
	Pos king_arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	Pos horse_arr[8] = {{x+2, y+1}, {x+1, y+2}, {x-1, y+2}, {x-2, y+1}, {x-2, y-1}, {x-1, y-2}, {x+1, y-2}, {x+2, y-1}};

	//Starting from 1 (i = 1), iterate n times (n = greater)
	for (int i = 1; i <= greater; i++){
		//Declare an end Pos to hold the position to be checked and a piece str to hold the str at that pos
		Pos end;
		string piece;

		//Check if the coordinates are within range of the board
		if ((x - i) >= 0){
			piece = getPiece(x - i, y);
			end = {x-i, y};
			//Check if there are enemy pieces that can checkmate the king and if there are no pieces in between them
			if (check_straight(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((x2 - i) >= 0){
			piece = getPiece(x + i, y);
			end = {x+i, y};
			if (check_straight(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((y - i) >= 0){
			piece = getPiece(x, y - i);
			end = {x, y-i};
			if (check_straight(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((y2 - i) >= 0){
			piece = getPiece(x, y + i);
			end = {x, y+i};
			if (check_straight(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((diag1 - i) >= 0){
			piece = getPiece(x - i, y - i);
			end = {x-i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((diag2 - i) >= 0){
			piece = getPiece(x - i, y + i);
			end = {x-i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((diag3 - i) >= 0){
			piece = getPiece(x + i, y + i);
			end = {x+i, y+i};
			if (check_diagonal(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
		if ((diag4 - i) >= 0){
			piece = getPiece(x + i, y - i);
			end = {x+i, y-i};
			if (check_diagonal(enemy_id, piece) && xy_check(pos, end))
				return true;
		}
	}
	//Check if the pos will be checkmated by pawns
	if (check_pawn(enemy_id, pos))
		return true;
	//Check if the pos will be checkmated by Knights
	else if (check_cases(enemy_id, 'N', horse_arr))
		return true;
	//Check if the pos will be checkmated by Kings
	else if (check_cases(enemy_id, 'K', king_arr))
		return true;

	return false;
}
/*
 * GameBoard::checkmate_3 - checks if 3 consecutive positions are in checkmate (doesn't have to be simultaneously)
 *
 * @param start - the first pos (initial King pos)
 * @param pos2 - the 2nd pos (king pos + 1 left or right)
 * @param po3 - the 3rd pos (king pos + 2 left or right)
 * @param id - the player's id
 * @return false - one of the pos is in checkmate
 * @return true - none of the pos are in checkmate
 */
bool GameBoard::checkmate_3(Pos start, Pos pos2, Pos pos3, char id){
	return (!checkmate(start, id) && !checkmate(pos2, id) && !checkmate(pos3, id));
}

/*
 * GameBoard::checkmate_all - checks if all the possible moves are under checkmate
 *
 * @param pos - the King's pos
 * @param ID - the player's id
 * @return false - one of the possible positions aren't in checkmate
 * @return true - all the possible positions are in checkmate
 */
bool GameBoard::checkmate_all(Pos pos, char ID){
	//Assign pos.x to x and pos.y to y to improve readability
	int x = pos.x, y = pos.y;

	//Create a array of all the possible moves a king can make (excluding castling)
	Pos arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	//Create a for loop that iterates 8 times
	for (int i = 0; i < 8; i++){
		//Check if the position is within the range of the board
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			//If there is no checkmate at one of the possible positions, return false
			if (!checkmate(arr[i], ID))
				return false;
		}
	}
	//If all the possible positions are in checkmate, return true
	return true;
}

/*
 * GameBoard::check_move - checks if a king's move is valid
 *
 * @param start - the start pos
 * @param end - the end pos
 * @return false - the king's move isn't valid
 * @return true - the king's move is valid
 */
bool GameBoard::check_move(Pos start, Pos end){
	//Get the strings at the start and end positions
	string current = getPiece(start.x, start.y);
	string final = getPiece(end.x, end.y);

	//Get the player id, enemy id and the difference in x and y coords
	//abs() is used since Kings can move forwards and backwards
	char id = current[0];
	char enemy_id = id + 10*player_check(id);
	int dx = abs(end.x-start.x);
	int dy = abs(end.y-start.y);

	//A King has 3 Possible moves excluding castling
	/*
	 * If the King isn't blocked or won't go into checkmate...
	 * Case 1: Check if the move is 1 tile horizontally right or left (dx == 1 && dy == 0)
	 * Case 2: Check if the move is 1 tile vertically up or down (dx == 0 && dy == 1)
	 * Case 3: Check if the move is 1 tile diagonally forward or backwards (dx == 1 && dy == 1)
	 */
	//If any of these moves is valid, return true
	return ((dx <= 1 && dy <= 1) && (dx != 0 || dy != 0) && !checkmate(end, id)
			&& (final[0] == enemy_id || final == "  "));
}

/*
 * GameBoard::king_is_alone - checks if the king is the only piece the player has left
 *
 * @param ID - the player's id
 * @return false - the king is not the only piece the player has left
 * @return true - the king is the only piece left on the player's side
 */
bool GameBoard::king_is_alone(char ID){
	//Loop through the string matrix (chess board)
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			//Get the p string at the tile as the loop continues
			string p = getPiece(i, j);
			//If p string contains the player's ID and a piece other than King, return false (king is not alone)
			if (p[0] == ID && (p[1] == 'P' || p[1] == 'R' || p[1] == 'B' || p[1] == 'N' || p[1] == 'Q'))
				return false;
		}
	}
	//Otherwise, if the king is alone, return true
	return true;
}

/*
 * GameBoard::king_is_stuck - checks if the king is stuck (is it able to move?)
 * 				 			- (or is it blocked by other pieces or can't move into checkmate)
 *
 * @param pos - the position of the king
 * @return false - the king isn't stuck
 * @return true - the king is stuck
 */
bool GameBoard::king_is_stuck(Pos pos){
	//Assign pos.x to x and pos.y to y to improve readability
	int x = pos.x, y = pos.y;

	//Create a Pos array of all the possible moves the king can make (no castling)
	Pos arr[8] = {{x+1, y}, {x+1, y+1}, {x, y+1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}, {x, y-1}, {x+1, y-1}};
	//Create a for loop that iterates 8 times
	for (int i = 0; i < 8; i++){
		//Check if the position is within the range of the board
		if ((arr[i].x <= 7 && arr[i].x >= 0) && (arr[i].y <= 7 && arr[i].y >= 0)){
			//If the move in the array is possible, return false
			if (check_move(pos, arr[i]))
				return false;
		}
	}
	//Otherwise, if the king is stuck and no moves are possible, return true
	return true;
}

/*
 * class Piece - acts as a superclass to the Pawn, Rook, Bishop, Queen and King classes
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end)
 * 						- moves a string at the start position to the end position (in the string matrix)
 * 						- fills the start position with an default string: "  " (piece is no longer in the start pos)
 */
class Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		game.matrix[end.y][end.x] = game.matrix[start.y][start.x];
		game.matrix[start.y][start.x] = "  ";
		}
};

/*
 * class Pawn - the Pawn subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end)
 * 						- checks the Pawn's start and end positions to validate the move
 * 						- if the Pawn has a valid start and end, call the Piece superclass's move function
 * 						- Otherwise, tell the player the move is invalid
 *
 * 						void promote(char ID, string &final)
 * 						- promotes a Pawn to a Queen when it reaches the end of a board (subject to player's side)
 *
 */
class Pawn: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		//Get the strings at the start and end positions
		string current = game.matrix[start.y][start.x];
		string final = game.matrix[end.y][end.x];

		//Get the enemy id and the difference in x and y coords
		//dy has no abs() since the pawn can only move in one direction
		char enemy_id = current[0] + 10*game.player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = end.y-start.y;

		//Check if the string at the start position isn't empty
		if (current != "  "){

			//Determine the Pawn's direction according to its id (' ' or '*')
			//Pass it into the player_check function which will return either 1 or -1 depending on the id
			//Let dy = dy * dir to validate the pawn's movement (is it going in the right direction?)
			int dir = game.player_check(current[0]);
			dy *= dir;
			//A Pawn has 3 different moves - check for those cases
			/*
			 * Case 1: If the pawn is at it's starting position (y % 5 == 1) and not blocked,
			 * 		   check if the move is 2 tiles forward (dy = 2 and dx = 0)
			 * Case 2: If the pawn is not blocked, check if the move is 1 tile forward (dy = 1, dx = 0)
			 * Case 3: If there are pieces in the tiles one diagonal forward,
			 * 		   check if the move is 1 tile forward and 1 tile to the side (left or right) (dy = 1, dx = 1)
			 */
			if (((((start.y%5 == 1 && dy == 2) || dy == 1) && dx == 0 && game.xy_check(start, end) && final == "  ")
				|| (final != "  " && final[0] == enemy_id && (dx == 1 && dy == 1)))){

				//If any of these are true, call the Piece class's move method
				//If the y position is at the end of the board, promote the Pawn into a Queen
				Piece::move(game, start, end);
				if (end.y % 7 == 0){
					promote(current[0], final);
					game.matrix[end.y][end.x] = final;
				}
			}
			//Otherwise, if the cases are false, tell the user their move is invalid
			else
				cout << "Invalid move" << endl;
		}
	}
	void promote(char ID, string &final){
		//Promote a pawn at the end of the board to a queen
		//Concatenate the strings with the += operator or the values will are seen as chars
		//(If the values are seen as chars, the int values of the char are added up and not the characters themselves)
		final = ID;
		final += 'Q';
	}
};

/*
 * class Rook - the Rook subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end, Player &player)
 * 						- checks the Rook's start and end positions to validate the moves
 * 						  (dx != 0 && dy == 0) or (dx == 0 && dy != 0)
 * 						- If the positions are valid, call the Piece superclass's move function
 * 						  and modify the r#_moved boolean variable (used for castling)
 * 						- Otherwise, tell the player the move is invalid
 */
class Rook: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end, Player &player){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);

		//abs() is used since Rooks can move forwards and backwards
		char enemy_id = current[0] + 10*game.player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);

		if (current != "  "){
			//A Rook can move horizontally or vertically, there are 2 possible cases
			/*
			 * Case 1: If the Rook isn't obstructed, check if the move is horizontally
			 * 		   left or right (dx != 0 && dy == 0)
			 * Case 2: If the Rook isn't blocked, check if the move is vertically
			 * 		   up or down (dx == 0 && dy != 0)
			 */
			//The Rook can move to empty spaces or capture enemy pieces in it's line of movement
			if (((dx != 0 && dy == 0) || (dx == 0 && dy != 0)) && game.xy_check(start, end)
				&& (final[0] == enemy_id || final == "  ")){

				Piece::move(game, start, end);

				//If the Rook moved, set the r#_moved variable to true
				//If the Rook was in the left corner (x == 0), set r1_moved = true
				//Otherwise, if the rook was in the right corner (x == 7), set r2_moved = true
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

/*
 * class Bishop - the Bishop subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end)
 * 						- checks the Bishop's start and end positions to validate the moves
 * 						  (dx == dy)
 * 						- If the positions are valid, call the Piece superclass's move function
 * 						- Otherwise, tell the player the move is invalid
 */
class Bishop: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);

		char enemy_id = current[0] + 10*game.player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);

		if (current != "  "){
			//A Bishop can only move diagonally, there is only 1 case
			//Case 1: If the Bishop isn't blocked, check if the move is diagonal (dx == dy)
			//The Bishop can move to empty spaces or capture enemy pieces in it's line of movement
			if (dx == dy && game.xy_check(start, end) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

/*
 * class Knight - the Knight subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end)
 * 						- checks the Knight's start and end positions to validate the moves
 * 						  (dx == 2 && dy == 1) or (dx == 1 && dy == 2)
 * 						- If the positions are valid, call the Piece superclass's move function
 * 						- Otherwise, tell the player the move is invalid
 */
class Knight: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);

		char enemy_id = current[0] + 10*game.player_check(current[0]);
		int dx = abs(end.x-start.x);
		int dy = abs(end.y-start.y);

		if (current != "  "){
			//A Knight can only move in a 'L' shape, there is 1 case
			//Case 1: If the Knight isn't blocked, check if it move is 2 tiles up and 1 tile left, etc.
			//(dx == 1 && dy == 2) or (dx == 2 && dy == 1)
			//The Knight can move to empty spaces or capture enemy pieces in it's line of movement
			if (((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

/*
 * class Queen - the Queen subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end)
 * 						- checks the Queen's start and end positions to validate the moves
 * 						  (dx != 0 && dy == 0) or (dx == 0 && dy != 0) or (dx == dy)
 * 						- If the positions are valid, call the Piece superclass's move function
 * 						- Otherwise, tell the player the move is invalid
 */
class Queen: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end){
		string current = game.getPiece(start.x, start.y);
		string final = game.getPiece(end.x, end.y);

		char enemy_id = current[0] + 10*game.player_check(current[0]);

		if (current != "  "){
			//A Queen can move horizontally, vertically and diagonally. There are 3 cases
			/*
			 * Case 1: If the Queen isn't blocked, check if the move is horizontally
			 * 		   left or right (dx != 0 && dy == 0)
			 * Case 2: If the Queen isn't blocked, check if the move is vertically
			 * 		   up or down (dx == 0 && dy != 0)
			 * Case 3: If the Queen isn't halted, check if the move is diagonal
			 * 		   (dx == dy)
			 */
			if (game.xy_check(start, end) && (final[0] == enemy_id || final == "  "))
				Piece::move(game, start, end);
			else
				cout << "Invalid move" << endl;
		}
	}
};

/*
 * class King - the King subclass of the Piece superclass
 * 				Methods:
 * 						void move(GameBoard &game, Pos start, Pos end,  Player &player)
 * 						- checks the King's start and end positions to validate the moves
 *						  (dx == 1 && dy == 0) or (dx == 0 && dy == 1) or (dx == 1 && dy == 1)
 *						  or castling: (dx == 2 && dy == 0)
 * 						- If the positions are valid, call the Piece superclass's move function
 * 						- Once the king has moved, set the k_moved bool var to true (used for castling)
 * 						- Otherwise, tell the player the move is invalid
 */
class King: public Piece{
public:
	void move(GameBoard &game, Pos start, Pos end, Player &player){
		string current = game.getPiece(start.x, start.y);

		//Get the player id of the piece (ie. ' ' or '*')
		char id = current[0];
		int dx = abs(end.x-start.x);

		//If the string at the start pos isn't empty
		if (current != "  "){
			//Call the check_move function, checks for 3 of the King's possible moves
			/*
			 * If the King isn't blocked or won't go into checkmate...
			 * Case 1: Check if the move is 1 tile horizontally right or left (dx == 1 && dy == 0)
			 * Case 2: Check if the move is 1 tile vertically up or down (dx == 0 && dy == 1)
			 * Case 3: Check if the move is 1 tile diagonally forward or backwards (dx == 1 && dy == 1)
			 */
			if (game.check_move(start, end)){
				Piece::move(game, start, end);
				player.king = end;//Update the king's location for the Player struct
				player.k_moved = true;//Update the k_moved bool var (the king moved)
				return;//Terminate function execution
			}
			//If all those cases are false, check the final case: CASTLING
			/*
			 * If the King isn't blocked, hasn't moved, isn't in check,
			 * won't go over check, and won't go into check...
			 * Case 4: Check if the move is 2 tiles horizontally right or left (dx == 2 && dy == 0)
			 */
			else if (!player.k_moved && dx == 2){
				//Initialize Pos vars to check for checkmate
				Pos pos2, pos3;

				//Check which Rook the King is castling towards

				//If the King's final position is 2 tiles to the left, then the Rook is in the left corner
				//Thus, check if the corresponding bool var, r1_moved, is false
				//(this must be the left corner Rook's first movement)
				if (end.x == start.x - 2 && !player.r1_moved){

					//Initialize the Pos vars with the coords 1 and 2 tiles to the left
					pos2 = {start.x - 1, start.y};
					pos3 = {start.x - 2, start.y};

					//Check if there are no other pieces between the King and the Rook
					//And if the king's start pos, pos2, and pos3 aren't in the checkmate
					if (game.xy_check(start, {0, 0}) && game.checkmate_3(start, pos2, pos3, id)){

						Piece::move(game, start, end);//If true, move the King 2 tiles to the left
						Piece::move(game, {0, 0}, pos2);//Move the Rook to pos2 (1 tile to the left of the king's start pos)
						player.king = end;
						player.k_moved = true;
						return;
					}
				}
				//Else if the King's final pos is 2 tiles to the right, the the Rook is in the right corner
				//Thus, check if the corresponding bool var, r2_moved, is false
				//(must be the right corner Rook's first movement)
				else if (end.x == start.x + 2 && !player.r2_moved){

					//Initialize the Pos vars with the coords 1 and 2 tiles to the right
					pos2 = {start.x + 1, start.y};
					pos3 = {start.x + 2, start.y};

					//Check if there are no other pieces between the King and the Rook
					//And if the king's start pos, pos2, and pos3 aren't in the checkmate
					if (game.xy_check(start, {7, 0}) && game.checkmate_3(start, pos2, pos3, id)){

						Piece::move(game, start, end);//If true, move the King 2 tiles to the right
						Piece::move(game, {7, 0}, pos2);//Move the Rook to pos2 (1 tile to the right of the king's start pos)
						player.king = end;
						player.k_moved = true;
						return;
					}
				}
			}
			//Otherwise, if all the cases are false, tell the user their move is invalid
			cout << "Invalid move" << endl;
		}
	}
};

/*
 * validateFormat - checks if the user inputted move is correct
 *
 * @param move - the user's move
 * @return false - move is incorrect
 * @return true - move is correct
 */
bool validateFormat(string move){
	//Check if the length of the move is 8 (the length of the move will always be the same)
	if (move.length() != 8)
		return false;

	//Create strings to hold the letters and numbers to be checked
	string x_coords = "abcdefgh";
	string y_coords = "12345678";
	string to = move.substr(3, 2);

	//Check if any of the chars in the move can be found in the strings
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

	//The move is valid!
	return true;
}

int main(void){

	//Welcome the user to the game and state some rules
	cout << "Welcome to Text-Based Chess!\n" << endl;
	cout << "Moves should be inputted in the form: x# to x#" << endl;
	cout << "The range of x is a-g & the range of # is 1-8\n" << endl;
	cout << "Please enter valid moves according to the rules of chess" << endl;
	cout << "Otherwise, the program will not accept the input\n" << endl;
	cout << "If you would like to exit the game, " << endl;
	cout << "enter 'quit' into the input\n" << endl;
	cout << "Enjoy!\n" << endl;

	//Declare the objects, flags, Players, and strings
	//Initialize the turns and each player's king's position
	GameBoard game;
	game.display();
	bool play = true;
	string action;
	Player white, black;
	white.king = {4, 0}, black.king = {4, 7};
	char turn = ' ';

	//While the play bool var is true, go through the loop
	while (play){
		//If it's the white player's turn and...
		//1. They can capture the black's king, white wins
		//2. The white king is in checkmate and is stuck, black wins
		//3. The white king is alone and all it's possible moves are in checkmate, black wins
		if (turn == ' '){
			if (game.checkmate(black.king, '*')){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else if (game.checkmate(white.king, ' ') && game.king_is_stuck(white.king)){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else if (game.king_is_alone(' ') && game.checkmate_all(white.king, ' ')){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else
				cout << "White's turn: " << endl;
		}
		//Else if it's the black player's turn and...
		//1. The white king can be captured, black wins
		//2. The black king is stuck and in checkmate, white wins
		//3. All the black king's possible moves are in checkmate, white wins
		else{
			if (game.checkmate(white.king, ' ')){
				cout << "Checkmate. Black wins!" << endl;
				break;
			}
			else if (game.checkmate(black.king, '*') && game.king_is_stuck(black.king)){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else if (game.king_is_alone('*') && game.checkmate_all(black.king, '*')){
				cout << "Checkmate. White wins!" << endl;
				break;
			}
			else
				cout << "Black's turn: " << endl;
		}

		//Prompt the user to enter a move and accept it using the getline function
		//Remember to remove the space character added onto the string by the getline function
		cout << "Enter a move: ";
		getline(cin, action);

		/**********************************************************************************************/

		//Some compilers don't add the space at the end. If that's the case, just comment out this line.
		action.erase(action.length()-1);

		/**********************************************************************************************/

		//Validate the move's format
		if (validateFormat(action)){
			//If the move is correct, find the 1st pos by creating a substr from the start of the move to the first space
			int first_space = action.find(" ");
			string coord1 = action.substr(0, first_space);
			//Convert the chars into integers using ASCII int values
			Pos pos1 = {(coord1[0] - 'a'), (coord1[1] - '1')};

			//Find the 2nd pos by creating a substr from the 1 character after the first space to the end of the string
			int next_coord = action.find(" ", first_space+1) + 1;
			string coord2 = action.substr(next_coord, (action.length() - next_coord));
			Pos pos2 = {(coord2[0] - 'a'), (coord2[1] - '1')};

			//Get the string piece using the 1st pos
			//and use it to find the piece's player id (ie. ' ', '*')  and the piece id (ie. 'P', 'B')
			string piece = game.getPiece(pos1.x, pos1.y);
			char player_ID = piece[0], piece_ID = piece[1];

			//Find the corresponding piece with the same player_ID as the turn and try to move it
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

			//Change the turn once a player has moved their piece
			if (game.getPiece(pos1.x, pos1.y) == "  " && piece != "  ")
				turn = (turn == ' ') ? '*' : ' ';

			game.display();
		}
		//If the player enters 'quit', the program terminates
		else if (action.compare("quit") == 0){
			play = false;
			cout << "quitting..." << endl;
		}
		//If the move format is invalid, warn the user.
		else{
			cout << "Invalid format. Please re-enter a valid move." << endl;
		}
	}
}
