#pragma once
#include "ChessBoard.h"
#include <string>
#include <tsl/htrie_map.h>
#include <tsl/htrie_set.h>


class ChessEngine {
private :
	tsl::htrie_map<char, int> transposition_table;
	tsl::htrie_map<char, int> fenMap;
public:
	ChessBoard* chessboard;
	ChessPieceMove nextOptimalMove;
	ChessEngine();
	~ChessEngine();
	ChessEngine(std::string& fen);
	void genNextPossibleMoves();
	int getNextPossibleMovesCount();
	void computeNextOptimalMove();
	int minmaxSearch(std::string fen, int depth, int limit, bool isMaxTurn);
	int alphabetaSearch(std::string fen, int depth, int limit, int alpha, int beta, bool isMaxTurn);
	std::vector<std::pair<std::string, int>> getAllChessboardWithLegalMoveCount(std::string fen, int depth, bool flag);
	int evaluateChessboard(ChessBoard* chessboard);
	int countNodes(std::string fen, int depth);
	void move(int oldX, int oldY, int newX, int newY,char promotedTo);
};