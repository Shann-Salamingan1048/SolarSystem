#include "SolarSystem.h"

SolarSystem::SolarSystem(bool fullScreen)
{
	initGLFW();
	if (fullScreen)
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
		m_width = videoMode->width;
		m_height = videoMode->height;
		m_window = glfwCreateWindow(m_width, m_height, m_defaultTitle, nullptr, nullptr);
	}
	else
	{
		m_width = m_defaultSize[0];
		m_height = m_defaultSize[1];
		m_window = glfwCreateWindow(m_width, m_height, m_defaultTitle, nullptr, nullptr);
	}
}
SolarSystem::SolarSystem(int width, int height)
	: m_width(width), m_height(height)
{
	initGLFW();
	m_window = glfwCreateWindow(m_width, m_height, m_defaultTitle, nullptr, nullptr);
}
SolarSystem::SolarSystem()
	: m_width(m_defaultSize[0]), m_height(m_defaultSize[1])
{
	initGLFW();
	m_window = glfwCreateWindow(m_width, m_height, m_defaultTitle, nullptr, nullptr);
}

void SolarSystem::Run()
{
	initLastXY();
	glfwSets();
	glEnable(GL_DEPTH_TEST);

	initSphere();

	Shader shader("glsl\\SolarSystem.vert", "glsl\\SolarSystem.frag");
	const glm::vec3 earthPosition = glm::vec3(0.0f, 0.0f, -7.0f);
	const glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "\\SolarSystem\\pics\\";
	std::string earthPath = parentDir + texPath + "earth-png.png";
	std::string sunPath = parentDir + texPath + "sun.png";
	// Load and create a Texture
	Texture earth(earthPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
	Texture sun(sunPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
	shader.use(); // don't forget to activate/use the shader before setting uniforms!
	shader.setInt("texture1", 0);
	// Enable VSync (optional)
	glfwSwapInterval(1);

	fr.startlastTime();
	const float speed[2] = { 4.5, 1.0f };
	while (!glfwWindowShouldClose(m_window))
	{
		fr.startCurrTime();
		fr.startDeltaTime();
		if (fr.checkDeltaTime())
		{
			cam.currentFrame = static_cast<float>(glfwGetTime());
			cam.deltaTime = cam.currentFrame - cam.lastFrame;
			cam.lastFrame = cam.currentFrame;

			processInput(m_window);

			// Clear both color and depth buffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// 🔄 ROTATION via GLM
			float time = static_cast<float>(glfwGetTime());
			shader.use();
			glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), m_width / (float)m_height, 0.1f, 100.0f);
			shader.setMat4("projection", projection);

			glm::mat4 view = cam.getViewMatrix();
			shader.setMat4("view", view);
			//Bind texture 
			earth.Bind();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, time * speed[0], glm::vec3(0.0f, 1.0f, 0.0f));// rotate on Y axis
			model = glm::translate(model, earthPosition); 
			glBindVertexArray(vao);
			shader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);

			sun.Bind();
			model = glm::mat4(1.0f);
			model = glm::rotate(model, time * speed[1], glm::vec3(0.0f, 0.0f, 1.0f));// rotate on Y axis
			model = glm::translate(model, sunPosition);
			model = glm::scale(model, glm::vec3(2.0f));     // sun is larger

			glBindVertexArray(vao);
			shader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);

			fr.setLastTime(fr.getCurrTime());
		}
		else
		{
			fr.sleep();
		}

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	shader.deleteShader();
	earth.Delete();
	glfwTerminate();
}
/*
void SolarSystem::initSphere()
{
	m_vertices.clear();
	m_indices.clear();
	for (unsigned int i = 0; i <= Y_SEGMENTS; ++i)
	{
		for (unsigned int j = 0; j <= X_SEGMENTS; ++j)
		{
			float xSegment = (float)j / (float)X_SEGMENTS;
			float ySegment = (float)i / (float)Y_SEGMENTS;
			float lat = PI * (-0.5f + ySegment);
			float lng = 2.0f * PI * xSegment;

			float x = std::cos(lng) * std::cos(lat);
			float y = std::sin(lat);
			float z = std::sin(lng) * std::cos(lat);

			float xPos = RADIUS * x;
			float yPos = RADIUS * y;
			float zPos = RADIUS * z;
		
			float u = xSegment;
			float v = ySegment;

			
			float margin = 0.1f; // 10% margin from edges
			u = margin + u * (1.0f - 2.0f * margin);
			v = margin + v * (1.0f - 2.0f * margin);

			m_vertices.push_back(xPos);
			m_vertices.push_back(yPos);
			m_vertices.push_back(zPos);
			m_vertices.push_back(u);
			m_vertices.push_back(v);
		}
	}

	// Keep your existing indices generation code
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x < X_SEGMENTS; ++x)
		{
			unsigned int i0 = y * (X_SEGMENTS + 1) + x;
			unsigned int i1 = (y + 1) * (X_SEGMENTS + 1) + x; 
			unsigned int i2 = y * (X_SEGMENTS + 1) + (x + 1);
			unsigned int i3 = (y + 1) * (X_SEGMENTS + 1) + (x + 1);

			m_indices.push_back(i0);
			m_indices.push_back(i1);
			m_indices.push_back(i2);
			m_indices.push_back(i1);
			m_indices.push_back(i3);
			m_indices.push_back(i2);
		}
	}
} // the idk
*/
void SolarSystem::initSphere()
{
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;

			float xPos = RADIUS * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = RADIUS * std::cos(ySegment * PI);
			float zPos = RADIUS * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			// NEW UV MAPPING: Map sphere coordinates to circular area in texture
			float u = xSegment;
			float v = ySegment;

			// Convert sphere UV to circular coordinates
			float theta = u * 2.0f * PI;  // 0 to 2π
			float phi = v * PI;           // 0 to π

			// Map to circular area (assuming Earth circle is centered in texture)
			float radius = 0.45f; // Radius of Earth circle in texture (adjust as needed)
			float centerU = 0.5f;
			float centerV = 0.5f;

			// Convert spherical coordinates to circular texture coordinates
			float r = sin(phi) * radius;  // Distance from center
			u = centerU + r * cos(theta);
			v = centerV + r * sin(theta);

			// Ensure we stay within bounds
			u = std::max(0.0f, std::min(1.0f, u));
			v = std::max(0.0f, std::min(1.0f, v));

			m_vertices.push_back(xPos);
			m_vertices.push_back(yPos);
			m_vertices.push_back(zPos);
			m_vertices.push_back(u);
			m_vertices.push_back(v);
		}
	}

	// Keep your existing indices generation code
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x < X_SEGMENTS; ++x)
		{
			unsigned int i0 = y * (X_SEGMENTS + 1) + x;
			unsigned int i1 = (y + 1) * (X_SEGMENTS + 1) + x;
			m_indices.push_back(i0);
			m_indices.push_back(i1);
			m_indices.push_back(i0 + 1);
			m_indices.push_back(i1);
			m_indices.push_back(i1 + 1);
			m_indices.push_back(i0 + 1);
		}
	}
}// the okay slight
// print
void SolarSystem::printMat4(const glm::mat4& mat, const std::string& name) const
{
	std::print("{}: \n", name);
	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t j = 0; j < 4; ++j)
		{
			std::print("{} ", mat[i][j]);
		}
		std::println();
	}
}
void SolarSystem::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT);
}
void SolarSystem::glfwSets()
{
	if (m_window == nullptr)
	{
		std::println("Failed to create GLFW window");
		glfwTerminate();
		return;
	}
	// 🔧 This line is essential!
	glfwSetWindowUserPointer(m_window, this); // it tells GLFW “Attach this SolarSystem* pointer to the GLFWwindow* object, so I can retrieve it later inside callbacks.”
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback_wrapper);
	glfwSetCursorPosCallback(m_window, mouse_callback_wrapper);
	glfwSetScrollCallback(m_window, scroll_callback_wrapper);
	glfwSetCursorPos(m_window, m_width / 2, m_height / 2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::println("Failed to initialize GLAD");
		return;
	}
}
void SolarSystem::initGLFW()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

}
void SolarSystem::initLastXY()
{
	lastX = m_width / 2;
	lastY = m_height / 2;
}
void SolarSystem::framebuffer_size_callback(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
}
void SolarSystem::scroll_callback(double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(static_cast<float>(yoffset));
}
void SolarSystem::mouse_callback(double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);

}
// Static Wrappers
void SolarSystem::framebuffer_size_callback_wrapper(GLFWwindow* win, int width, int height)
{
	auto* app = static_cast<SolarSystem*>(glfwGetWindowUserPointer(win));
	app->framebuffer_size_callback(width, height);
}
void SolarSystem::scroll_callback_wrapper(GLFWwindow* win, double xoffset, double yoffset)
{
	auto* app = static_cast<SolarSystem*>(glfwGetWindowUserPointer(win));
	app->scroll_callback(xoffset, yoffset);
}
void SolarSystem::mouse_callback_wrapper(GLFWwindow* win, double xpos, double ypos)
{
	auto* app = static_cast<SolarSystem*>(glfwGetWindowUserPointer(win));
	app->mouse_callback(xpos, ypos);
}