#include "ChessBoard.h"
#include "enum.h"

#include <iostream>
#include <assert.h>

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
	chessboard.resize(8, std::vector<ChessPiece*>(8, NULL));
	std::vector<std::vector<ChessPieceType>> chessTypeBoard = {
		{BlackRookType,BlackKnightType,BlackBishopType,BlackQueenType,BlackKingType,BlackBishopType,BlackKnightType,BlackRookType},
		{BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType,BlackPawnType},
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

ChessBoard::~ChessBoard()
{
	for (auto& it : allChessPieces) {
		deleteChessPiece(it.second);
	}
	for (auto& it : nextPossibleMoves) {
		cleanUp(it);
	}
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
				if (chessboard[i][j]->getIsWhite())
					whitePieceCount++;
				else
					blackPieceCount++;
				chessboard[i][j]->setChessBoard(this);
				allChessPieces[chessboard[i][j]->id] = chessboard[i][j];
				chesspieceMap[chessboard[i][j]->getType()].push_back(chessboard[i][j]);
			}

		}
	}
}


ChessPieceMove ChessBoard::isValidMove(int oldX, int oldY, int newX, int newY, char promotedTo)
{
	if (!isInsideChessBoard(newX, newY) || !isInsideChessBoard(oldX, oldY))
		return ChessPieceMove();
	for (auto& it : nextPossibleMoves) {
		if (it.newX == newX && it.newY == newY && it.oldX == oldX && it.oldY == oldY) {
			if (promotedTo == ' ' || (it.chesspiece3 != NULL && promotedTo == (char)it.chesspiece3->getType())) {
				return it;
			}
		}
	}
	return ChessPieceMove();
}

void ChessBoard::cleanUp(ChessPieceMove move) {
	ChessPiece* temp = move.chesspiece3;
	if (temp) {
		int sz = prevMoves.size();
		if (sz >= 2 && prevMoves[sz - 2].chesspiece3) {
			if (temp->id == prevMoves[sz - 2].chesspiece3->id)
				return;
		}
		if (sz >= 1 && prevMoves[sz - 1].chesspiece3) {
			if (temp->id == prevMoves[sz - 1].chesspiece3->id)
				return;
		}
		deleteChessPiece(temp);
	}

}

void ChessBoard::move(int oldX, int oldY, int newX, int newY, char promotedTo = ' ')
{
	ChessPieceMove chesspieceMove = isValidMove(oldX, oldY, newX, newY, promotedTo);
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
				if (chesspieceMove.chesspiece2->getIsWhite())
					whitePieceCount--;
				else
					blackPieceCount--;
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
	else {
		std::cout << "Execute nhi huwa" << std::endl;
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
			if (prevMove.chesspiece2->getIsWhite())
				whitePieceCount++;
			else
				blackPieceCount++;
			break;
		case Kill:
			prevMove.chesspiece2->isDead = false;
			prevMove.chesspiece2->x = newX;
			prevMove.chesspiece2->y = newY;
			chessboard[newY][newX] = prevMove.chesspiece2;
			if (prevMove.chesspiece2->getIsWhite())
				whitePieceCount++;
			else
				blackPieceCount++;
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


	std::string fen = toFen();

	for (auto& nextMove : nextPossibleMoves) {
		char promotedTo = nextMove.chesspiece3 == NULL ? ' ' : (char)nextMove.chesspiece3->getType();
		move(nextMove.oldX, nextMove.oldY, nextMove.newX, nextMove.newY, promotedTo);
		bool flag = isKingVulnerable(isWhitesTurn ^ true);
		undo();
		if (!flag)
			validPossibleMoves.push_back(nextMove);
		else
			cleanUp(nextMove);
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
	Pawn* pawn;
	King* king;
	std::vector<ChessPieceType> cpts;
	cpts.push_back({ isWhitesTurn ? WhitePawnType : BlackPawnType });
	cpts.push_back({ isWhitesTurn ? WhiteKnightType : BlackKnightType });
	cpts.push_back({ isWhitesTurn ? WhiteQueenType : BlackQueenType });
	cpts.push_back({ isWhitesTurn ? WhiteBishopType : BlackBishopType });
	cpts.push_back({ isWhitesTurn ? WhiteRookType : BlackRookType });
	cpts.push_back({ isWhitesTurn ? WhiteKingType : BlackKingType });
	for (auto cpt : cpts) {
		for (auto& it : chesspieceMap[cpt]) {
			if (it->isDead)
				continue;
			switch (it->getType()) {
			case WhitePawnType:
			case BlackPawnType:
				pawn = static_cast<Pawn*>(it);
				pawn->generatePossibleMoves();
				break;
			case WhiteKingType:
			case BlackKingType:
				king = static_cast<King*>(it);
				king->generatePossibleMoves();
				break;
			default:
				it->generatePossibleMoves();
				break;
			}
		}
	}

	validatePossibleMoves();
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


