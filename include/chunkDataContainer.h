#ifndef CHUNK_DATA_CLASS_H
#define CHUNK_DATA_CLASS_H

#include <string>
#include <array>
#include <vector>
#include "VAO.h"

#define NUM_BLOCKS 6
#define RENDER_DISTANCE 24
#define CHUNK_SIZE 16

struct ChunkDataContainer {
	// stores the blockID of each block in the chunk
	std::vector<short> chunkData;
	std::vector<float> lightData = std::vector<float>((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
	VAO array;
    VBO meshBuffer;
    EBO indexBuffer;
	std::vector<unsigned int> mesh;
    std::vector<unsigned int> indices;
	int EBOsize = 0;
	std::array <int, 3> chunkID;
	float distance;
	bool inQueue = 0;
	bool inGeneratorQueue = 0;
	bool emptyChunk = 1;
	bool unCompiledChunk = 1;
	bool unGeneratedChunk = 1;
	bool vaolck = 0;
	bool renderlck = 0;
	bool forUpdate = 0;
};

struct BlockTemplate {
    // vertex format:
    // | 4 bits for texture U | 4 bits for texture V | 8 bits for X | 8 bits for Y | 8 bits for Z |
    // | 28 bits for texture coordinate | 3 bits for normal | 1 bit for ambient occlusion |

    const std::vector<unsigned int> blockBitMap {
        //| U|| V||     X||     Y||     Z|
        0b10001000000010000000100000001000,      0b1010,
        0b00001000000000000000100000001000,      0b1010,
        0b00000000000000000000000000001000,      0b1010,
        0b10000000000010000000000000001000,      0b1010,
        //| U|| V||     X||     Y||     Z|
        0b10001000000000000000100000000000,      0b1000,
        0b00001000000010000000100000000000,      0b1000,
        0b00000000000010000000000000000000,      0b1000,
        0b10000000000000000000000000000000,      0b1000,
        //| U|| V||     X||     Y||     Z|
        0b10001000000010000000100000000000,      0b0110,
        0b00001000000010000000100000001000,      0b0110,
        0b00000000000010000000000000001000,      0b0110,
        0b10000000000010000000000000000000,      0b0110,
        //| U|| V||     X||     Y||     Z|
        0b10001000000000000000100000001000,      0b0100,
        0b00001000000000000000100000000000,      0b0100,
        0b00000000000000000000000000000000,      0b0100,
        0b10000000000000000000000000001000,      0b0100,
        //| U|| V||     X||     Y||     Z|
        0b10001000000010000000100000000000,      0b0010,
        0b00001000000000000000100000000000,      0b0010,
        0b00000000000000000000100000001000,      0b0010,
        0b10000000000010000000100000001000,      0b0010,
        //| U|| V||     X||     Y||     Z|
        0b10001000000010000000000000001000,      0b0000,
        0b00001000000000000000000000001000,      0b0000,
        0b00000000000000000000000000000000,      0b0000,
        0b10000000000010000000000000000000,      0b0000
    };

    const std::vector<int> faceType {
        5,
        4,
        3,
        2,
        1,
        0
    };

    const std::vector<uint> dataEBO {
        0, 1, 2,
    	2, 3, 0,
    };
};

struct Blocks {
    std::string blockName;
	bool isSolid = 1;
    bool castsAO = 1;
};

#endif