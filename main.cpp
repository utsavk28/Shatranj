#include "ChessEngine.h"
#include "visualize.h"
#include <iostream>
#include <assert.h>

int getAllChessboardinDepth(std::string fen, int depth) {
	ChessEngine chessEngine = ChessEngine(fen);
	return chessEngine.countNodes(fen, depth);
}


int main(int argc, char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 20
	std::string fen2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; // 48
	std::string fen3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0"; // 14
	std::string fen4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; // 6
	std::string fen5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"; // 44
	std::string fen6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "; // 46
	std::string temp = "rnbqkbnr/ppppp2p/8/8/8/8/PPP2P1P/RNBQKBNR w KQkq - 0 1";
	ChessEngine chessEngine = ChessEngine(fen1);
	bool playOnce = false;
	//visualize::visualizeChessBoard(chessEngine.chessboard);
	/*for (auto it : chessEngine.chessboard->nextPossibleMoves) {
		std::cout << it.oldX << it.oldY << it.newX << it.newY << (it.chesspiece3 == NULL ? ' ' : (char)it.chesspiece3->getType()) << std::endl;
	}
	std::cout << move.oldX << move.oldY << move.newX << move.newY;
	std::cout << (move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType()) << std::endl;*/

	while(true)
	{
		chessEngine.computeNextOptimalMove();
		ChessPieceMove move = chessEngine.nextOptimalMove;
		chessEngine.move(move.oldX, move.oldY, move.newX, move.newY, (move.chesspiece3 == NULL ? ' ' : (char)move.chesspiece3->getType()));
		if (chessEngine.nextOptimalMove.chesspiece1 != NULL) {
			std::cout << (char)chessEngine.nextOptimalMove.chesspiece1->getType() << std::endl;
			std::cout << chessEngine.nextOptimalMove.oldX << " " << chessEngine.nextOptimalMove.oldY << std::endl;
			std::cout << chessEngine.nextOptimalMove.newX << " " << chessEngine.nextOptimalMove.newY << std::endl;
		}
		visualize::visualizeChessBoard(chessEngine.chessboard);
		if (playOnce)
			break;
	}

	//system("pause");
	return 0;
}
