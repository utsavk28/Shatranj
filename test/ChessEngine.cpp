#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "ChessEngine.h"

bool createChessEngine() {
	ChessEngine chessEngine = ChessEngine();
	return true;
}

TEST_CASE("Chess Engine : Creation", "[ChessEngineCreation]") {

	REQUIRE(createChessEngine() == true);
}

