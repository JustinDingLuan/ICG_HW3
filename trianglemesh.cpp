#include "trianglemesh.h"
using namespace std;
TriangleMesh::TriangleMesh()
{
	numTriangles = 0;
	numVertices = 0;
	vboId = 0;
	objCenter = { 0,0,0 };
	objExtent = { 0,0,0 };
}

TriangleMesh::~TriangleMesh()
{
	vertices.clear();
	subMeshes.clear();
}

int TriangleMesh::getIndex(int n) {
	if (n <= 0) {
		return numVertices + n;
	}
	return n - 1;
}
bool TriangleMesh::LoadMaterial(const std::string filePath, std::string filename) {
	ifstream Materialfile(filePath + "/" + filename);
	string s;
	string filetype = filename.substr(filename.length() - 3, 3);

	if (!Materialfile.is_open() || filetype != "mtl") {
		cout << "cannot find mtl file" << endl;
		return false;
	}
	else {
		cout << "Successfully Open Material File!" << endl;
	}

	while (std::getline(Materialfile, s)) {
		string str;
		stringstream data;
		data << s;
		data >> str;
		if (str == "newmtl") {
			string materialname;
			data >> materialname;
			materials.push_back(PhongMaterial());
			materials.back().SetName(materialname);
		}
		else if (str == "Ns") {
			float ns = 0.0f;
			data >> ns;
			materials.back().SetNs(ns);
		}
		else if (str == "Ka") {
			glm::vec3 ka = { 0,0,0 };
			data >> ka.x >> ka.y >> ka.z;
			materials.back().SetKa(ka);
		}
		else if (str == "Kd") {
			glm::vec3 kd = { 0,0,0 };
			data >> kd.x >> kd.y >> kd.z;
			materials.back().SetKd(kd);
		}
		else if (str == "Ks") {
			glm::vec3 ks = { 0,0,0 };
			data >> ks.x >> ks.y >> ks.z;
			materials.back().SetKs(ks);
		}
		else if (str == "map_Kd") {
			string path;
			data >> path;			
			string txtpath = filePath + "/" + path;			
			ImageTexture* texture = new ImageTexture(txtpath);			
			materials.back().SetMapKd(texture);
			materials.back().SetIsMapKd(1);
		}
	}

	for (int i = 0;i < materials.size();i++) {
		Mapmaterial[materials[i].GetName()] = &(materials[i]);
	}
	
	return true;
}

string TriangleMesh::GetsubFileName(string filePath) {
	int index = static_cast<int>(filePath.rfind("/"));
	string materialsubpath = " ";

	if (index != string::npos) {
		materialsubpath = filePath.substr(0, index);
	}
	return materialsubpath;
}

bool TriangleMesh::LoadFromFile(const std::string filePath, const bool normalized)
{	
	int index = static_cast<int>(filePath.find("."));
	string filename = filePath.substr(0, index);
	string Path = "ICG2022_HW3_Test_Models/" + filename + "/" + filePath + ".obj";
	ifstream OBJfile(Path);
	string s;

	if (!OBJfile.is_open()) {		
		return false;
	}
	else {
		cout << "Successfully Open Object File!" << endl;
	}

	vector<glm::vec3> vtx;
	vector<glm::vec3> nor;
	vector<glm::vec2> txt;

	while (std::getline(OBJfile, s)) {

		string vertex_info;
		stringstream data;
		data << s;
		data >> vertex_info;

		if (vertex_info == "mtllib") {
			string materialfilename;
			data >> materialfilename;			
			LoadMaterial(GetsubFileName(Path), materialfilename);
		}
		else if (vertex_info == "usemtl") {
			string mtlname;
			data >> mtlname;
			subMeshes.push_back(SubMesh());
			subMeshes.back().material = Mapmaterial[mtlname];
		}
		else if (vertex_info == "v") {
			glm::vec3 v = { 0,0,0 };
			data >> v.x >> v.y >> v.z;
			vtx.push_back(v);
		}
		else if (vertex_info == "vn") {
			glm::vec3 n = { 0,0,0 };
			data >> n.x >> n.y >> n.z;
			nor.push_back(n);
		}
		else if (vertex_info == "vt") {
			glm::vec2 t = { 0,0 };
			data >> t.x >> t.y;
			txt.push_back(t);
		}
		else if (vertex_info == "f") {
			vector<string> str;
			string str2;

			while (data >> str2) {
				str.push_back(str2);
			}
			for (int i = 0;i < str.size();i++) {
				for (int j = 0;j < str[i].length();j++) {
					if (str[i][j] == '/') {
						str[i][j] = ' ';
					}
				}
			}

			vector<unsigned int>tempIndices;

			for (int i = 0;i < str.size();i++) {
				stringstream ss;
				ss << str[i];
				vector<int>PTNIndices(3);
				for (int j = 0;j < 3;j++) {
					ss >> PTNIndices[j];
				}

				glm::vec3 temp1 = vtx[getIndex(PTNIndices[0])];
				glm::vec3 temp2 = nor[getIndex(PTNIndices[2])];
				glm::vec2 temp3 = txt[getIndex(PTNIndices[1])];
				vertices.push_back(VertexPTN(temp1, temp2, temp3));
				tempIndices.push_back(static_cast<unsigned int>(vertices.size() - 1));
			}
			for (unsigned int j = 0;j < tempIndices.size() - 2;j++) {
				subMeshes.back().vertexIndices.push_back(tempIndices[0]);
				subMeshes.back().vertexIndices.push_back(tempIndices[j + 1]);
				subMeshes.back().vertexIndices.push_back(tempIndices[j + 2]);
				numTriangles++;
			}
		}
	}

	numVertices = static_cast<int>(vertices.size());
	if (normalized) {
		glm::vec3 maxPos = vertices[0].position;
		glm::vec3 minPos = vertices[0].position;

		for (unsigned int j = 0;j < vertices.size();j++) {
			maxPos = glm::max(maxPos, vertices[j].position);
			minPos = glm::min(minPos, vertices[j].position);
		}

		objCenter = (maxPos + minPos) / 2.0f;
		objExtent = maxPos - minPos;

		for (int i = 0;i < vertices.size();i++) {
			vertices[i].position -= objCenter;
		}
		maxPos -= objCenter;
		minPos -= objCenter;

		objCenter = { 0,0,0 };

		float maxAxis = max(max(objExtent.x, objExtent.y), objExtent.z);

		for (int i = 0;i < vertices.size();i++) {
			vertices[i].position /= maxAxis;
		}

		objExtent /= maxAxis;
	}

	return true;
}

void TriangleMesh::createbuffer() {
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTN) * numVertices, &vertices[0], GL_STATIC_DRAW);

	for (auto& i : subMeshes) {
		glGenBuffers(1, &i.iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i.iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * i.vertexIndices.size(), &i.vertexIndices[0], GL_STATIC_DRAW);
	}

}

void TriangleMesh::draw(SubMesh& submesh) {
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (const GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (const GLvoid*)12);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), (const GLvoid*)24);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, submesh.iboId);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(submesh.vertexIndices.size()), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// Show model information.
void TriangleMesh::ShowInfo()
{
	std::cout << "# Vertices: " << numVertices << std::endl;
	std::cout << "# Triangles: " << numTriangles << std::endl;
	std::cout << "Total " << subMeshes.size() << " subMeshes loaded" << std::endl;
	for (unsigned int i = 0; i < subMeshes.size(); ++i) {
		const SubMesh& g = subMeshes[i];
		std::cout << "SubMesh " << i << " with material: " << g.material->GetName() << std::endl;
		std::cout << "Num. triangles in the subMesh: " << g.vertexIndices.size() / 3 << std::endl;
	}
	std::cout << "Model Center: " << objCenter.x << ", " << objCenter.y << ", " << objCenter.z << std::endl;
	std::cout << "Model Extent: " << objExtent.x << " x " << objExtent.y << " x " << objExtent.z << std::endl;
}

