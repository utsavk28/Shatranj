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
	int id = 0;
	bool isWhitesTurn = true;
	int halfMoveCounts = 0;
	int fullMoveCounts = 0;
	int enPassantX = -1, enPassantY = -1;
	std::vector<std::vector<ChessPiece*>> chessboard;
	std::vector<ChessPiece*> removedPawns;
	std::map<int, ChessPiece*> allChessPieces;
	std::map<ChessPieceType, std::vector<ChessPiece*>> chesspieceMap;
	std::vector<ChessPieceMove> prevMoves,nextPossibleMoves;

	ChessBoard();
	~ChessBoard();
	ChessBoard(std::string &fenn);
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

