#pragma once
#include "ChessPiece.h"
#include "enum.h"
#include <vector>
#include <map>
#include <string>

class ChessPiece;
struct ChessPieceMove;

class ChessBoard {
public:
	bool isWhitesTurn = true;
	int halfMoveCounts = 0;
	int fullMoveCounts = 0;
	int enPassantX = -1, enPassantY = -1;
	std::vector<std::vector<ChessPiece*>> chessboard;
	std::map<ChessPieceType, std::vector<ChessPiece*>> chesspieceMap;
	std::vector<ChessPieceMove> prevMoves,nextPossibleMoves;

	ChessBoard();
	~ChessBoard();
	ChessBoard(std::string &fenn);
	void init(std::vector<std::vector<ChessPieceType>> chessTypeBoard);
	bool isKingVulnerable();
	bool isKingVulnerable(bool isWhitesTurn);
	bool isSquareInDanger(int x, int y, bool isWhite);
	void setIsWhitesTurn(std::string& s);
	void setupCastling(std::string& s);
	void setupEnPassant(std::string& s);	
	void genNextPossibleMoves();
	int getNextPossibleMovesCount();
	bool isInsideChessBoard(int x, int y);
	std::string toFen();
	bool hasPiece(ChessPieceType cpt);
};

