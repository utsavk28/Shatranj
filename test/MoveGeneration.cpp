#include <catch2/catch_test_macros.hpp>
#include "ChessEngine.h"
#include <string>

int getTotalNodes(std::string& fen) {
	ChessEngine chessEngine = ChessEngine(fen);
	chessEngine.genNextPossibleMoves();
	return chessEngine.getNextPossibleMovesCount();
}

int getAllChessboardinDepth(std::string fen,int depth) {
	ChessEngine chessEngine = ChessEngine(fen);
	return chessEngine.countNodes(fen,depth);
}

// https://www.chessprogramming.org/Perft_Results
TEST_CASE("Move Generation : Perft Results", "[MoveGeneration]") {

	std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 20
	std::string fen2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; // 48
	std::string fen3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0"; // 14
	std::string fen4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; // 6
	std::string fen5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"; // 44
	std::string fen6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "; // 46

	REQUIRE(getTotalNodes(fen1) == 20);
	REQUIRE(getTotalNodes(fen2) == 48);
	REQUIRE(getTotalNodes(fen3) == 14);
	//REQUIRE(getTotalNodes(fen4) == 6);
	REQUIRE(getTotalNodes(fen5) == 44);
	REQUIRE(getTotalNodes(fen6) == 46);
}

// https://www.chessprogramming.org/Perft_Results
TEST_CASE("Tree Chessboard Generation : Perft Results", "[TreeChessboardGeneration]") {

	std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 1, 20, 400, 8902, 197281, 4865609
	std::string fen2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; // 1, 48, 2039, 97862, 4085603
	std::string fen3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0"; // 1, 14, 191, 2812, 43238, 674624
	std::string fen4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; // 1, 6, 264, 9467, 422333
	std::string fen5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"; // 1, 44, 1486, 62379, 2103487
	std::string fen6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "; // 1, 46, 2079, 89890, 3894594

	REQUIRE(getAllChessboardinDepth(fen1, 0) == 1);
	REQUIRE(getAllChessboardinDepth(fen1, 1) == 20);
	REQUIRE(getAllChessboardinDepth(fen1, 2) == 400);
	REQUIRE(getAllChessboardinDepth(fen1, 3) == 8902);
	//REQUIRE(getAllChessboardinDepth(fen1, 4) == 197281);
	//REQUIRE(getAllChessboardinDepth(fen1, 5) == 4865609);

	REQUIRE(getAllChessboardinDepth(fen2, 1) == 48);
	REQUIRE(getAllChessboardinDepth(fen2, 2) == 2039);
	//REQUIRE(getAllChessboardinDepth(fen2, 3) == 97862);
	//REQUIRE(getAllChessboardinDepth(fen2, 4) == 4085603);

	REQUIRE(getAllChessboardinDepth(fen3, 1) == 14);
	REQUIRE(getAllChessboardinDepth(fen3, 2) == 191);
	REQUIRE(getAllChessboardinDepth(fen3, 3) == 2812);
	//REQUIRE(getAllChessboardinDepth(fen3, 4) == 43238);
	//REQUIRE(getAllChessboardinDepth(fen3, 5) == 674624);

	REQUIRE(getAllChessboardinDepth(fen5, 1) == 44);
	REQUIRE(getAllChessboardinDepth(fen5, 2) == 1486);
	//REQUIRE(getAllChessboardinDepth(fen5, 3) == 62379);
	//REQUIRE(getAllChessboardinDepth(fen5, 4) == 2103487);

	REQUIRE(getAllChessboardinDepth(fen6, 1) == 46);
	REQUIRE(getAllChessboardinDepth(fen6, 2) == 2079);
	//REQUIRE(getAllChessboardinDepth(fen6, 3) == 89890);
	//REQUIRE(getAllChessboardinDepth(fen6, 4) == 3894594);
}

