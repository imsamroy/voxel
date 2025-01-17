#ifndef CHUNK_GEN_CLASS_H
#define CHUNK_GEN_CLASS_H

#include <math.h>
#include "fastFloat.h"
#include <vector>

#include "FastNoise/FastNoise.h"

#define CHUNK_SIZE 16

class ChunkGen {
private:
	float frequency = 0.005f;
	int seed = 1337;
public:
	std::vector<short> chunk;
	std::vector<bool> bfs;
	std::vector<ushort> light;

	//FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("GQAfACUAAACAP5qZWT8AAIA/AACAPxkAGQAXAAAAgL8AAIA/exQuPwAAgD8TAKRwnUAHAAH//wAAARkAJQA5tMg9rkdhPzm0yD0AAIA/GQAbABkAJQAK16M8AACAPwrXozwAAIA/JwABAAAABwABGwD//wQAAHsUjkAB//8EAAEEAAAAAACamRlAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOxROL8BGQAlAAAAgD8fhQtAAACAPwAAgD8ZAAQAAAAAADMzg0AAAAAAAAAAAAAAAADsUbg+AAAAAAAAAAABDQACAAAAuB4lQAcAAAAAAD8AH4XrPgH//wIAAK5H4T4BGwAXAAAAAMAAAIA/SOF6vwAAgD8lAAAAAD8AAIA/AAAAPwAAgD8UABsA//8PAAAAAIC/AArXIz8AheuRPwAAAEBAAAAAAAAA7FE4vg==");
	FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("HwAUAB8ADQAEAAAA16NQQBMAuB4FPyUAAACAP3E9Sj8AAIA/AACAPxkAGQAXAAAAgL8AAIA/exQuPwAAgD8TAKRwnUAHAAH//wAAARkAJQA5tMg9rkdhPzm0yD0AAIA/GQAbABkAJQAK16M8AACAPwrXozwAAIA/JwABAAAABwABGwD//wQAAHsUjkAB//8EAAEEAAAAAACamRlAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOxROL8AKVyPPgAK1yM9AQ0AAgAAAOF6NEAZACUAAACAP/YonD8AAIA/AACAPxkABAAAAAAAMzODQAAAAAAAAAAAAAAAAOxROD4AAAAAAAAAAAEHAAH//wIAALgeBT8AAAAAAACuR+E+AAAAAAABEwDsUbg+BwAAAAAAAAAAAAAAASUAAACAP1yPgj8AAIA/AACAPxcAAACAvwAAgD/NzEy+ZmamPxMAPQrXPv//GgAAAACAPg==");
	//FastNoise::SmartNode<> fnGenerator2 = FastNoise::NewFromEncodedNodeTree("FAAfABQAHwANAAQAAADXo1BAEwC4HgU/JQAAAIA/cT1KPwAAgD8AAIA/GQAZABcAAACAvwAAgD97FC4/AACAPxMApHCdQAcAAf//AAABGQAlADm0yD2uR2E/ObTIPQAAgD8ZABsAGQAlAArXozwAAIA/CtejPAAAgD8nAAEAAAAHAAEbAP//BAAAexSOQAH//wQAAQQAAAAAAJqZGUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA7FE4vwApXI8+AArXIz0BDQACAAAA4Xo0QBkAJQAAAIA/9iicPwAAgD8AAIA/GQAEAAAAAAAzM4NAAAAAAAAAAAAAAAAA7FE4PgAAAAAAAAAAAQcAAf//AgAAuB4FPwAAAAAAAK5H4T4AAAAAAAETAOxRuD4HAAAAAAAAAAAAAAABJQAAAIA/XI+CPwAAgD8AAIA/FwAAAIC/AACAP83MTL5mZqY/EwA9Ctc+//8aAAAAAIA+AAAAAAABFwCkcD0/rkdhP65HYT5cj8I+EwCuR+E+GgABEQACAAAAAADgQBAAAACIQR8AFgABAAAACwADAAAAAgAAAAMAAAAEAAAAAAAAAD8BFAD//yAAAAAAAD8AAAAAPwAAAAA/AAAAAD8BFwAAAIC/AACAPz0KF0BSuB5AEwAAAKBABgAAj8J1PACamZk+AAAAAAAA4XoUPwAAAAAAAAAAAAA=");
	//FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree(/*"DQACAAAAhesRQAcAAKRwvT8ACtcjPg=="*//*"DQAFAAAAAAAAQAgAAAAAAD8AAAAAAA=="*/"HgAfABMA9iicPw0AAwAAAOF61D8HAACF61E/AFyPwr4BBAAAAAAAheuRvwAAAAAAAAAAAAAAAD0K174AAAAAAAAAAACamdk/ASEAGQAlAAAAgD9cj8I+AACAPwAAgD8NAAIAAAB7FO4/BwAAAAAAPwAAAAAAARkABwABBAAAAAAAAACQQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACUAAACAP83MzD0AAIA/AACAPw0AAwAAAAAAAEAWAAcAAAAHAAAAAAA/AAAAAAABHgATAOxROD7//wwAARMAzcy8wP//DAA=");
	FastNoise::SmartNode<> fnGenerator2 = FastNoise::NewFromEncodedNodeTree("DQACAAAAhesRQAcAAKRwvT8ACtcjPg==");
	FastNoise::SmartNode<> fnGenerator3 = FastNoise::NewFromEncodedNodeTree("DQACAAAAhesRQAcAAKRwvT8ACtcjPg==");

	ChunkGen();

	void generateChunk(std::vector<short> &chunk, int coordX, int coordY, int coordZ);
	void initChunk(std::vector<short> &chunk);

	int returnBlock(int coordX, int coordY, int coordZ);
};

#endif
