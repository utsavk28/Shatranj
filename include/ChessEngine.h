#pragma once
#include "ChessBoard.h"
#include <string>
#include <tsl/htrie_map.h>
#include <tsl/htrie_set.h>

const int kingWt = 2000;
const int queenWt = 90;
const int rookWt = 50;
const int bishopWt = 30;
const int knightWt = 30;
const int pawnWt = 10;
const int pawnWt2 = 5; // Passed, Isolated & Skipped Pawn
const int mobilityWt = 10; // Mobility 

const std::vector<std::vector<int>> posKnightWt{
	{-50, -40, -30, -30, -30, -30, -40, -50},
	{ -40, -20, 0, 0, 0, 0, -20, -40 },
	{ -30, 0, 10, 15, 15, 10, 0, -30 },
	{ -30, 5, 15, 20, 20, 15, 5, -30 },
	{ -30, 5, 15, 20, 20, 15, 5, -30 },
	{ -30, 0, 10, 15, 15, 10, 0, -30 },
	{ -40, -20, 0, 0, 0, 0, -20, -40 },
	{ -50, -40, -30, -30, -30, -30, -40, -50 },
};


const std::vector<std::vector<int>> posRookWt{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{ 5, 10, 10, 10, 10, 10, 10, 5 },
	{ -5, 0, 0, 0, 0, 0, 0, -5 },
	{ -5, 0, 0, 0, 0, 0, 0, -5 },
	{ -5, 0, 0, 0, 0, 0, 0, -5 },
	{ -5, 0, 0, 0, 0, 0, 0, -5 },
	{ -5, 0, 0, 0, 0, 0, 0, -5 },
	{ 0, 0, 0, 5, 5, 0, 0, 0 },
};

const std::vector<std::vector<int>> posQueenWt{
	{-20, -10, -10, -5, -5, -10, -10, -20},
	{ -10, 0, 0, 0, 0, 0, 0, -10 },
	{ -10, 0, 5, 5, 5, 5, 0, -10 },
	{ -5, 0, 5, 5, 5, 5, 0, -5 },
	{ 0, 0, 5, 5, 5, 5, 0, -5 },
	{ -10, 5, 5, 5, 5, 5, 0, -10 },
	{ -10, 0, 5, 0, 0, 0, 0, -10 },
	{ -20, -10, -10, -5, -5, -10, -10, -20 }};

const std::vector<std::vector<int>> posBishopWt{
	{-20, -10, -10, -10, -10, -10, -10, -20},
	{ -10, 0, 0, 0, 0, 0, 0, -10 },
	{ -10, 0, 5, 10, 10, 5, 0, -10 },
	{ -10, 5, 5, 10, 10, 5, 5, -10 },
	{ -10, 0, 10, 10, 10, 10, 0, -10 },
	{ -10, 10, 10, 10, 10, 10, 10, -10 },
	{ -10, 5, 0, 0, 0, 0, 5, -10 },
	{ -20, -10, -10, -10, -10, -10, -10, -20 },
};

const std::vector<std::vector<int>> posPawnStartWt{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{ 50, 50, 50, 50, 50, 50, 50, 50 },
	{ 10, 10, 20, 30, 30, 20, 10, 10 },
	{ 5, 5, 10, 25, 25, 10, 5, 5 },
	{ 0, 0, 0, 20, 20, 0, 0, 0 },
	{ 5, -5, -10, 0, 0, -10, -5, 5 },
	{ 5, 10, 10, -20, -20, 10, 10, 5 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }};

const std::vector<std::vector<int>> posPawnEndWt{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{ 80, 80, 80, 80, 80, 80, 80, 80 },
	{ 50, 50, 50, 50, 50, 50, 50, 50 },
	{ 30, 30, 30, 30, 30, 30, 30, 30 },
	{ 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 10, 10, 10, 10, 10, 10, 10, 10 },
	{ 10, 10, 10, 10, 10, 10, 10, 10 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }};

const std::vector<std::vector<int>> posKingStartWt{
	{-80, -70, -70, -70, -70, -70, -70, -80},
	{ -60, -60, -60, -60, -60, -60, -60, -60 },
	{ -40, -50, -50, -60, -60, -50, -50, -40 },
	{ -30, -40, -40, -50, -50, -40, -40, -30 },
	{ -20, -30, -30, -40, -40, -30, -30, -20 },
	{ -10, -20, -20, -20, -20, -20, -20, -10 },
	{ 20, 20, -5, -5, -5, -5, 20, 20 },
	{ 20, 30, 10, 0, 0, 10, 30, 20 },
};

const std::vector<std::vector<int>> posKingEndWt{
	{-20, -10, -10, -10, -10, -10, -10, -20},
	{ -5, 0, 5, 5, 5, 5, 0, -5 },
	{ -10, -5, 20, 30, 30, 20, -5, -10 },
	{ -15, -10, 35, 45, 45, 35, -10, -15 },
	{ -20, -15, 30, 40, 40, 30, -15, -20 },
	{ -25, -20, 20, 25, 25, 20, -20, -25 },
	{ -30, -25, 0, 0, 0, 0, -25, -30 },
	{ -50, -30, -30, -30, -30, -30, -30, -50 }};

const std::vector<std::vector<int>> posKnightStartWt = posKnightWt;
const std::vector<std::vector<int>> posKnightEndWt = posKnightWt;

const std::vector<std::vector<int>> posBishopStartWt = posBishopWt;
const std::vector<std::vector<int>> posBishopEndWt = posBishopWt;

const std::vector<std::vector<int>> posRookStartWt = posRookWt;
const std::vector<std::vector<int>> posRookEndWt = posRookWt;

const std::vector<std::vector<int>> posQueenStartWt = posQueenWt;
const std::vector<std::vector<int>> posQueenEndWt = posQueenWt;

class ChessEngine {
private:
	tsl::htrie_map<char, int> transposition_table;
	tsl::htrie_map<char, int> fenMap;
public:
	ChessBoard* chessboard;
	ChessPieceMove nextOptimalMove;
	ChessEngine();
	~ChessEngine();
	ChessEngine(std::string& fen);
	void genNextPossibleMoves();
	void copy(ChessPieceMove move);
	int getNextPossibleMovesCount();
	void computeNextOptimalMove();
	int minmaxSearch(std::string fen, int depth, int limit, bool isMaxTurn);
	int alphabetaSearch(std::string fen, int depth, int limit, int alpha, int beta, bool isMaxTurn);
	std::vector<std::pair<std::string, int>> getAllChessboardWithLegalMoveCount(std::string fen, int depth, bool flag);
	int evaluateChessboard(ChessBoard* chessboard);
	int countNodes(std::string fen, int depth);
	void move(int oldX, int oldY, int newX, int newY, char promotedTo);
};