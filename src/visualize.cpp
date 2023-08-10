#include "visualize.h"
#include "enum.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <map>

int visualize::visualizeChessBoard(ChessBoard* chessboard)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL initialization failed: %s", SDL_GetError());
		return -1;
	}

	// Create a window
	SDL_Window* window = SDL_CreateWindow("SDL Starter Code", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	// Create a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_Surface* tempSurface = IMG_Load("res/board_empty.png");
	SDL_Texture* chessboardTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = IMG_Load("res/pieces_white_1.png");
	SDL_Texture* whitePieceTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = IMG_Load("res/pieces_black_1.png");
	SDL_Texture* blackPieceTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	std::map<ChessPieceType, SDL_Rect> srcMp;
	std::vector<std::vector<SDL_Rect>> destMp(8,std::vector<SDL_Rect>(8));
	std::vector<ChessPieceType> whitePieces{WhiteRookType, WhiteKnightType, WhiteBishopType, WhiteQueenType, WhiteKingType, WhitePawnType};
	std::vector<ChessPieceType> blackPieces{BlackRookType, BlackKnightType, BlackBishopType, BlackQueenType, BlackKingType, BlackPawnType};

	SDL_Rect chessboard_rect;
	chessboard_rect.x = chessboard_rect.y = 0;
	chessboard_rect.w = chessboard_rect.h = 128;
	SDL_Rect chessboard_rect_dest = SDL_Rect(chessboard_rect);
	chessboard_rect_dest.x = 220;
	chessboard_rect_dest.y = 40;
	chessboard_rect_dest.h = 5 * chessboard_rect.h;
	chessboard_rect_dest.w = 5 * chessboard_rect.w;


	for (int i = 0; i < 6; i++) {
		SDL_Rect temp;
		temp.x = 15 * i;
		temp.y = 0;
		temp.w = temp.h = 15;
		srcMp[whitePieces[i]] = srcMp[blackPieces[i]] = temp;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (chessboard->chessboard[i][j] != NULL) {
				ChessPieceType chessPieceType = chessboard->chessboard[i][j]->getType();
				SDL_Rect temp;
				temp.w = 5 * srcMp[chessPieceType].w;
				temp.h = 5 * srcMp[chessPieceType].h;
				temp.x = chessboard_rect_dest.x + 5*4 + j * temp.w;
				temp.y = chessboard_rect_dest.y + 5*4 + i * temp.h;
				destMp[i][j] = temp;
			}
		}
	}

	// Main loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		// Event handling
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Clear the screen (black in this case)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Add your rendering code here
		// ...
		SDL_RenderCopy(renderer, chessboardTexture, &chessboard_rect, &chessboard_rect_dest);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (chessboard->chessboard[i][j] != NULL) {
					bool isWhite = chessboard->chessboard[i][j]->getIsWhite();
					ChessPieceType chessPieceType = chessboard->chessboard[i][j]->getType();
					/*std::cout << destMp[chessPieceType].x << " ";
					std::cout << destMp[chessPieceType].y << " ";*/
					if (isWhite)
						SDL_RenderCopy(renderer, whitePieceTexture, &srcMp[chessPieceType], &destMp[i][j]);
					else
						SDL_RenderCopy(renderer, blackPieceTexture, &srcMp[chessPieceType], &destMp[i][j]);
					//std::cout << std::endl;
				}
			}
		}
		// Update the screen
		SDL_RenderPresent(renderer);
	}

	// Clean up and exit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
