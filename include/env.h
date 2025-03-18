#pragma once

#include <array>

constexpr uint8_t BOARD_LENGTH = 6;
constexpr uint8_t BOARD_SIZE = 36;
constexpr uint8_t TARGET = 16;
constexpr uint8_t MAX_PIECE_SIZE = 3;
constexpr uint8_t MIN_PIECE_SIZE = 2;

typedef uint64_t Bitmap;

enum class Dir {
	HORZ = 1,
	VERT = 6,
};

const std::array<Bitmap, BOARD_LENGTH> RowMasks = []() {
	std::array<Bitmap, BOARD_LENGTH> rowMasks;
	for (int y = 0; y < BOARD_LENGTH; ++y) {
		Bitmap mask{ 0 };
		for (int x = 0; x < BOARD_LENGTH; ++x) {
			const int i = y * BOARD_LENGTH + x;
			mask |= (Bitmap)1 << i;
		}
		rowMasks[y] = mask;
	}
	return rowMasks;
}();

const std::array<Bitmap, BOARD_LENGTH> ColMasks = []() {
	std::array<Bitmap, BOARD_LENGTH> ColMasks;
	for (int x = 0; x < BOARD_LENGTH; ++x) {
		Bitmap mask{ 0 };
		for (int y = 0; y < BOARD_LENGTH; ++y) {
			const int i = y * BOARD_LENGTH + x;
			mask |= (Bitmap)1 << i;
		}
		ColMasks[y] = mask;
	}
	return ColMasks;
}();

consteval Bitmap TopRow = RowMasks.front();
consteval Bitmap BotRow = RowMasks.back();

consteval Bitmap LeftCol = ColMasks.front();
consteval Bitmap RightCol = ColMasks.front();
