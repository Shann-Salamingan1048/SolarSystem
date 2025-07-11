#include "Shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
	try
	{
		std::string vShaderCode = readShaderFile(vertPath);
		std::string fShaderCode = readShaderFile(fragPath);

		GLuint vertShader = compileShader(vShaderCode.c_str(), GL_VERTEX_SHADER, "VERTEX");
		GLuint fragShader = compileShader(fShaderCode.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");

		m_ID = createShaderProgram(vertShader, fragShader);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}
	catch (const std::exception& e) 
	{
		// Clean up on failure
		if (m_ID != 0) {
			glDeleteProgram(m_ID);
			m_ID = 0;
		}
		std::println("ERROR::SHADER::CONSTRUCTION_FAILED: {}", e.what());
		throw; // Re-throw to let caller handle
	}

}
GLuint Shader::compileShader(const char* source, GLenum type, const char* typeName)
{
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		throw std::runtime_error(std::format("Failed to create {} shader", typeName));
	}

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetShaderInfoLog(shader, logLength, nullptr, log.data());

		glDeleteShader(shader);
		throw std::runtime_error(std::format("{} shader compilation failed: {}",
			typeName, log));
	}

	return shader;
}
GLuint Shader::createShaderProgram(GLuint vertShader, GLuint fragShader)
{
	GLuint program = glCreateProgram();
	if (program == 0) {
		throw std::runtime_error("Failed to create shader program");
	}
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, '\0');
		glGetProgramInfoLog(program, logLength, nullptr, log.data());

		glDeleteProgram(program);
		throw std::runtime_error(std::format("Shader program linking failed: {}", log));
	}

	return program;
	
}
std::string Shader::readShaderFile(const char* filePath)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(filePath);
		std::stringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}
	catch (const std::ifstream::failure& e)
	{
		throw std::runtime_error(std::format("Failed to read shader file '{}': {}",
			filePath, e.what()));
	}
}
GLint Shader::getUniformLocation(const std::string& name) const
{
	if (m_uniformCache.contains(name))
		return m_uniformCache[name];

	GLint location = glGetUniformLocation(m_ID, name.c_str());
	m_uniformCache[name] = location;
	return location;
}
void Shader::deleteShader()
{
	glDeleteProgram(m_ID);
}
void Shader::use()
{
	glUseProgram(m_ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(getUniformLocation(name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name.c_str()), value);
}
// 1 means im uploading one vec2, vec3, vec4 uniforrm (not an array of vecs)
/*
	&value[0] gives you a pointer to the first float in the vec2.
	OpenGL expects a pointer to raw float data (const GLfloat*), and 
	&value[0] gives it exactly that.

	So &value[0] is a pointer to an array like: { value.x, value.y }
*/
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(getUniformLocation(name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(getUniformLocation(name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(getUniformLocation(name.c_str()), 1, &value[0]);
}
/// ordinary floats
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(getUniformLocation(name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(getUniformLocation(name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(getUniformLocation(name.c_str()), x, y,z,w);
}
///
// Matrix
// 1 means im uploading one mat2, mat3, mat4 uniforrm (not an array of mats)
/*
* 
* GL_FALSE = Don't transpose → Use the matrix as is (column-major layout).
* GL_TRUE = Transpose it (interprets your data as row-major).
	&mat[0][0] gives you a pointer to the first float in the vec2.
	OpenGL expects a pointer to raw float data (const GLfloat*), and
	&mat[0][0] gives it exactly that.

	So &mat[0][0] is a  
	Layout in memory (column-major):
	mat[0][0]  mat[0][1]   // first column (x values)
	mat[1][0]  mat[1][1]   // second column (y values)
*/
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
//////
unsigned int Shader::getID()
{
	return m_ID;
}
