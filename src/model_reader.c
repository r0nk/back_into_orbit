#include <stdio.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct aiScene * get_aiScene(char * path)
{
	struct aiScene* scene = aiImportFile(path, 
			aiProcess_CalcTangentSpace       | 
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType);
	if( !scene)
	{
		printf("import_model ERR:%s\n",aiGetErrorString());
		return NULL;
	}
	return scene;
}



