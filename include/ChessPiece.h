#pragma once
#include "ChessBoard.h"
#include "enum.h"
#include <vector>

class ChessPiece;

struct ChessPieceMove {
	ChessPiece* chesspiece1;
	ChessPiece* chesspiece2;
	ChessPiece* chesspiece3;
	int oldX, oldY;
	int newX, newY;
	bool isPromoted;
	ChessPieceMoveType chesspieceMoveType;

	ChessPieceMove() {
		chesspiece1 = NULL;
		chesspiece2 = NULL;
		chesspiece3 = NULL;
		this->oldX = -1;
		this->oldY = -1;
		this->newX = -1;
		this->newY = -1;
		isPromoted = false;
		chesspieceMoveType = Empty;
	}

	ChessPieceMove(ChessPiece* cp1, ChessPiece* cp2, int newX, int newY, int oldX, int oldY, ChessPieceMoveType cpmt) {
		chesspiece1 = cp1;
		chesspiece2 = cp2;
		chesspiece3 = NULL;
		this->oldX = oldX;
		this->oldY = oldY;
		this->newX = newX;
		this->newY = newY;
		isPromoted = false;
		chesspieceMoveType = cpmt;
	}

	~ChessPieceMove() {
		chesspiece1 = NULL;
		chesspiece2 = NULL;
		chesspiece3 = NULL;
	}
};


class ChessBoard;

class ChessPiece {
protected:
	bool isWhite;
	ChessPieceType type;
	std::vector<int> directionx;
	std::vector<int> directiony;
	int range;

public:
	bool isDead = false;
	int id = 0;
	int x = -1, y = -1, numOfMoves = 0;
	ChessBoard* chessboard;
	ChessPiece(int x, int y, bool isWhite);
	~ChessPiece();
	void init(std::vector<int>& directionx, std::vector<int>& directiony, int range);
	void generatePossibleMoves();
	void setType(ChessPieceType type);
	ChessPieceType getType();
	bool getIsWhite();
	void setChessBoard(ChessBoard* chessboard);
};

void deleteChessPiece(ChessPiece* cp);

class Pawn : public ChessPiece {
private:
	std::vector<int> directionx{}, attackDirectionX{ -1,1 };
	std::vector<int> directiony{}, attackDirectionY{ 1,1 };
	int range = 1;

public:
	bool canPlay2Step = true;
	bool prevMoveWas2Step = false;
	Pawn(int x, int y, bool isWhite);
	~Pawn();
	void generatePossibleMoves();
	bool getPrevMoveWas2Step();
	void setPrevMoveWas2Step(bool val);
	void setCanPlay2Step(bool val);
	void pawnPromotion(ChessPiece* cp1, ChessPiece* cp2, int newX, int newY, ChessPieceMoveType cpmt);
};

class Rook : public ChessPiece {
private:
	std::vector<int> directionx{ 0,0,-1, 1 };
	std::vector<int> directiony{ -1, 1, 0, 0 };
	int range = 8;
public:
	bool hasMoved = false;
	Rook(int x, int y, bool isWhite);
	~Rook();
	//void generatePossibleMoves();
};

class Knight : public ChessPiece {
private:
	std::vector<int> directionx{ 1,1,-1,-1,2,2,-2,-2 };
	std::vector<int> directiony{ 2, -2, 2, -2, 1, -1, 1, -1};
	int range = 1;

public:
	Knight(int x, int y, bool isWhite);
	~Knight();
	//void generatePossibleMoves();
};

class Bishop : public ChessPiece {
private:
	std::vector<int> directionx{ 1,1,-1,-1 };
	std::vector<int> directiony{ 1, -1, -1, 1 };
	int range = 8;

public:
	Bishop(int x, int y, bool isWhite);
	~Bishop();
	//void generatePossibleMoves();
};

class Queen : public ChessPiece {
private:
	std::vector<int> directionx{ 1,1,-1,-1, 0, 0, -1, 1 };
	std::vector<int> directiony{ 1, -1, -1, 1, 1, -1, 0, 0 };
	int range = 8;

public:
	Queen(int x, int y, bool isWhite);
	~Queen();
	//void generatePossibleMoves();
};

class King : public ChessPiece {
private:
	std::vector<int> directionx{ 1,1,-1,-1, 0, 0, -1, 1 };
	std::vector<int> directiony{ 1, -1, -1, 1, 1, -1, 0, 0  };
	int range = 1;
public:
	bool hasMoved = false;
	King(int x, int y, bool isWhite);
	~King();
	void generatePossibleMoves();
	bool isInDanger();
};

