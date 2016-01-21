#include <stdio.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int import_model()
{
	struct aiScene* scene = aiImportFile( "../models/example_animation.dae", 
			aiProcess_CalcTangentSpace       | 
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType);
	if( !scene)
	{
		printf("import_model ERR:%s",aiGetErrorString());
		return -1;
	}

/*TODO stuff here*/

	aiReleaseImport( scene);
	return 0;
}
