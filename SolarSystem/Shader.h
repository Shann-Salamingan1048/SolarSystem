#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <print>
#include <unordered_map>
class Shader
{
public:

	Shader(const char* vertPath, const char* fragPath);
	void use();
	unsigned int getID();
	void deleteShader();

public:
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

public:
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	unsigned int m_ID;

private:
	mutable std::unordered_map<std::string, GLuint> m_uniformCache;
	GLint getUniformLocation(const std::string& name) const;
private:
	std::string readShaderFile(const char* filePath);
	GLuint createShaderProgram(GLuint vertShader, GLuint fragShader);
	GLuint compileShader(const char* source, GLenum type, const char* typeName);
};