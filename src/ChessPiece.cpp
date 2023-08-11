#include "ChessPiece.h"
#include <iostream>
#include <assert.h>

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
	//std::cout << "ChessPiece " << (char)type << " " << id << std::endl;
	chessboard = NULL;
	directionx.clear();
	directiony.clear();
}


void deleteChessPiece(ChessPiece* cp) {
	Pawn* pawn;
	Rook* rook;
	Knight* knight;
	Bishop* bishop;
	Queen* queen;
	King* king;
	ChessPieceType cpt = cp->getType();
	//std::cout << (char)cpt << std::endl;
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
	//std::cout << "Pawn Destructor" << std::endl;
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
	//std::cout << "Rook Destructor " << std::endl;
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
	//std::cout << "Knight Destructor " << std::endl;
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
	//std::cout << "Bishop Destructor " << std::endl;
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
	//std::cout << "Queen Destructor " << std::endl;
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
	//std::cout << "King Destructor " << std::endl;
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
