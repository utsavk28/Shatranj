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


ChessPieceMove ChessPiece::isValidMove(int x, int y,char promotedTo)
{
	if (!chessboard->isInsideChessBoard(x, y))
		return ChessPieceMove();
	for (auto& it : nextPossibleMoves)
		if (it.newX == x && it.newY == y) {
			if (promotedTo == ' ' || (it.chesspiece3 != NULL && promotedTo == (char)it.chesspiece3->getType()))
				return it;
		}
	return ChessPieceMove();
}

void ChessPiece::setType(ChessPieceType type)
{
	this->type = type;
}

void ChessPiece::move(int x, int y,char promotedTo=' ')
{
	ChessPieceMove chesspieceMove = isValidMove(x, y,promotedTo);
	if (chesspieceMove.chesspiece1 != NULL) {
		chessboard->prevMoves.push_back(chesspieceMove);
		int oldX = chesspieceMove.oldX;
		int oldY = chesspieceMove.oldY;
		int newX = chesspieceMove.newX;
		int newY = chesspieceMove.newY;
		int rookX, rookY;

		if (chesspieceMove.chesspiece2) {
			int x2 = chesspieceMove.chesspiece2->x;
			int y2 = chesspieceMove.chesspiece2->y;
			chessboard->chessboard[y2][x2] = NULL;
		}

		chesspieceMove.chesspiece1->x = newX;
		chesspieceMove.chesspiece1->y = newY;
		chesspieceMove.chesspiece1->numOfMoves++;
		chessboard->chessboard[oldY][oldX] = NULL;
		chessboard->chessboard[newY][newX] = chesspieceMove.chesspiece1;
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
				chessboard->chessboard[rookY][rookX] = chesspieceMove.chesspiece2;
				chesspieceMove.chesspiece2->numOfMoves++;
				break;
			default:
				break;
			}
		}

		if (chesspieceMove.chesspiece1->getType() == (isWhite ? WhitePawnType : BlackPawnType) && chesspieceMove.isPromoted) {
			ChessPieceType cpt = isWhite ? WhitePawnType : BlackPawnType;
			chesspieceMove.chesspiece1->isDead = true;
			chesspieceMove.chesspiece1->x = -1;
			chesspieceMove.chesspiece1->y = -1;

			std::vector<ChessPiece*> cpv = chessboard->chesspieceMap[cpt];
			cpv.erase(std::remove_if(cpv.begin(), cpv.end(), [&](ChessPiece* cp) {
				return  cp->id == chesspieceMove.chesspiece1->id;
				}), cpv.end());
			chessboard->chesspieceMap[cpt] = cpv;

			ChessPieceType cpt1 = chesspieceMove.chesspiece3->getType();
			chesspieceMove.chesspiece3->x = newX;
			chesspieceMove.chesspiece3->y = newY;
			chessboard->chessboard[newY][newX] = chesspieceMove.chesspiece3;
			chesspieceMove.chesspiece3->numOfMoves = 1;
			chessboard->chesspieceMap[cpt1].push_back(chesspieceMove.chesspiece3);
		}

		postMoveProcess(chesspieceMove);
		chessboard->isWhitesTurn ^= true;
	}


}

void ChessPiece::undo() {


	if (chessboard->prevMoves.size() > 0) {
		ChessPieceMove prevMove = chessboard->prevMoves.back();
		chessboard->prevMoves.pop_back();
		int oldX = prevMove.oldX;
		int oldY = prevMove.oldY;
		int newX = prevMove.newX;
		int newY = prevMove.newY;
		int rookX, rookY;
		int enX, enY;

		if (prevMove.chesspiece1->getType() == (isWhite ? WhitePawnType : BlackPawnType) && prevMove.isPromoted) {
			ChessPieceType cpt = isWhite ? WhitePawnType : BlackPawnType;
			ChessPieceType cpt1 = prevMove.chesspiece3->getType();

			prevMove.chesspiece1->isDead = false;
			prevMove.chesspiece1->x = newX;
			prevMove.chesspiece1->y = newY;

			chessboard->chesspieceMap[cpt].push_back(prevMove.chesspiece1);
			chessboard->chessboard[newY][newX] = prevMove.chesspiece1;

			std::vector<ChessPiece*> cpv = chessboard->chesspieceMap[cpt1];
			cpv.erase(std::remove_if(cpv.begin(), cpv.end(), [&](ChessPiece* cp) {
				return  cp->id == prevMove.chesspiece3->id;
				}), cpv.end());
			chessboard->chesspieceMap[cpt1] = cpv;

			ChessPiece* temp = prevMove.chesspiece3;
			if(temp)
				delete temp;
		}


		prevMove.chesspiece1->x = oldX;
		prevMove.chesspiece1->y = oldY;
		chessboard->chessboard[newY][newX] = NULL;
		chessboard->chessboard[oldY][oldX] = prevMove.chesspiece1;
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
			chessboard->chessboard[enY][enX] = prevMove.chesspiece2;
			break;
		case Kill:
			prevMove.chesspiece2->isDead = false;
			prevMove.chesspiece2->x = newX;
			prevMove.chesspiece2->y = newY;
			chessboard->chessboard[newY][newX] = prevMove.chesspiece2;
			break;
		case Castle:
			rookX = newX > oldX ? 7 : 0;
			rookY = newY;
			prevMove.chesspiece2->x = rookX;
			prevMove.chesspiece2->y = rookY;
			chessboard->chessboard[rookY][rookX == 7 ? 5 : 3] = NULL;
			chessboard->chessboard[rookY][rookX] = prevMove.chesspiece2;
			prevMove.chesspiece2->numOfMoves--;
			break;
		default:
			break;
		}


		postUndoProcess(prevMove);
		chessboard->isWhitesTurn ^= true;
	}
}

void ChessPiece::generatePossibleMoves() {
	nextPossibleMoves.clear();
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
				nextPossibleMoves.push_back(chesspieceMove);
				if (cpmt == Kill)
					break;
			}
			else
				break;
		}
	}

}

void ChessPiece::validatePossibleMoves() {
	std::vector<ChessPieceMove> validPossibleMoves{};

	std::vector<std::vector<char>> v(8, std::vector<char>(8, '_'));
	std::vector<std::vector<char>> v1(8, std::vector<char>(8, '_'));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard->chessboard[i][j])
				v[i][j] = (char)chessboard->chessboard[i][j]->getType();
		}
	}
	std::string fen = chessboard->toFen();

	for (auto& nextMove : nextPossibleMoves) {
		char promotedTo = nextMove.chesspiece3 == NULL ? ' ' : (char)nextMove.chesspiece3->getType();
		move(nextMove.newX, nextMove.newY, promotedTo);

		bool flag = chessboard->isKingVulnerable(chessboard->isWhitesTurn ^ true);
		if (!flag)
			validPossibleMoves.push_back(nextMove);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (chessboard->chessboard[i][j])
					v1[i][j] = (char)chessboard->chessboard[i][j]->getType();
				else
					v1[i][j] = '_';
			}
		}
		undo();

		std::string fenCheck = chessboard->toFen();
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
			for (auto it : chessboard->chessboard) {
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

void ChessPiece::postMoveProcess(ChessPieceMove move) {
	for (ChessPiece* it : chessboard->chesspieceMap[WhitePawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);
	}
	for (ChessPiece* it : chessboard->chesspieceMap[BlackPawnType]) {
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

void ChessPiece::postUndoProcess(ChessPieceMove move) {
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


	for (ChessPiece* it : chessboard->chesspieceMap[WhitePawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);

	}
	for (ChessPiece* it : chessboard->chesspieceMap[BlackPawnType]) {
		Pawn* pawn = static_cast<Pawn*>(it);
		pawn->setPrevMoveWas2Step(false);
	}

	if (chessboard->prevMoves.size() > 0) {
		move = chessboard->prevMoves.back();
		switch (move.chesspiece1->getType())
		{
		case WhitePawnType:
		case BlackPawnType:
			if (move.chesspieceMoveType == Pawn2Step)
				static_cast<Pawn*>(move.chesspiece1)->setPrevMoveWas2Step(true);
		}
	}
	else {
		if (chessboard->enPassantX != -1 && chessboard->enPassantY != -1) {
			static_cast<Pawn*>(chessboard->chessboard[chessboard->enPassantY][chessboard->enPassantX])->setPrevMoveWas2Step(true);
		}
	}

}

ChessPiece::~ChessPiece()
{
	/*for (auto& it : nextPossibleMoves) {
		delete it;
	}*/
	//nextPossibleMoves.clear();
	chessboard = NULL;
	nextPossibleMoves.clear();
	directionx.clear();
	directiony.clear();
	std::cout << "Hello " << (char)type << " " << id << std::endl;
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


std::vector< ChessPieceMove>& ChessPiece::getNextPossibleMoves()
{
	return nextPossibleMoves;
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
			nextPossibleMoves.push_back(chesspieceMove);
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
			nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Knight(newX, newY, isWhite);
			nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Bishop(newX, newY, isWhite);
			nextPossibleMoves.push_back(chesspieceMove);
		}
		{
			ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
			chesspieceMove.isPromoted = true;
			chesspieceMove.chesspiece3 = new Queen(newX, newY, isWhite);
			nextPossibleMoves.push_back(chesspieceMove);
		}
	}
	else {
		ChessPieceMove chesspieceMove = ChessPieceMove(cp1, cp2, newX, newY, x, y, cpmt);
		nextPossibleMoves.push_back(chesspieceMove);
	}
}


Rook::Rook(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteRookType);
	else
		setType(BlackRookType);
}



Knight::Knight(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteKnightType);
	else
		setType(BlackKnightType);
}


Bishop::Bishop(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteBishopType);
	else
		setType(BlackBishopType);
}



Queen::Queen(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteQueenType);
	else
		setType(BlackQueenType);
	//init(directionx, directiony, range, chessboard);
}


King::King(int x, int y, bool isWhite) :ChessPiece(x, y, isWhite) {
	init(directionx, directiony, range);
	if (isWhite)
		setType(WhiteKingType);
	else
		setType(BlackKingType);
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
				nextPossibleMoves.push_back(chesspieceMove);
			}
		}
	}
}
