#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "headers.h"

// ShaderProg Declarations.
class ShaderProg
{
public:
	// ShaderProg Public Methods.
	ShaderProg();
	~ShaderProg();

	bool LoadFromFiles(const std::string vsFilePath, const std::string fsFilePath);
	void Bind() { glUseProgram(shaderProgId); };
	void UnBind() { glUseProgram(0); };

	GLint GetLocMVP() const { return locMVP; }

protected:
	// ShaderProg Protected Methods.
	virtual void GetUniformVariableLocation();

	// ShaderProg Protected Data.
	GLuint shaderProgId;

private:
	// ShaderProg Private Methods.
	GLuint AddShader(const std::string& sourceText, GLenum shaderType);
	static bool LoadShaderTextFromFile(const std::string filePath, std::string& sourceText);

	// ShaderProg Private Data.
	GLint locMVP;
};

// ------------------------------------------------------------------------------------------------

// FillColorShaderProg Declarations.
class FillColorShaderProg : public ShaderProg
{
public:
	// FillColorShaderProg Public Methods.
	FillColorShaderProg();
	~FillColorShaderProg();

	GLint GetLocFillColor() const { return locFillColor; }

protected:
	// FillColorShaderProg Protected Methods.
	void GetUniformVariableLocation();

private:
	// FillColorShaderProg Private Data.
	GLint locFillColor;
};

// ------------------------------------------------------------------------------------------------

// PhongShadingDemoShaderProg Declarations.
class PhongShadingDemoShaderProg : public ShaderProg
{
public:
	// PhongShadingDemoShaderProg Public Methods.
	PhongShadingDemoShaderProg();
	~PhongShadingDemoShaderProg();

	GLint GetLocM() const { return locM; }
	GLint GetLocNM() const { return locNM; }
	GLint GetLocCameraPos() const { return locCameraPos; }

	GLint GetLocKa() const { return locKa; }
	GLint GetLocKd() const { return locKd; }
	GLint GetLocKs() const { return locKs; }
	GLint GetLocNs() const { return locNs; }

	GLint GetLocAmbientLight() const { return locAmbientLight; }

	GLint GetLocDirLightDir() const { return locDirLightDir; }
	GLint GetLocDirLightRadiance() const { return locDirLightRadiance; }

	GLint GetLocPointLightPos() const { return locPointLightPos; }
	GLint GetLocPointLightIntensity() const { return locPointLightIntensity; }

	GLint GetLocSpotLightPos() const { return locSpotLightPos; }
	GLint GetLocSpotLightDir() const { return locSpotLightDir; }
	GLint GetLocFalloffStart()  const { return locFallOffStart; }
	GLint GetLocTotalWidth()   const { return locSpotLightTotalWidth; }
	GLint GetLocSpotLightIntensity()  const { return locSpotLightIntensity; }

	GLint GetLocMapKd() const { return locMapKd; }
	GLint IsMapKd() { return ismapkd; }
	

protected:
	// PhongShadingDemoShaderProg Protected Methods.
	void GetUniformVariableLocation();

private:
	// PhongShadingDemoShaderProg Public Data.
	// Transformation matrix.
	GLint locM;
	GLint locNM;
	GLint locCameraPos;
	// Material properties.
	GLint locKa;
	GLint locKd;
	GLint locKs;
	GLint locNs;
	// Light data.
	GLint locAmbientLight;
	GLint locDirLightDir;
	GLint locDirLightRadiance;
	GLint locPointLightPos;
	GLint locPointLightIntensity;
	GLint locSpotLightPos;
	GLint locSpotLightDir;
	GLint locSpotLightIntensity;
	GLint locSpotLightTotalWidth;
	GLint locFallOffStart;
	// Texture data.
	GLint locMapKd;
	GLint ismapkd;	

};

// ------------------------------------------------------------------------------------------------

// SkyboxShaderProg Declarations.
class SkyboxShaderProg : public ShaderProg
{
public:
	// SkyboxShaderProg Public Methods.
	SkyboxShaderProg();
	~SkyboxShaderProg();

	GLint GetLocMapKd() const { return locMapKd; }

protected:
	// PhongShadingDemoShaderProg Protected Methods.
	void GetUniformVariableLocation();

private:
	// SkyboxShaderProg Public Data.
	GLint locMapKd;
};

#endif