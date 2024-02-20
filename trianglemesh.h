#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "headers.h"
#include "material.h"

struct VertexPTN
{
	VertexPTN() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
		texcoord = glm::vec2(0.0f, 0.0f);
	}
	VertexPTN(glm::vec3 p, glm::vec3 n, glm::vec2 uv) {
		position = p;
		normal = n;
		texcoord = uv;
	}
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

// SubMesh Declarations.
struct SubMesh
{
	SubMesh() {
		material = nullptr;
		iboId = 0;
	}
	PhongMaterial* material;
	GLuint iboId;
	std::vector<unsigned int> vertexIndices;
};


// TriangleMesh Declarations.
class TriangleMesh
{
public:
	// TriangleMesh Public Methods.
	TriangleMesh();
	~TriangleMesh();
	
	// Load the model from an *.OBJ file.
	bool LoadFromFile(const std::string filePath, const bool normalized = true);
	bool LoadMaterial(const std::string filePath, std::string filename);
	std::string GetsubFileName(std::string filePath);
	
	// Show model information.
	void ShowInfo();
	
	int getIndex(int n);
	int GetNumVertices() const { return numVertices; }
	int GetNumTriangles() const { return numTriangles; }
	int GetNumSubMeshes() const { return static_cast<int>(subMeshes.size()); }
	void createbuffer();
	std::vector<SubMesh>& Getsubmesh() { return subMeshes; }
	glm::vec3 GetObjCenter() const { return objCenter; }
	glm::vec3 GetObjExtent() const { return objExtent; }
	void draw(SubMesh& submesh);

private:
	GLuint vboId;
	
	std::vector<VertexPTN> vertices;	
	std::vector<SubMesh> subMeshes;

	std::vector<PhongMaterial>materials;
	std::unordered_map<std::string , PhongMaterial*> Mapmaterial;	

	int numVertices;
	int numTriangles;
	glm::vec3 objCenter;
	glm::vec3 objExtent;
};


#endif
