#include "ChessEngine.h"
#include <iostream>
#include <assert.h>

static int nodeCount = 0;


ChessEngine::ChessEngine() {
	chessboard = new ChessBoard();
	transposition_table.clear();
	fenMap.clear();
}

ChessEngine::~ChessEngine()
{
	delete chessboard;
	transposition_table.clear();
	fenMap.clear();
}

ChessEngine::ChessEngine(std::string& fen) {
	chessboard = new ChessBoard(fen);
	transposition_table.clear();
	fenMap.clear();
}

void ChessEngine::genNextPossibleMoves() {
	chessboard->genNextPossibleMoves();
}

int ChessEngine::getNextPossibleMovesCount() {

	return chessboard->getNextPossibleMovesCount();
}

int ChessEngine::evaluateChessboard(ChessBoard* chessboard)
{
	if (chessboard->chesspieceMap[WhiteKingType].size() == 0)
		return chessboard->isWhitesTurn;

	const int kingWt = 2000;
	const int queenWt = 90;
	const int rookWt = 50;
	const int bishopWt = 30;
	const int knightWt = 30;
	const int pawnWt = 10;
	const int pawnWt2 = 5; // Passed, Isolated & Skipped Pawn
	const int mobilityWt = 1; // Mobility 

	int materialScore = 0;
	int mobilityScore = 0;
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (chessboard->chessboard[i][j] != NULL) {
				switch (chessboard->chessboard[i][j]->getType())
				{
				case WhiteKingType:
					materialScore += kingWt;
					break;
				case BlackKingType:
					materialScore -= kingWt;
					break;

				case WhiteQueenType:
					materialScore += queenWt;
					break;
				case BlackQueenType:
					materialScore -= queenWt;
					break;

				case WhiteRookType:
					materialScore += rookWt;
					break;
				case BlackRookType:
					materialScore -= rookWt;
					break;

				case WhiteBishopType:
					materialScore += bishopWt;
					break;
				case BlackBishopType:
					materialScore -= bishopWt;
					break;

				case WhiteKnightType:
					materialScore += knightWt;
					break;
				case BlackKnightType:
					materialScore -= knightWt;
					break;

				case WhitePawnType:
					materialScore += pawnWt;
					break;
				case BlackPawnType:
					materialScore -= pawnWt;
					break;


				default:
					break;
				}
			}
		}
	}


	int score = (materialScore + mobilityScore) * (chessboard->isWhitesTurn ? 1 : -1);

	return score;
}

int ChessEngine::countNodes(std::string fen, int depth)
{
	if (depth == 0)
		return 1;
	ChessBoard* chessboard = new ChessBoard(fen);
	chessboard->genNextPossibleMoves();

	int sum = 0;
	if (depth == 1) {
		sum = (int)chessboard->nextPossibleMoves.size();
	}
	else {
		for (auto move : chessboard->nextPossibleMoves) {
			int oldX = move.oldX;
			int oldY = move.oldY;
			int newX = move.newX;
			int newY = move.newY;
			char promotedTo = move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType();
			chessboard->move(oldX, oldY, newX, newY, promotedTo);
			std::string newFen = chessboard->toFen();
			chessboard->undo();
			sum += countNodes(newFen, depth - 1);
		}
	}

	delete chessboard;

	return sum;
}

void ChessEngine::move(int oldX, int oldY, int newX, int newY, char promotedTo)
{
	chessboard->move(oldX, oldY, newX, newY, promotedTo);
}

std::vector<std::pair<std::string, int>> ChessEngine::getAllChessboardWithLegalMoveCount(std::string fen, int depth, bool flag) {
	std::vector<std::pair<std::string, int>> res;
	ChessBoard* chessboard = new ChessBoard(fen);
	chessboard->genNextPossibleMoves();
	if (flag || depth == 0)
		res.push_back({ fen,(int)chessboard->nextPossibleMoves.size() });

	if (depth == 0) {
		return res;
	}

	for (auto move : chessboard->nextPossibleMoves) {
		int oldX = move.oldX;
		int oldY = move.oldY;
		int newX = move.newX;
		int newY = move.newY;
		char promotedTo = move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType();
		chessboard->move(oldX, oldY, newX, newY, promotedTo);
		std::string newFen = chessboard->toFen();
		std::vector<std::pair<std::string, int>> res2 = getAllChessboardWithLegalMoveCount(newFen, depth - 1, flag);
		res.insert(res.end(), res2.begin(), res2.end());
		chessboard->undo();
	}
	delete chessboard;

	return res;
}


void ChessEngine::computeNextOptimalMove()
{
	int inf = INT_MAX - 1;
	nextOptimalMove = ChessPieceMove();
	nodeCount = 0;
	std::string fen = chessboard->toFen();
	chessboard->genNextPossibleMoves();
	//minmaxSearch(fen, 0, 2, true);
	alphabetaSearch(fen, 0, 3, -inf, inf, true);
	std::cout << nodeCount << std::endl;
	transposition_table.clear();
	fenMap.clear();
}

void ChessEngine::copy(ChessPieceMove move) {
	int oldX = move.oldX;
	int oldY = move.oldY;
	int newX = move.newX;
	int newY = move.newY;
	char cpt = (move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType());
	for (auto& it : chessboard->nextPossibleMoves) {
		if (it.oldX == oldX && it.oldY == oldY &&
			it.newX == newX && it.newY == newY &&
			cpt == (it.chesspiece3 == NULL ? ' ' : (char)it.chesspiece3->getType())) {
			nextOptimalMove = it;
		}
	}
}

int ChessEngine::minmaxSearch(std::string fen, int depth, int limit, bool isMaxTurn)
{
	if (fenMap[fen])
		return transposition_table[fen];
	fenMap[fen] = true;

	ChessBoard* currCB = new ChessBoard(fen);
	nodeCount++;
	int val;
	if (depth == limit) {
		val = evaluateChessboard(currCB);
	}
	else {
		currCB->genNextPossibleMoves();
		val = (INT_MAX - 2) * (isMaxTurn ? -1 : 1);

		for (auto move : currCB->nextPossibleMoves) {
			int oldX = move.oldX;
			int oldY = move.oldY;
			int newX = move.newX;
			int newY = move.newY;
			char promotedTo = move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType();
			currCB->move(oldX, oldY, newX, newY, promotedTo);
			if (currCB->hasPiece(currCB->isWhitesTurn ? BlackKingType : WhiteKingType) == false) {
				val = (INT_MAX - 2) * (isMaxTurn ? 1 : -1);
				break;
			}
			std::string newFen = currCB->toFen();
			currCB->undo();
			int searchVal = minmaxSearch(newFen, depth + 1, limit, isMaxTurn ^ true);
			if (isMaxTurn) {
				if (val < searchVal) {
					if (depth == 0) {
						copy(move);
					}
					val = searchVal;

				}
			}
			else {
				if (val > searchVal) {
					if (depth == 0) {
						copy(move);
					}
					val = searchVal;
				}
			}
		}
	}

	delete currCB;
	return transposition_table[fen] = val;
}



int ChessEngine::alphabetaSearch(std::string fen, int depth, int limit, int alpha, int beta, bool isMaxTurn)
{
	if (fenMap[fen])
		return transposition_table[fen];
	fenMap[fen] = true;

	ChessBoard* currCB = new ChessBoard(fen);
	nodeCount++;
	int val;
	if (depth == limit) {
		val = evaluateChessboard(currCB);
	}
	else {
		currCB->genNextPossibleMoves();
		val = (INT_MAX - 2) * (isMaxTurn ? -1 : 1);

		for (auto move : currCB->nextPossibleMoves) {
			int oldX = move.oldX;
			int oldY = move.oldY;
			int newX = move.newX;
			int newY = move.newY;
			char promotedTo = move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType();
			currCB->move(oldX, oldY, newX, newY, promotedTo);
			if (currCB->hasPiece(currCB->isWhitesTurn ? BlackKingType : WhiteKingType) == false) {
				val = (INT_MAX - 2) * (isMaxTurn ? 1 : -1);
				break;
			}

			std::string newFen = currCB->toFen();
			currCB->undo();
			int searchVal = alphabetaSearch(newFen, depth + 1, limit, alpha, beta, isMaxTurn ^ true);
			if (isMaxTurn) {
				if (val < searchVal) {
					if (depth == 0) {
						copy(move);
					}
					val = searchVal;
					alpha = searchVal;
				}
				if (beta <= alpha)
					break;
			}
			else {
				if (val > searchVal) {
					if (depth == 0) {
						copy(move);
					}
					val = searchVal;
					beta = searchVal;
				}
				if (beta <= alpha)
					break;
			}
		}

	}


	delete currCB;

	return transposition_table[fen] = val;
}
