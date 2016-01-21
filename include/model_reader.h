#ifndef MODEL_READER
#define MODEL_READER

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct aiScene * get_aiScene(char * path);
 
#endif
