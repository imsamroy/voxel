#include "../include/chunkList.h"
#include "chunkDataContainer.h"
#include "chunkGenerator.h"
#include "fastFloat.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <array>
#include <chrono>
#include <ratio>
#include <string>
#include <thread>

void ChunkList::chunkManager() {
    if (int(camPosX) % CHUNK_SIZE == 0 || int(camPosY) % CHUNK_SIZE == 0 || int(camPosZ) % CHUNK_SIZE == 0) {
        organiselck = 1;
        assignChunkID();
        organiselck = 0;
        putInVAOs();
    }
}

void ChunkList::calculateLoadedChunks() {
    frustumOffset = (double(CHUNK_SIZE) / 2) / tan(FOV / 2);
    cosineModifiedHalfFOV = cos(atan(screenDiag / (screenHeight / tan(FOV / 2))));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (run == 1) {
    	cosineModifiedHalfFOV = cos(atan(screenDiag / (screenHeight / tan(FOV / 2))));
        for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
            // float distance = sqrt(pow((chunkWorldContainer[i].chunkID[0] * CHUNK_SIZE) - camPosX + (CHUNK_SIZE / 2), 2) + pow((chunkWorldContainer[i].chunkID[1] * CHUNK_SIZE) - camPosY + (CHUNK_SIZE / 2), 2) + pow((chunkWorldContainer[i].chunkID[2] * CHUNK_SIZE) - camPosZ + (CHUNK_SIZE / 2), 2));
            float distance = abs((chunkWorldContainer[i].chunkID.x * CHUNK_SIZE) - camPosX + (CHUNK_SIZE / 2)) + abs(((chunkWorldContainer[i].chunkID.y * CHUNK_SIZE) - camPosY + (CHUNK_SIZE / 2))) + abs((chunkWorldContainer[i].chunkID.z * CHUNK_SIZE) - camPosZ + (CHUNK_SIZE / 2));
            chunkWorldContainer[i].distance = distance;
            chunkWorldContainer.at(i).frustumVisible = isFrustumCulled(chunkWorldContainer.at(i).chunkID);
        }
    }
}

void ChunkList::assignChunkID() {
    while (run == 1) {
    	ChunkCoords cameraChunk;
        cameraChunk.x = int(floor(camPosX / CHUNK_SIZE)) - (RENDER_DISTANCE / 2);
        cameraChunk.y = int(floor(camPosY / CHUNK_SIZE)) - (RENDER_DISTANCE / 2);
        cameraChunk.z = int(floor(camPosZ / CHUNK_SIZE)) - (RENDER_DISTANCE / 2);
        if (firstRun == 0) {
            int offsetX = fastFloat::fastFloor(camPosX / CHUNK_SIZE) - (RENDER_DISTANCE / 2);
            int offsetY = fastFloat::fastFloor(camPosY / CHUNK_SIZE) - (RENDER_DISTANCE / 2);
            int offsetZ = fastFloat::fastFloor(camPosZ / CHUNK_SIZE) - (RENDER_DISTANCE / 2);
            for (int i = 0; i < RENDER_DISTANCE; i++) {
                for (int j = 0; j < RENDER_DISTANCE; j++) {
                    for (int k = 0; k < RENDER_DISTANCE; k++) {
                        loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][0] = i + offsetX;
                        loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][1] = j + offsetY;
                        loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][2] = k + offsetZ;
                        loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][3] = 0;
                    }
                }
            }
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                chunkWorldContainer[i].chunkID.x = loadedChunkCoord[i][0];
                chunkWorldContainer[i].chunkID.y = loadedChunkCoord[i][1];
                chunkWorldContainer[i].chunkID.z = loadedChunkCoord[i][2];
                coordToIndexMap[coordsToKey(chunkWorldContainer.at(i).chunkID)] = i;
                loadedChunkCoord[i][4] = i;
                chunkWorldContainer[i].emptyChunk = 0;
                chunkWorldContainer[i].unGeneratedChunk = 1;
                chunkWorldContainer[i].unCompiledChunk = 1;
                chunkWorldContainer[i].renderlck = 1;
            }
            firstRun = 1;
        } else {
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                std::array<int, 3> axisDistances {
                    chunkWorldContainer[i].chunkID.x - cameraChunk.x,
                    chunkWorldContainer[i].chunkID.y - cameraChunk.y,
                    chunkWorldContainer[i].chunkID.z - cameraChunk.z,
                };
                ChunkCoords newChunkID;
                bool x = false;
                if (!(axisDistances.at(0) < RENDER_DISTANCE && axisDistances.at(0) >= 0)) {
                    int m = fastFloat::mod(axisDistances.at(0), RENDER_DISTANCE);
                    newChunkID.x = cameraChunk.x + m;
                    x = true;
                } else {
                    newChunkID.x = chunkWorldContainer[i].chunkID.x;
                }
                if (!(axisDistances.at(1) < RENDER_DISTANCE && axisDistances.at(1) >= 0)) {
                    int m = fastFloat::mod(axisDistances.at(1), RENDER_DISTANCE);
                    newChunkID.y = cameraChunk.y + m;
                    x = true;
                } else {
                    newChunkID.y = chunkWorldContainer[i].chunkID.y;
                }
                if (!(axisDistances.at(2) < RENDER_DISTANCE && axisDistances.at(2) >= 0)) {
                    int m = fastFloat::mod(axisDistances.at(2), RENDER_DISTANCE);
                    newChunkID.z = cameraChunk.z + m;
                    x = true;
                } else {
                    newChunkID.z = chunkWorldContainer[i].chunkID.z;
                }
                if (x) {
                    coordToIndexMap.erase(coordsToKey(chunkWorldContainer.at(i).chunkID));

                    chunkWorldContainer[i].chunkID = newChunkID;

                    coordToIndexMap[coordsToKey(chunkWorldContainer.at(i).chunkID)] = i;

                    chunkWorldContainer[i].unGeneratedChunk = 1;
                    
                    chunkWorldContainer.at(i).unCompiledChunk = 1;
                    chunkWorldContainer[i].EBOsize = 0;
                    chunkWorldContainer[i].chunkData.clear();
                    chunkWorldContainer[i].renderlck = 1;
                    chunkWorldContainer[i].emptyChunk = 0;
                    chunkWorldContainer.at(i).inQueue = 0;
                    chunkWorldContainer.at(i).isPermeableCheckDone = false;
                    chunkWorldContainer.at(i).occlusionUnCulled = false;
                }
            }   
        }
        cameraChunk.x = int(floor(camPosX / CHUNK_SIZE));
        cameraChunk.y = int(floor(camPosY / CHUNK_SIZE));
        cameraChunk.z = int(floor(camPosZ / CHUNK_SIZE));


        // const auto start = std::chrono::high_resolution_clock::now();
        BFSqueue.empty();
        
        doBFS(cameraChunk);

        for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
            chunkWorldContainer.at(i).inBFSqueue = 0;
            chunkWorldContainer.at(i).occlusionUnCulled = localOcclusionUnCulled.at(i);
            localOcclusionUnCulled.at(i) = false;
        }
        // const auto end = std::chrono::high_resolution_clock::now();
 
        // const std::chrono::duration<double> diff = end - start;
        // std::cout << "BFS search: " << diff << "\n";

        crntPerm = chunkWorldContainer.at(getIndex(cameraChunk)).permeability;


        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

bool ChunkList::isEdgeChunk(int coordX, int coordY, int coordZ) {
    if (coordX - fastFloat::fastFloor(camPosX / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosX / CHUNK_SIZE) - coordX == RENDER_DISTANCE / 2 || coordY - fastFloat::fastFloor(camPosY / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosY / CHUNK_SIZE) - coordY == RENDER_DISTANCE / 2 || coordZ - fastFloat::fastFloor(camPosZ / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosZ / CHUNK_SIZE) - coordZ == RENDER_DISTANCE / 2) {
        return 1;
    }
    return 0;
}

void ChunkList::organiseChunks(int threadID) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    while (run == 1) {
        if (organiselck == 0) {
            // int leastIndex = 0;
            // do {
            //     for (int i = 0; i < chunkWorldContainer.size(); i++) {
            //         if (chunkWorldContainer[i].distance <= chunkWorldContainer[leastIndex].distance && (chunkWorldContainer[i].unCompiledChunk == 1 || chunkWorldContainer[i].forUpdate == 1) && chunkWorldContainer[i].inQueue == 0 && isEdgeChunk(chunkWorldContainer[i].chunkID[0], chunkWorldContainer[i].chunkID[1], chunkWorldContainer[i].chunkID[2]) != 1) {
            //             leastIndex = i;
            //         }
            //     }
            // } while (chunkWorldContainer[leastIndex].inQueue == 1);
            // chunkWorldContainer[leastIndex].inQueue = 1;
            // index[threadID] = leastIndex;

            //std::cout << chunkWorldContainer[index[threadID]].unCompiledChunk << " ThreadID: " << threadID << " || Index: " << index[threadID] << " || X Y Z: " << chunkWorldContainer[index[threadID]].chunkID[0] << " " << chunkWorldContainer[index[threadID]].chunkID[1] << " " << chunkWorldContainer[index[threadID]].chunkID[2] << " || Distance: " << chunkWorldContainer[index[threadID]].distance << "\n";
            
            bool iLetYouRun = false;
            
            if (chunkUpdateQueue.size() > 0) {
            	index[threadID] = chunkUpdateQueue.front();
                if (chunkWorldContainer.at(index[threadID]).unGeneratedChunk == false)
                    iLetYouRun = true;
                chunkUpdateQueue.pop();
            } else if (chunkMeshingQueue.size() > 0) {
                index[threadID] = chunkMeshingQueue.front();
                if (chunkWorldContainer.at(index[threadID]).unCompiledChunk == true && chunkWorldContainer.at(index[threadID]).unGeneratedChunk == false)
                    iLetYouRun = true;
                chunkMeshingQueue.pop();
                chunkWorldContainer.at(index[threadID]).inQueue = false;
            }
            
            if (iLetYouRun) {
                chunkX[threadID] = chunkWorldContainer[index[threadID]].chunkID.x;
                chunkY[threadID] = chunkWorldContainer[index[threadID]].chunkID.y;
                chunkZ[threadID] = chunkWorldContainer[index[threadID]].chunkID.z;
                std::unique_lock<std::mutex> lock(chunkWorldContainerMutex);
                if ((chunkWorldContainer[index[threadID]].unCompiledChunk == 1 || chunkWorldContainer[index[threadID]].forUpdate == 1) && chunkWorldContainer[index[threadID]].unGeneratedChunk == 0) {
                    chunkWorldContainer[index[threadID]].vaolck = 1;
                    updateLight(chunkWorldContainer[index[threadID]].chunkID, threadID);
                    // const auto start = std::chrono::high_resolution_clock::now();

                    chunkWorldContainer[index[threadID]].EBOsize = buildChunk(threadID);

	                // const auto end = std::chrono::high_resolution_clock::now();
	 
	                // const std::chrono::duration<double> diff = end - start;
	                // std::cout << "Generating a chunk: " << diff << "\n";
                    chunkWorldContainer[index[threadID]].vaolck = 0;
                } else {
                    chunkWorldContainer[index[threadID]].vaolck = 1;
                }
                lock.unlock();
            }
        }
    }
}

void ChunkList::generateChunks() {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    while (run == 1) {
        if (organiselck == 0) {
            int index = 0;
            for (int i = 0; i < chunkWorldContainer.size(); i++) {
                if (chunkWorldContainer[i].distance <= chunkWorldContainer[index].distance && chunkWorldContainer[i].unGeneratedChunk == 1) {
                    index = i;
                }
            }

			// bool iLetYouRun = false;
            
            // if (chunkGeneratingQueue.size() > 0) {
            //     index = chunkGeneratingQueue.front();
            //     //if (chunkWorldContainer.at(index).inGeneratorQueue) {
            //         iLetYouRun = true;
            //     //}
            //     chunkGeneratingQueue.pop();
            //     chunkWorldContainer.at(index).inGeneratorQueue = 0;
            // }


            //std::cout << chunkWorldContainer[index].unGeneratedChunk <<  " Gen || Index: " << index << " || X Y Z: " << chunkWorldContainer[index].chunkID[0] << " " << chunkWorldContainer[index].chunkID[1] << " " << chunkWorldContainer[index].chunkID[2] << " || Distance: " << chunkWorldContainer[index].distance << "\n";
            if (chunkWorldContainer[index].unGeneratedChunk == 1) {

                generator.initChunk(chunkWorldContainer[index].chunkData);
                generator.generateChunk(chunkWorldContainer[index].chunkData, chunkWorldContainer[index].chunkID.x, chunkWorldContainer[index].chunkID.y, chunkWorldContainer[index].chunkID.z);
                
                // const auto start = std::chrono::high_resolution_clock::now();

                // checkPermeability(chunkWorldContainer.at(index));

                // const auto end = std::chrono::high_resolution_clock::now();
 
                // const std::chrono::duration<double> diff = end - start;
                // std::cout << "Generating a chunk: " << diff << "\n";

                chunkWorldContainer[index].unGeneratedChunk = 0;
                chunkWorldContainer[index].unCompiledChunk = 1;

            }
        }
    }
}

BlockCoords::BlockCoords(int index) {
    x = index / (CHUNK_SIZE * CHUNK_SIZE);
    y = (index / CHUNK_SIZE) % CHUNK_SIZE;
    z = index % CHUNK_SIZE;
}

BlockCoords::BlockCoords(int _x, int _y, int _z) {
    x = _x;
    y = _y;
    z = _z;
}

void ChunkList::putInVAOs() {
    for (int i = 0; i < chunkWorldContainer.size(); i++) {
        if (chunkWorldContainer[i].unCompiledChunk == 0 && chunkWorldContainer[i].EBOsize != 0 && chunkWorldContainer[i].vaolck == 0) {
            uploadLight(i);
            chunkWorldContainer[i].array.Bind();

            chunkWorldContainer[i].meshBuffer.Bind();
            chunkWorldContainer[i].indexBuffer.Bind();

            chunkWorldContainer[i].meshBuffer.Gen(chunkWorldContainer[i].mesh);
            chunkWorldContainer[i].indexBuffer.Gen(chunkWorldContainer[i].indices);

            chunkWorldContainer[i].array.LinkAttribIPointer(chunkWorldContainer[i].meshBuffer, 0, 1, GL_INT, 2 * sizeof(int), (void*)0);
		    chunkWorldContainer[i].array.LinkAttribIPointer(chunkWorldContainer[i].meshBuffer, 1, 1, GL_INT, 2 * sizeof(int), (void*)(1 * sizeof(int)));

            chunkWorldContainer[i].array.Unbind();
            chunkWorldContainer[i].meshBuffer.Unbind();
            chunkWorldContainer[i].indexBuffer.Unbind();

            chunkWorldContainer[i].renderlck = 0;
            chunkWorldContainer[i].vaolck = 1;
        }
    }
}

bool ChunkList::alreadyIn(std::vector <int> queue, int element) {
    for (int i = 0; i < queue.size(); i++) {
        if (queue.at(i) == element) {
            return 0;
            break;
        }
    }
    return 1;
}

void ChunkList::updateChunk(ChunkCoords chunkCoords, bool surroundings) {
	int index = getIndex(chunkCoords);
	if (chunkWorldContainer.at(index).forUpdate == false) {
		// checkPermeability(chunkWorldContainer.at(index));
		chunkWorldContainer.at(index).forUpdate = true;
		chunkUpdateQueue.push(index);
	}
    if (surroundings) {
    	std::array<unsigned int, 6> neighbouringChunkIndices;

	    neighbouringChunkIndices.at(0) = getIndex(chunkCoords.x + 1, chunkCoords.y, chunkCoords.z);

	    neighbouringChunkIndices.at(1) = getIndex(chunkCoords.x - 1, chunkCoords.y, chunkCoords.z);

	    neighbouringChunkIndices.at(4) = getIndex(chunkCoords.x, chunkCoords.y, chunkCoords.z + 1);

	    neighbouringChunkIndices.at(5) = getIndex(chunkCoords.x, chunkCoords.y, chunkCoords.z - 1);

	    neighbouringChunkIndices.at(2) = getIndex(chunkCoords.x, chunkCoords.y + 1, chunkCoords.z);

	    neighbouringChunkIndices.at(3) = getIndex(chunkCoords.x, chunkCoords.y - 1, chunkCoords.z);

	    for (int chunkIndex : neighbouringChunkIndices) {
	    	if (chunkIndex != -1) {
	    		if (chunkWorldContainer.at(chunkIndex).forUpdate == false) {
		    		chunkUpdateQueue.push(chunkIndex);
		    		chunkWorldContainer.at(chunkIndex).forUpdate = true;
	    		}
	    	}
	    }
    }
}