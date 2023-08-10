#pragma once

enum ChessPieceType {
	WhitePawnType = 'P',
	WhiteRookType = 'R',
	WhiteKnightType = 'N',
	WhiteBishopType = 'B',
	WhiteQueenType = 'Q',
	WhiteKingType = 'K',

	BlackPawnType = 'p',
	BlackRookType = 'r',
	BlackKnightType = 'n',
	BlackBishopType = 'b',
	BlackQueenType = 'q',
	BlackKingType = 'k',

	EmptyType = '_'
};

enum ChessPieceMoveType {
	Kill = 'k',
	Castle = 'c',
	Move = 'm',
	Pawn2Step = 'p',
	EnPassant = 'e',

	Empty = '_'
};