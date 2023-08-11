#include <iostream>
#include <assert.h>
#include "enum.h"
#include <vector>
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class ChessPiece;
struct ChessPieceMove;

class ChessBoard {
public:
	int id = 0;
	bool isWhitesTurn = true;
	int halfMoveCounts = 0;
	int fullMoveCounts = 0;
	int enPassantX = -1, enPassantY = -1;
	std::vector<std::vector<ChessPiece*>> chessboard;
	std::vector<ChessPiece*> removedPawns;
	std::map<int, ChessPiece*> allChessPieces;
	std::map<ChessPieceType, std::vector<ChessPiece*>> chesspieceMap;
	std::vector<ChessPieceMove> prevMoves, nextPossibleMoves;

	ChessBoard();
	~ChessBoard();
	ChessBoard(std::string& fenn);
	void init(std::vector<std::vector<ChessPieceType>> chessTypeBoard);
	ChessPieceMove isValidMove(int x, int y, char promotedTo);
	void validatePossibleMoves();
	bool isKingVulnerable();
	bool isKingVulnerable(bool isWhitesTurn);
	bool isSquareInDanger(int x, int y, bool isWhite);
	void setIsWhitesTurn(std::string& s);
	void setupCastling(std::string& s);
	void setupEnPassant(std::string& s);
	void genNextPossibleMoves();
	void move(int x, int y, char promotedTo);
	void postMoveProcess(ChessPieceMove move);
	void undo();
	void postUndoProcess(ChessPieceMove move);
	void cleanUp(ChessPieceMove move);
	int getNextPossibleMovesCount();
	bool isInsideChessBoard(int x, int y);
	std::string toFen();
	bool hasPiece(ChessPieceType cpt);
};


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


static int chessboardId = 0;

std::vector<std::string> split(std::string& s, char c) {
	std::vector<std::string> v;
	int i = 0, n = (int)s.size();
	std::string temp = "";
	for (i = 0; i < n; i++) {
		if (s[i] == ' ') {
			v.push_back(temp);
			temp = "";
		}
		else {
			temp.push_back(s[i]);
		}
	}
	v.push_back(temp);
	return v;
}


ChessBoard::ChessBoard() {
	id = chessboardId++;
	//std::cout << "Chessboard " << id << std::endl;
	chessboard.resize(8, std::vector<ChessPiece*>(8, NULL));
	std::vector<std::vector<ChessPieceType>> chessTypeBoard = {
		{BlackRookType,BlackKnightType,BlackBishopType,BlackQueenType,BlackKingType,BlackBishopType,BlackKnightType,BlackRookType},
		{BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType},
		//{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		//{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		//{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		//{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		{EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType,EmptyType},
		{WhitePawnType,WhitePawnType,WhitePawnType,WhitePawnType,WhitePawnType,WhitePawnType,WhitePawnType,WhitePawnType},
		{WhiteRookType,WhiteKnightType,WhiteBishopType,WhiteQueenType,WhiteKingType,WhiteBishopType,WhiteKnightType,WhiteRookType},
	};

	init(chessTypeBoard);
	isWhitesTurn = true;
}

ChessBoard::ChessBoard(std::string& fen) {
	id = chessboardId++;
	//std::cout << "Chessboard " << id << std::endl;
	chessboard.resize(8, std::vector<ChessPiece*>(8, NULL));
	std::vector<std::vector<ChessPieceType>> chessTypeBoard(8, std::vector<ChessPieceType>(8, EmptyType));
	std::vector<std::string> fen_arr = split(fen, ' ');
	int i = 0, j = 0;
	for (auto c : fen_arr[0]) {
		if (c == '/') {
			i++;
			j = 0;
		}
		else {
			if (c - '0' >= 0 && c - '0' <= 9)
				j += (c - '0');
			else
				chessTypeBoard[i][j++] = static_cast<ChessPieceType>(c);
		}
	}

	init(chessTypeBoard);
	setIsWhitesTurn(fen_arr[1]);
	setupCastling(fen_arr[2]);
	setupEnPassant(fen_arr[3]);
	halfMoveCounts = std::stoi(fen_arr[4]);
	fullMoveCounts = std::stoi(fen_arr[5]);
}

void deleteChessPiece(ChessPiece *cp) {
	Pawn* pawn;
	Rook* rook;
	Knight* knight;
	Bishop* bishop;
	Queen* queen;
	King* king;
	ChessPieceType cpt = cp->getType();
	switch (cpt)
	{
	case WhitePawnType:
	case BlackPawnType:
		pawn = static_cast<Pawn*>(cp);
		//std::cout << "Pawn" << std::endl;
		delete pawn;
		break;
	case WhiteRookType:
	case BlackRookType:
		rook = static_cast<Rook*>(cp);
		//std::cout << "Rook" << std::endl;
		delete rook;
		break;
	case WhiteBishopType:
	case BlackBishopType:
		bishop = static_cast<Bishop*>(cp);
		//std::cout << "Bishop" << std::endl;
		delete bishop;
		break;
	case WhiteKnightType:
	case BlackKnightType:
		knight = static_cast<Knight*>(cp);
		//std::cout << "Knight" << std::endl;
		delete knight;
		break;
	case WhiteQueenType:
	case BlackQueenType:
		queen = static_cast<Queen*>(cp);
		//std::cout << "Queen" << std::endl;
		delete queen;
		break;
	case WhiteKingType:
	case BlackKingType:
		king = static_cast<King*>(cp);
		//std::cout << "King" << std::endl;
		delete king;
		break;
	default:
		delete cp;
	}
}

ChessBoard::~ChessBoard()
{	
	for (auto& it : allChessPieces) {
		deleteChessPiece(it.second);
	}
	for (auto& it : nextPossibleMoves) {
		/*if (it.chesspiece3) {
			std::cout << (char)it.chesspiece3->getType() << std::endl;
		}*/
		cleanUp(it);
	}
	/*for (auto& it : chesspieceMap) {
		for (auto& it2 : it.second) {
			delete it2;
		}
	}

	for (auto &it : removedPawns) {
		delete it;
	}*/
}




void ChessBoard::init(std::vector<std::vector<ChessPieceType>> chessTypeBoard) {
	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			switch (chessTypeBoard[i][j])
			{
			case WhitePawnType:
				chessboard[i][j] = new Pawn(j, i, true);
				break;
			case WhiteRookType:
				chessboard[i][j] = new Rook(j, i, true);
				break;
			case WhiteKnightType:
				chessboard[i][j] = new Knight(j, i, true);
				break;
			case WhiteBishopType:
				chessboard[i][j] = new Bishop(j, i, true);
				break;
			case WhiteQueenType:
				chessboard[i][j] = new Queen(j, i, true);
				break;
			case WhiteKingType:
				chessboard[i][j] = new King(j, i, true);
				break;

			case BlackPawnType:
				chessboard[i][j] = new Pawn(j, i, false);
				break;
			case BlackRookType:
				chessboard[i][j] = new Rook(j, i, false);
				break;
			case BlackKnightType:
				chessboard[i][j] = new Knight(j, i, false);
				break;
			case BlackBishopType:
				chessboard[i][j] = new Bishop(j, i, false);
				break;
			case BlackQueenType:
				chessboard[i][j] = new Queen(j, i, false);
				break;
			case BlackKingType:
				chessboard[i][j] = new King(j, i, false);
				break;
			default:
				chessboard[i][j] = NULL;
			}

			if (chessboard[i][j] != NULL) {
				chessboard[i][j]->setChessBoard(this);
				allChessPieces[chessboard[i][j]->id] = chessboard[i][j];
				chesspieceMap[chessboard[i][j]->getType()].push_back(chessboard[i][j]);
			}

		}
	}
}


ChessPieceMove ChessBoard::isValidMove(int x, int y, char promotedTo)
{
	if (!isInsideChessBoard(x, y))
		return ChessPieceMove();
	for (auto& it : nextPossibleMoves)
		if (it.newX == x && it.newY == y) {
			if (promotedTo == ' ' || (it.chesspiece3 != NULL && promotedTo == (char)it.chesspiece3->getType()))
				return it;
		}
	return ChessPieceMove();
}

void ChessBoard::cleanUp(ChessPieceMove move) {
	ChessPiece* temp = move.chesspiece3;
	if (temp)
		deleteChessPiece(temp);
}

void ChessBoard::move(int x, int y, char promotedTo = ' ')
{
	ChessPieceMove chesspieceMove = isValidMove(x, y, promotedTo);
	if (chesspieceMove.chesspiece1 != NULL) {
		prevMoves.push_back(chesspieceMove);
		int oldX = chesspieceMove.oldX;
		int oldY = chesspieceMove.oldY;
		int newX = chesspieceMove.newX;
		int newY = chesspieceMove.newY;
		int rookX, rookY;

		if (chesspieceMove.chesspiece2) {
			int x2 = chesspieceMove.chesspiece2->x;
			int y2 = chesspieceMove.chesspiece2->y;
			chessboard[y2][x2] = NULL;
		}

		chesspieceMove.chesspiece1->x = newX;
		chesspieceMove.chesspiece1->y = newY;
		chesspieceMove.chesspiece1->numOfMoves++;
		chessboard[oldY][oldX] = NULL;
		chessboard[newY][newX] = chesspieceMove.chesspiece1;
		if (chesspieceMove.chesspiece2 != NULL)
		{
			switch (chesspieceMove.chesspieceMoveType)
			{
			case Kill:
			case EnPassant:
				chesspieceMove.chesspiece2->isDead = true;
				chesspieceMove.chesspiece2->x = -1;
				chesspieceMove.chesspiece2->y = -1;

				break;
			case Castle:
				rookX = newX + (newX > oldX ? -1 : 1);
				rookY = newY;
				chesspieceMove.chesspiece2->x = rookX;
				chesspieceMove.chesspiece2->y = rookY;
				chessboard[rookY][rookX] = chesspieceMove.chesspiece2;
				chesspieceMove.chesspiece2->numOfMoves++;
				break;
			default:
				break;
			}
		}

		if (chesspieceMove.chesspiece1->getType() == (chesspieceMove.chesspiece1->getIsWhite() ? WhitePawnType : BlackPawnType) && chesspieceMove.isPromoted) {
			ChessPieceType cpt = chesspieceMove.chesspiece1->getIsWhite() ? WhitePawnType : BlackPawnType;
			chesspieceMove.chesspiece1->isDead = true;
			chesspieceMove.chesspiece1->x = -1;
			chesspieceMove.chesspiece1->y = -1;

			std::vector<ChessPiece*> cpv = chesspieceMap[cpt];
			removedPawns.push_back(chesspieceMove.chesspiece1);
			cpv.erase(std::remove_if(cpv.begin(), cpv.end(), [&](ChessPiece* cp) {
				return  cp->id == chesspieceMove.chesspiece1->id;
				}), cpv.end());
			chesspieceMap[cpt] = cpv;

			ChessPieceType cpt1 = chesspieceMove.chesspiece3->getType();
			chesspieceMove.chesspiece3->x = newX;
			chesspieceMove.chesspiece3->y = newY;
			chessboard[newY][newX] = chesspieceMove.chesspiece3;
			chesspieceMove.chesspiece3->numOfMoves = 1;
			chesspieceMap[cpt1].push_back(chesspieceMove.chesspiece3);
		}

		postMoveProcess(chesspieceMove);
		isWhitesTurn ^= true;
	}
}


void ChessBoard::postMoveProcess(ChessPieceMove move) {
	for (ChessPiece* it : chesspieceMap[WhitePawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);
	}
	for (ChessPiece* it : chesspieceMap[BlackPawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);
	}
	int numOfMoves = move.chesspiece1->numOfMoves;
	switch (move.chesspiece1->getType())
	{
	case WhitePawnType:
	case BlackPawnType:
		if (move.chesspieceMoveType == Pawn2Step)
			static_cast<Pawn*>(move.chesspiece1)->setPrevMoveWas2Step(true);
		static_cast<Pawn*>(move.chesspiece1)->setCanPlay2Step(numOfMoves == 0);
		break;
	case WhiteKingType:
	case BlackKingType:
		static_cast<King*>(move.chesspiece1)->hasMoved = numOfMoves > 0;
		break;
	case WhiteRookType:
	case BlackRookType:
		static_cast<Rook*>(move.chesspiece1)->hasMoved = numOfMoves > 0;
		break;
	default:
		break;
	}
	if (move.chesspiece2 != NULL) {
		int numOfMoves = move.chesspiece2->numOfMoves;
		switch (move.chesspiece2->getType())
		{
		case WhiteKingType:
		case BlackKingType:
			static_cast<King*>(move.chesspiece2)->hasMoved = numOfMoves > 0;
			break;
		case WhiteRookType:
		case BlackRookType:
			static_cast<Rook*>(move.chesspiece2)->hasMoved = numOfMoves > 0;
			break;
		default:
			break;
		}
	}

}

void ChessBoard::postUndoProcess(ChessPieceMove move) {
	int numOfMoves = move.chesspiece1->numOfMoves;
	switch (move.chesspiece1->getType())
	{
	case WhitePawnType:
	case BlackPawnType:
		static_cast<Pawn*>(move.chesspiece1)->setCanPlay2Step(numOfMoves == 0);
		break;
	case WhiteKingType:
	case BlackKingType:
		static_cast<King*>(move.chesspiece1)->hasMoved = numOfMoves > 0;
		break;
	case WhiteRookType:
	case BlackRookType:
		static_cast<Rook*>(move.chesspiece1)->hasMoved = numOfMoves > 0;
		break;
	default:
		break;
	}
	if (move.chesspiece2 != NULL) {
		int numOfMoves = move.chesspiece2->numOfMoves;
		switch (move.chesspiece2->getType())
		{
		case WhiteKingType:
		case BlackKingType:
			static_cast<King*>(move.chesspiece2)->hasMoved = numOfMoves > 0;
			break;
		case WhiteRookType:
		case BlackRookType:
			static_cast<Rook*>(move.chesspiece2)->hasMoved = numOfMoves > 0;
			break;
		default:
			break;
		}
	}


	for (ChessPiece* it : chesspieceMap[WhitePawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);

	}
	for (ChessPiece* it : chesspieceMap[BlackPawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);
	}

	if (prevMoves.size() > 0) {
		move = prevMoves.back();
		switch (move.chesspiece1->getType())
		{
		case WhitePawnType:
		case BlackPawnType:
			if (move.chesspieceMoveType == Pawn2Step)
				static_cast<Pawn*>(move.chesspiece1)->setPrevMoveWas2Step(true);
		}
	}
	else {
		if (enPassantX != -1 && enPassantY != -1) {
			static_cast<Pawn*>(chessboard[enPassantY][enPassantX])->setPrevMoveWas2Step(true);
		}
	}

}

void ChessBoard::undo() {
	if (prevMoves.size() > 0) {
		ChessPieceMove prevMove = prevMoves.back();
		prevMoves.pop_back();
		int oldX = prevMove.oldX;
		int oldY = prevMove.oldY;
		int newX = prevMove.newX;
		int newY = prevMove.newY;
		int rookX, rookY;
		int enX, enY;

		if (prevMove.chesspiece1->getType() == (prevMove.chesspiece1->getIsWhite() ? WhitePawnType : BlackPawnType) && prevMove.isPromoted) {
			ChessPieceType cpt = prevMove.chesspiece1->getIsWhite() ? WhitePawnType : BlackPawnType;
			ChessPieceType cpt1 = prevMove.chesspiece3->getType();

			prevMove.chesspiece1->isDead = false;
			prevMove.chesspiece1->x = newX;
			prevMove.chesspiece1->y = newY;

			chesspieceMap[cpt].push_back(prevMove.chesspiece1);
			chessboard[newY][newX] = prevMove.chesspiece1;

			std::vector<ChessPiece*> cpv = chesspieceMap[cpt1];
			cpv.erase(std::remove_if(cpv.begin(), cpv.end(), [&](ChessPiece* cp) {
				return  cp->id == prevMove.chesspiece3->id;
				}), cpv.end());

			removedPawns.erase(std::remove_if(removedPawns.begin(), removedPawns.end(), [&](ChessPiece* cp) {
				return  cp->id == prevMove.chesspiece1->id;
				}), removedPawns.end());

			chesspieceMap[cpt1] = cpv;
		}


		prevMove.chesspiece1->x = oldX;
		prevMove.chesspiece1->y = oldY;
		chessboard[newY][newX] = NULL;
		chessboard[oldY][oldX] = prevMove.chesspiece1;
		prevMove.chesspiece1->numOfMoves--;
		switch (prevMove.chesspieceMoveType)
		{
		case Move:
			break;
		case EnPassant:
			prevMove.chesspiece2->isDead = false;
			enX = newX;
			enY = newY + (prevMove.chesspiece2->getIsWhite() ? -1 : 1);
			prevMove.chesspiece2->x = enX;
			prevMove.chesspiece2->y = enY;
			chessboard[enY][enX] = prevMove.chesspiece2;
			break;
		case Kill:
			prevMove.chesspiece2->isDead = false;
			prevMove.chesspiece2->x = newX;
			prevMove.chesspiece2->y = newY;
			chessboard[newY][newX] = prevMove.chesspiece2;
			break;
		case Castle:
			rookX = newX > oldX ? 7 : 0;
			rookY = newY;
			prevMove.chesspiece2->x = rookX;
			prevMove.chesspiece2->y = rookY;
			chessboard[rookY][rookX == 7 ? 5 : 3] = NULL;
			chessboard[rookY][rookX] = prevMove.chesspiece2;
			prevMove.chesspiece2->numOfMoves--;
			break;
		default:
			break;
		}

		postUndoProcess(prevMove);
		isWhitesTurn ^= true;
	}
}

void ChessBoard::validatePossibleMoves() {
	std::vector<ChessPieceMove> validPossibleMoves{};

	std::vector<std::vector<char>> v(8, std::vector<char>(8, '_'));
	std::vector<std::vector<char>> v1(8, std::vector<char>(8, '_'));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard[i][j])
				v[i][j] = (char)chessboard[i][j]->getType();
		}
	}
	std::string fen = toFen();

	for (auto& nextMove : nextPossibleMoves) {
		char promotedTo = nextMove.chesspiece3 == NULL ? ' ' : (char)nextMove.chesspiece3->getType();
		move(nextMove.newX, nextMove.newY, promotedTo);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (chessboard[i][j])
					v1[i][j] = (char)chessboard[i][j]->getType();
				else
					v1[i][j] = '_';
			}
		}
		undo();

		bool flag = isKingVulnerable(isWhitesTurn ^ true);
		if (!flag)
			validPossibleMoves.push_back(nextMove);
		else
			cleanUp(nextMove);

		std::string fenCheck = toFen();
		if (fen != fenCheck) {
			std::cout << "Move => " << std::endl;
			std::cout << (char)nextMove.chesspiece1->getType() << " ";
			std::cout << (char)nextMove.chesspieceMoveType << " ";
			std::cout << nextMove.oldX << " " << nextMove.oldY << " ";
			std::cout << nextMove.newX << " " << nextMove.newY << " ";
			std::cout << std::endl;
			std::cout << "Old => " << std::endl;
			std::cout << fen << std::endl;
			for (auto it : v) {
				for (auto it2 : it) {
					std::cout << it2;
				}
				std::cout << std::endl;
			}

			std::cout << "Move State => " << std::endl;
			for (auto it : v1) {
				for (auto it2 : it) {
					std::cout << it2;
				}
				std::cout << std::endl;
			}
			std::cout << "New => " << std::endl;
			std::cout << fenCheck << std::endl;
			for (auto it : chessboard) {
				for (auto it2 : it) {
					if (it2 != NULL)
						std::cout << (char)(it2->getType());
					else
						std::cout << "_";
				}
				std::cout << std::endl;
			}

		}
		assert(fen == fenCheck);
	}
	nextPossibleMoves = validPossibleMoves;
}

bool ChessBoard::isKingVulnerable(bool isWhitesTurn) {
	ChessPieceType king = isWhitesTurn ? WhiteKingType : BlackKingType;
	return static_cast<King*>(chesspieceMap[king][0])->isInDanger();
}

bool ChessBoard::isKingVulnerable()
{
	return isKingVulnerable(this->isWhitesTurn);
}

void ChessBoard::genNextPossibleMoves()
{
	for (auto move : nextPossibleMoves) {
		cleanUp(move);
	}
	nextPossibleMoves.clear();
	int count = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard[i][j] != NULL && chessboard[i][j]->getIsWhite() == isWhitesTurn) {
				Pawn* pawn;
				King* king;
				switch (chessboard[i][j]->getType()) {
				case WhitePawnType:
				case BlackPawnType:
					pawn = static_cast<Pawn*>(chessboard[i][j]);
					pawn->generatePossibleMoves();
					break;
				case WhiteKingType:
				case BlackKingType:
					king = static_cast<King*>(chessboard[i][j]);
					king->generatePossibleMoves();
					break;
				default:
					chessboard[i][j]->generatePossibleMoves();
					break;
				}
			}
		}
	}
	validatePossibleMoves();
	//count += (int)chessboard[i][j]->nextPossibleMoves.size();
	//for (auto& nextMove : chessboard[i][j]->nextPossibleMoves) {
	//	nextPossibleMoves.push_back(nextMove);
	//}
}

int ChessBoard::getNextPossibleMovesCount()
{
	return (int)nextPossibleMoves.size();
}

std::string ChessBoard::toFen()
{
	std::string fen = "";
	int i, j;
	// Board Rep (1)
	for (i = 0; i < 8; i++) {
		int numOfEmptyCells = 0;
		for (j = 0; j < 8; j++) {
			if (chessboard[i][j] == NULL)
				numOfEmptyCells++;
			else {
				if (numOfEmptyCells)
					fen += (numOfEmptyCells + '0');
				numOfEmptyCells = 0;
				fen += (char)chessboard[i][j]->getType();
			}
		}
		if (numOfEmptyCells)
			fen += std::to_string(numOfEmptyCells);
		if (i != 7)
			fen += '/';
	}

	fen += " ";
	// next turn (2)
	fen += (isWhitesTurn ? 'w' : 'b');

	fen += " ";
	// castling (3)
	bool hasWhiteKingMoved = static_cast<King*>(chesspieceMap[WhiteKingType][0])->hasMoved;
	bool hasBlackKingMoved = static_cast<King*>(chesspieceMap[BlackKingType][0])->hasMoved;
	if (hasWhiteKingMoved == false) {
		if (chessboard[7][7] != NULL && chessboard[7][7]->getType() == WhiteRookType &&
			static_cast<Rook*>(chessboard[7][7])->hasMoved == false)
			fen += "K";

		if (chessboard[7][0] != NULL && chessboard[7][0]->getType() == WhiteRookType &&
			static_cast<Rook*>(chessboard[7][0])->hasMoved == false)
			fen += "Q";

	}
	if (hasBlackKingMoved == false) {
		if (chessboard[0][7] != NULL && chessboard[0][7]->getType() == BlackRookType &&
			static_cast<Rook*>(chessboard[0][7])->hasMoved == false)
			fen += "k";

		if (chessboard[0][0] != NULL && chessboard[0][0]->getType() == BlackRookType &&
			static_cast<Rook*>(chessboard[0][0])->hasMoved == false)
			fen += "q";
	}

	if (fen.back() == ' ')
		fen += '-';

	fen += " ";
	// En Passant (4)
	for (auto it : chesspieceMap[isWhitesTurn ? BlackPawnType : WhitePawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		if (pawn->prevMoveWas2Step) {
			int x = pawn->x;
			int y = pawn->y + (pawn->getIsWhite() ? 1 : -1);
			char cx = 'a' + x;
			char cy = '0' + (8 - y);
			fen += cx;
			fen += cy;
			break;
		}
	}
	if (fen.back() == ' ')
		fen += "-";

	fen += " ";
	// Half Move (5)
	fen += std::to_string(halfMoveCounts);

	fen += " ";
	// Full Move (6)
	fen += std::to_string(fullMoveCounts);

	return fen;
}

bool ChessBoard::isInsideChessBoard(int x, int y)
{
	return (0 <= x && x <= 7 && 0 <= y && y <= 7);
}

bool ChessBoard::hasPiece(ChessPieceType cpt)
{
	for (auto it : chesspieceMap[cpt]) {
		if (!it->isDead)
			return true;
	}
	return false;
}

void ChessBoard::setIsWhitesTurn(std::string& s) {
	if (s[0] == 'b')
		isWhitesTurn = false;
	else
		isWhitesTurn = true;
}

void ChessBoard::setupCastling(std::string& s) {
	for (auto& it : chesspieceMap[WhiteKingType]) {
		static_cast<King*>(it)->hasMoved = true;
		it->numOfMoves = 1;
	}
	for (auto& it : chesspieceMap[BlackKingType]) {
		static_cast<King*>(it)->hasMoved = true;
		it->numOfMoves = 1;
	}
	for (auto& it : chesspieceMap[WhiteRookType]) {
		static_cast<Rook*>(it)->hasMoved = true;
		it->numOfMoves = 1;
	}
	for (auto& it : chesspieceMap[BlackRookType]) {
		static_cast<Rook*>(it)->hasMoved = true;
		it->numOfMoves = 1;
	}
	for (auto c : s) {
		switch (c)
		{
		case (char)WhiteQueenType:
			static_cast<Rook*>(chessboard[7][0])->hasMoved = false;
			static_cast<King*>(chessboard[7][4])->hasMoved = false;
			chessboard[7][0]->numOfMoves = 0;
			chessboard[7][4]->numOfMoves = 0;
			break;
		case (char)BlackQueenType:
			static_cast<Rook*>(chessboard[0][0])->hasMoved = false;
			static_cast<King*>(chessboard[0][4])->hasMoved = false;
			chessboard[0][0]->numOfMoves = 0;
			chessboard[0][4]->numOfMoves = 0;
			break;
		case (char)WhiteKingType:
			static_cast<Rook*>(chessboard[7][7])->hasMoved = false;
			static_cast<King*>(chessboard[7][4])->hasMoved = false;
			chessboard[7][7]->numOfMoves = 0;
			chessboard[7][4]->numOfMoves = 0;
			break;
		case (char)BlackKingType:
			static_cast<Rook*>(chessboard[0][7])->hasMoved = false;
			static_cast<King*>(chessboard[0][4])->hasMoved = false;
			chessboard[0][7]->numOfMoves = 0;
			chessboard[0][4]->numOfMoves = 0;
			break;
		default:
			break;
		}
	}
}

void ChessBoard::setupEnPassant(std::string& s)
{
	if (s.size() == 2) {
		int x = s[0] - 'a';
		int y = 8 - (s[1] - '0') + (isWhitesTurn ? 1 : -1);
		enPassantX = x;
		enPassantY = y;
		static_cast<Pawn*>(chessboard[y][x])->prevMoveWas2Step = true;
	}
}

bool ChessBoard::isSquareInDanger(int x, int y, bool isWhite) {
	// King
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			int newX = i + x;
			int newY = i + y;
			if (!isInsideChessBoard(newX, newY) || chessboard[newY][newX] == NULL)
				continue;
			if (chessboard[newY][newX]->getType() == (isWhite ? BlackKingType : WhiteKingType))
				return true;
		}
	}

	// Pawn
	int pawnX1 = x + 1;
	int pawnX2 = x - 1;
	int pawnY = y + (isWhite ? -1 : 1);
	for (int k = -1; k <= 1; k += 2) {
		int pawnX = x + k;
		int pawnY = y + (isWhite ? -1 : 1);
		if (!isInsideChessBoard(pawnX, pawnY) || chessboard[pawnY][pawnX] == NULL)
			continue;
		if (chessboard[pawnY][pawnX]->getType() == (isWhite ? BlackPawnType : WhitePawnType)) {
			return true;
		}

	}

	// Rook & Queen's Rook Movement
	for (int k = -1; k <= 1; k += 2) {
		// Horizontal
		for (int i = x + k; i < 8 && i >= 0; i += k) {
			if (chessboard[y][i] == NULL)
				continue;
			if (chessboard[y][i]->getType() == (isWhite ? BlackRookType : WhiteRookType) ||
				chessboard[y][i]->getType() == (isWhite ? BlackQueenType : WhiteQueenType)) {
				return true;
			}
			break;
		}
		// Vertical
		for (int i = y + k; i < 8 && i >= 0; i += k) {
			if (chessboard[i][x] == NULL)
				continue;
			if (chessboard[i][x]->getType() == (isWhite ? BlackRookType : WhiteRookType) ||
				chessboard[i][x]->getType() == (isWhite ? BlackQueenType : WhiteQueenType)) {
				return true;
			}
			break;
		}
	}

	// Bishop & Queen's Bishop Movement
	for (int k = -1; k <= 1; k += 2) {
		// Diagonal
		for (int i = y + k, j = x + k; i < 8 && i >= 0 && j < 8 && j >= 0; i += k, j += k) {
			if (chessboard[i][j] == NULL)
				continue;
			if (chessboard[i][j]->getType() == (isWhite ? BlackBishopType : WhiteBishopType) ||
				chessboard[i][j]->getType() == (isWhite ? BlackQueenType : WhiteQueenType)) {
				return true;
			}
			break;
		}
		// Inverse Diagonal
		for (int i = y - k, j = x + k; i < 8 && i >= 0 && j < 8 && j >= 0; i -= k, j += k) {
			if (chessboard[i][j] == NULL)
				continue;
			if (chessboard[i][j]->getType() == (isWhite ? BlackBishopType : WhiteBishopType) ||
				chessboard[i][j]->getType() == (isWhite ? BlackQueenType : WhiteQueenType)) {
				return true;
			}
			break;
		}
	}

	// Knight
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -2; j <= 2; j += 4) {
			if ((isInsideChessBoard(x + i, y + j) && chessboard[y + j][x + i] != NULL &&
				chessboard[y + j][x + i]->getType() == (isWhite ? BlackKnightType : WhiteKnightType)) ||
				(isInsideChessBoard(x + j, y + i) && chessboard[y + i][x + j] != NULL &&
					chessboard[y + i][x + j]->getType() == (isWhite ? BlackKnightType : WhiteKnightType))) {
				return true;
			}
		}
	}

	return false;
}



ChessPiece::ChessPiece(int x, int y, bool isWhite)
	:x(x), y(y), isWhite(isWhite)
{
	type = EmptyType;
	static int _id = 0;
	id = _id++;
	numOfMoves = 0;
}

void ChessPiece::init(std::vector<int>& directionx, std::vector<int>& directiony, int range) {
	this->directionx = directionx;
	this->directiony = directiony;
	this->range = range;
	//std::cout << this->range << std::endl;
}

void ChessPiece::setType(ChessPieceType type)
{
	this->type = type;
}


void ChessPiece::generatePossibleMoves() {
	int i, j, n = (int)directionx.size();

	for (i = 0; i < n; i++) {
		int newX = x;
		int newY = y;
		for (j = 0; j < range; j++) {
			newX += directionx[i] * (isWhite ? -1 : 1);
			newY += directiony[i] * (isWhite ? -1 : 1);
			if (!chessboard->isInsideChessBoard(newX, newY))
				break;
			if (chessboard->chessboard[newY][newX] == NULL ||
				chessboard->chessboard[newY][newX]->getIsWhite() != isWhite) {
				ChessPiece* cp1 = this;
				ChessPiece* cp2 = chessboard->chessboard[newY][newX];
				ChessPieceMoveType cpmt = (cp2 == NULL ? Move : Kill);
				ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
				chessboard->nextPossibleMoves.push_back(chesspieceMove);
				if (cpmt == Kill)
					break;
			}
			else
				break;
		}
	}

}


ChessPiece::~ChessPiece()
{
	std::cout << "ChessPiece " << (char)type << " " << id << std::endl;
	chessboard = NULL;
	directionx.clear();
	directiony.clear();
}


ChessPieceType ChessPiece::getType() {
	return type;
}

bool ChessPiece::getIsWhite()
{
	return isWhite;
}

void ChessPiece::setChessBoard(ChessBoard* chessboard) {
	this->chessboard = chessboard;
}


Pawn::Pawn(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhitePawnType);
	else
		setType(BlackPawnType);
	if (y == (isWhite ? 6 : 1))
		canPlay2Step = true;
	else
		canPlay2Step = false;
	numOfMoves = y == (isWhite ? 6 : 1) ? 0 : 1;
}

Pawn::~Pawn()
{
	std::cout << "Pawn Destructor" << std::endl;
	attackDirectionX.clear();
	attackDirectionY.clear();
}


void Pawn::generatePossibleMoves()
{
	ChessPiece::generatePossibleMoves();
	int newX = x;
	int newY = y + (isWhite ? -1 : 1);
	if (chessboard->isInsideChessBoard(newX, newY) && chessboard->chessboard[newY][newX] == NULL) {
		ChessPiece* cp1 = this;
		ChessPiece* cp2 = chessboard->chessboard[newY][newX];
		ChessPieceMoveType cpmt = Move;
		pawnPromotion(cp1, cp2, newX, newY, cpmt);
	}

	if (canPlay2Step) {
		int newX = x;
		int newY = y + (isWhite ? -1 : 1);
		int newY2 = newY + (isWhite ? -1 : 1);
		if (chessboard->chessboard[newY][newX] == NULL && chessboard->chessboard[newY2][newX] == NULL) {
			ChessPiece* cp1 = this;
			ChessPiece* cp2 = chessboard->chessboard[newY2][newX];
			ChessPieceMoveType cpmt = Pawn2Step;
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY2, x, y, cpmt);
			chessboard->nextPossibleMoves.push_back(chesspieceMove);
		}
	}

	for (int i = 0; i < attackDirectionX.size(); i++) {
		int newX = x + (isWhite ? -1 : 1) * attackDirectionX[i];
		int newY = y + (isWhite ? -1 : 1) * attackDirectionY[i];
		int adjX = x + (isWhite ? -1 : 1) * attackDirectionX[i];
		int adjY = y;

		if (!chessboard->isInsideChessBoard(newX, newY))
			continue;

		if (chessboard->chessboard[newY][newX] != NULL && isWhite != chessboard->chessboard[newY][newX]->getIsWhite()) {
			ChessPiece* cp1 = this;
			ChessPiece* cp2 = chessboard->chessboard[newY][newX];
			ChessPieceMoveType cpmt = (cp2 == NULL ? Move : Kill);
			pawnPromotion(cp1, cp2, newX, newY, cpmt);

		}

		// En Passant
		if (chessboard->chessboard[adjY][adjX] != NULL &&
			isWhite != chessboard->chessboard[adjY][adjX]->getIsWhite() &&
			chessboard->chessboard[adjY][adjX]->getType() == (isWhite ? BlackPawnType : WhitePawnType) &&
			static_cast<Pawn*>(chessboard->chessboard[adjY][adjX])->getPrevMoveWas2Step()) {
			ChessPiece* cp1 = this;
			ChessPiece* cp2 = chessboard->chessboard[adjY][adjX];
			ChessPieceMoveType cpmt = EnPassant;
			pawnPromotion(cp1, cp2, newX, newY, cpmt);
		}
	}
}



bool Pawn::getPrevMoveWas2Step()
{
	return prevMoveWas2Step;
}

void Pawn::setCanPlay2Step(bool val) {
	canPlay2Step = val;
}

void Pawn::setPrevMoveWas2Step(bool val)
{
	prevMoveWas2Step = val;
}

void Pawn::pawnPromotion(ChessPiece* cp1, ChessPiece* cp2, int newX, int newY, ChessPieceMoveType cpmt)
{
	if (newY == (isWhite ? 0 : 7)) {
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			Rook* rook = new Rook(newX, newY, isWhite);
			rook->hasMoved = true;
			rook->numOfMoves = 1;
			chesspieceMove.chesspiece3 = rook;
			chessboard->nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Knight(newX, newY, isWhite);
			chessboard->nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Bishop(newX, newY, isWhite);
			chessboard->nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Queen(newX, newY, isWhite);
			chessboard->nextPossibleMoves.push_back(chesspieceMove);
		}
	}
	else {
		ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
		chessboard->nextPossibleMoves.push_back(chesspieceMove);
	}
}


Rook::Rook(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteRookType);
	else
		setType(BlackRookType);
}

Rook::~Rook()
{
	std::cout << "Rook Destructor " << std::endl;
	directionx.clear();
	directiony.clear();
}



Knight::Knight(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteKnightType);
	else
		setType(BlackKnightType);
}

Knight::~Knight()
{
	std::cout << "Knight Destructor " << std::endl;
	directionx.clear();
	directiony.clear();
}


Bishop::Bishop(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteBishopType);
	else
		setType(BlackBishopType);
}

Bishop::~Bishop()
{
	std::cout << "Bishop Destructor " << std::endl;
	directionx.clear();
	directiony.clear();
}



Queen::Queen(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteQueenType);
	else
		setType(BlackQueenType);
	//init(directionx, directiony, range, chessboard);
}

Queen::~Queen()
{
	std::cout << "Queen Destructor " << std::endl;
	directionx.clear();
	directiony.clear();
}


King::King(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteKingType);
	else
		setType(BlackKingType);
}

King::~King()
{
	std::cout << "King Destructor " << std::endl;
	directionx.clear();
	directiony.clear();
}


bool King::isInDanger()
{
	if (isDead)
		return true;
	return chessboard->isSquareInDanger(x, y, isWhite);
}

void King::generatePossibleMoves()
{
	ChessPiece::generatePossibleMoves();
	// Castling
	if (!hasMoved && !isInDanger()) {
		int y = isWhite ? 7 : 0;
		for (int i = 0; i < 8; i += 7)
		{
			bool flag = true;
			int newX = x + (i == 0 ? -1 : 1) * 2;
			int castleX = x;
			int newY = y;

			for (int j = (i == 0 ? 1 : x + 1); j < (i == 0 ? x : 7); j++) {
				if (chessboard->chessboard[y][j] != NULL)
					flag = false;
			}

			for (int j = 0; j <= 2; j++) {
				bool flag2 = chessboard->isSquareInDanger(castleX, y, isWhite);
				if (flag2)
					flag = false;
				castleX += (i == 0 ? -1 : 1);
			}


			if (flag && chessboard->chessboard[y][i] != NULL &&
				chessboard->chessboard[y][i]->getType() == (isWhite ? WhiteRookType : BlackRookType) &&
				static_cast<Rook*>(chessboard->chessboard[y][i])->hasMoved == false) {
				ChessPiece* cp1 = this;
				ChessPiece* cp2 = chessboard->chessboard[y][i];
				ChessPieceMoveType cpmt = Castle;
				ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
				chessboard->nextPossibleMoves.push_back(chesspieceMove);
			}
		}
	}
}



namespace visualize {
	int visualizeChessBoard(ChessBoard* chessboard);
}

int visualize::visualizeChessBoard(ChessBoard* chessboard)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL initialization failed: %s", SDL_GetError());
		return -1;
	}

	// Create a window
	SDL_Window* window = SDL_CreateWindow("SDL Starter Code", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	// Create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_Surface* tempSurface = IMG_Load("res/board_empty.png");
	SDL_Texture* chessboardTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = IMG_Load("res/pieces_white_1.png");
	SDL_Texture* whitePieceTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = IMG_Load("res/pieces_black_1.png");
	SDL_Texture* blackPieceTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	std::map<ChessPieceType, SDL_Rect> srcMp;
	std::vector<std::vector<SDL_Rect>> destMp(8, std::vector<SDL_Rect>(8));
	std::vector<ChessPieceType> whitePieces{WhiteRookType, WhiteKnightType, WhiteBishopType, WhiteQueenType, WhiteKingType, WhitePawnType};
	std::vector<ChessPieceType> blackPieces{BlackRookType, BlackKnightType, BlackBishopType, BlackQueenType, BlackKingType, BlackPawnType};

	SDL_Rect chessboard_rect;
	chessboard_rect.x = chessboard_rect.y = 0;
	chessboard_rect.w = chessboard_rect.h = 128;
	SDL_Rect chessboard_rect_dest = SDL_Rect(chessboard_rect);
	chessboard_rect_dest.x = 220;
	chessboard_rect_dest.y = 40;
	chessboard_rect_dest.h = 5 * chessboard_rect.h;
	chessboard_rect_dest.w = 5 * chessboard_rect.w;


	for (int i = 0; i < 6; i++) {
		SDL_Rect temp;
		temp.x = 15 * i;
		temp.y = 0;
		temp.w = temp.h = 15;
		srcMp[whitePieces[i]] = srcMp[blackPieces[i]] = temp;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard->chessboard[i][j] != NULL) {
				ChessPieceType chessPieceType = chessboard->chessboard[i][j]->getType();
				SDL_Rect temp;
				temp.w = 5 * srcMp[chessPieceType].w;
				temp.h = 5 * srcMp[chessPieceType].h;
				temp.x = chessboard_rect_dest.x + 5 * 4 + j * temp.w;
				temp.y = chessboard_rect_dest.y + 5 * 4 + i * temp.h;
				destMp[i][j] = temp;
			}
		}
	}

	// Main loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		// Event handling
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Clear the screen (black in this case)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Add your rendering code here
		// ...
		SDL_RenderCopy(renderer, chessboardTexture, &chessboard_rect, &chessboard_rect_dest);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (chessboard->chessboard[i][j] != NULL) {
					bool isWhite = chessboard->chessboard[i][j]->getIsWhite();
					ChessPieceType chessPieceType = chessboard->chessboard[i][j]->getType();
					/*std::cout << destMp[chessPieceType].x << " ";
					std::cout << destMp[chessPieceType].y << " ";*/
					if (isWhite)
						SDL_RenderCopy(renderer, whitePieceTexture, &srcMp[chessPieceType], &destMp[i][j]);
					else
						SDL_RenderCopy(renderer, blackPieceTexture, &srcMp[chessPieceType], &destMp[i][j]);
					//std::cout << std::endl;
				}
			}
		}
		// Update the screen
		SDL_RenderPresent(renderer);
	}

	// Clean up and exit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}



int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 20
	std::string fen2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; // 48
	std::string fen3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0"; // 14
	std::string fen4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; // 6
	std::string fen5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"; // 44
	std::string fen6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "; // 46
	std::string temp = "2b5/3P4/8/8/8/7K/8/7k w - - 0 0";

	{
		ChessBoard chessboard = ChessBoard(fen5);
		chessboard.genNextPossibleMoves();
		//visualize::visualizeChessBoard(&chessboard);
	}


	return 0;
}
