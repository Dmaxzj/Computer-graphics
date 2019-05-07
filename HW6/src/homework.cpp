#include "homework.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, bool &flag);

void installHW2(GLFWwindow* window) {
	Shader ShaderProgram1("./shader1.vs", "./shader1.fs");
	Shader ShaderProgram2("./shader2.vs", "./shader2.fs");


	float vertices1[] = {
		-0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left  
		 -0.1f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		 -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // top
		 0.3, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,		// top right
		 0.7f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = { 0, 1, 2,
							1, 2, 3,
							3, 4 };

	unsigned int VBO1, VAO1, EBO;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	bool my_tool_active = false;
	float my_color[] = { 0.5, 0.5, 0.0, 1.0f };
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	float point1[2] = { 0 }, point2[2] = { 0 }, point3[3] = { 0 };
	bool isTriangle = true;
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("mode"))
			{
				if (ImGui::MenuItem("Triangle", "", &isTriangle)) {}
				if (ImGui::MenuItem("Circle", "")) { isTriangle = !isTriangle; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::SliderFloat2("first point", point1, -1, 1, "%.2f");
		ImGui::ColorEdit4("Color", my_color);
		ImGui::End();
		ImGui::Render();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ShaderProgram1.use();

		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_POINTS, 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * 6));

		ShaderProgram2.use();
		ShaderProgram2.setColor(my_color[0], my_color[1], my_color[2], my_color[3]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * 3));

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
}

void installHW3(GLFWwindow* window) {
	Shader ShaderProgram("./hw3_0.vs", "./hw3_0.fs");

	float vertex[3] = { 0.5, 0.5, 0.0 };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	bool my_tool_active = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	float point1[2] = { 0.0 }, point2[2] = { 0.0 }, point3[3] = { 0.0 }, radius = 0.0;
	bool isTriangle = true, isRasterzation = false;

	bool shouldUpdate = false;
	vector< vector<float>> buffer;
	
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("mode"))
			{
				if (ImGui::MenuItem("Triangle", "", &isTriangle)) {
					shouldUpdate = true;
				}
				if (ImGui::MenuItem("Circle", "")) { 
					shouldUpdate = true;
					isTriangle = !isTriangle; 
				}
				if (ImGui::Checkbox("Rasterzatation", &isRasterzation)) {
					shouldUpdate = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (isTriangle) {
			shouldUpdate |= ImGui::SliderFloat2("first point", point1, -1, 1, "%.2f");
			shouldUpdate |= ImGui::SliderFloat2("second point", point2, -1, 1, "%.2f");
			shouldUpdate |= ImGui::SliderFloat2("third point", point3, -1, 1, "%.2f");
		}
		else {
			shouldUpdate |= ImGui::SliderFloat("radius", &radius, 0, 1, "%.2f");
		}

		ImGui::End();
		ImGui::Render();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ShaderProgram.use();
		glBindVertexArray(VAO);
		if (shouldUpdate == false) {
			for (auto const v : buffer) {
				float p[3] = { v[0], v[1], v[2] };
				ShaderProgram.setVec3(p, "position");
				glDrawArrays(GL_POINTS, 0, 1);
			}
		}
		else {
			shouldUpdate = false;
			if (isTriangle) {
				auto line1 = bresenhamDrawLine(point1, point2);
				auto line2 = bresenhamDrawLine(point1, point3);
				auto line3 = bresenhamDrawLine(point2, point3);
				line1.insert(line1.end(), line2.begin(), line2.end());
				line1.insert(line1.end(), line3.begin(), line3.end());
				if (isRasterzation) {
					auto r = rasterzatationTriangle(line1);
					for (auto const v : r) {
						float p[3] = { v[0], v[1], v[2] };
						ShaderProgram.setVec3(p, "position");
						glDrawArrays(GL_POINTS, 0, 1);
					}
					buffer = r;
				}
				else {
					for (auto const v : line1) {
						float p[3] = { v[0], v[1], v[2] };
						ShaderProgram.setVec3(p, "position");
						glDrawArrays(GL_POINTS, 0, 1);
					}
					buffer = line1;
				}

			}
			else {
				auto circle = bresenhamDrawCircle(radius);
				for (auto const v : circle) {
					float p[3] = { v[0], v[1], v[2] };
					ShaderProgram.setVec3(p, "position");
					glDrawArrays(GL_POINTS, 0, 1);
				}
				buffer = circle;
			}
		}
		


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void installHW4(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	Shader ShaderProgram("./hw4_0.vs", "./hw4_0.fs");
	Shader ShaderProgram2("./hw4_0.vs", "./hw4_0.fs");

	float vertices[] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("./resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("./resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	ShaderProgram.use();
	ShaderProgram.setInt(0, "texture1");
	ShaderProgram.setInt(1, "texture2");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -30.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ShaderProgram.setMat4(view, "view");
	ShaderProgram.setMat4(projection, "projection");

	ShaderProgram2.use();
	ShaderProgram2.setInt(0, "texture1");
	ShaderProgram2.setInt(1, "texture2");
	ShaderProgram2.setMat4(view, "view");
	ShaderProgram2.setMat4(projection, "projection");
	ShaderProgram2.setMat4(model, "model");
	bool my_tool_active = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	bool isTranslate = false, isRotate = false, isScale = false;
	float preTranslateX = 0.0, preTranslateZ = 0.0, preTranslateY = 0.0, preRotate = 0.0, preScale =  1.0;
	double preTTime = 0.00, preRTime = 0.00, preSTime = 1.00;
	double preTime = 0.00, currentTime = 0.00;
	while (!glfwWindowShouldClose(window))
	{	
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		currentTime = glfwGetTime();
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("mode"))
			{
				if (ImGui::Checkbox("Translate", &isTranslate)) {

				}
				if (ImGui::Checkbox("Rotate", &isRotate)) {

				}
				if (ImGui::Checkbox("Scale", &isScale)) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
		ImGui::Render();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ShaderProgram.use();

		model = glm::mat4(1.0f);
		if (isTranslate) {
			preTTime += currentTime - preTime;
			preTranslateX = sin(preTTime) * 10;
			preTranslateZ = cos(preTTime) * 12;
			preTranslateY = -cos(preTTime) * 2;
		}

		if (isRotate) {
			preRTime += currentTime - preTime;
			preRotate = preRTime;
		}

		if (isScale) {
			preSTime += currentTime - preTime;
			preScale = (abs(sin(preSTime)) + 0.2) * 0.8;
		}

		preTime = currentTime;
		model = glm::translate(model, glm::vec3(preTranslateX, preTranslateY, preTranslateZ));
		model = glm::rotate(model, preRotate, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(preScale));
		ShaderProgram.setMat4(model, "model");

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ShaderProgram2.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


auto camera = Camera();
void installHW5(GLFWwindow *window)
{
  glEnable(GL_DEPTH_TEST);
  glfwSetCursorPosCallback(window, mouse_callback);

  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  Shader ShaderProgram("./hw5_0.vs", "./hw5_0.fs");
  
  float vertices[] = {
      -2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
      2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
      2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
      2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
      -2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
      -2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,

      -2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
      2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
      2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
      2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
      -2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
      -2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,

      -2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
      -2.0f, 2.0f, 2.0f, 1.0f, 0.0f, 1.0f,

      2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
      2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
      2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
      2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
      2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
      2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 1.0f,

      -2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
      2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 0.0f,
      2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
      2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
      -2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
      -2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 0.0f,

      -2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
      2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
      2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
      2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
      -2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
      -2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 1.0f};
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);


  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);


  ShaderProgram.use();
  ShaderProgram.setMat4(model, "model");
  ShaderProgram.setMat4(view, "view");
  ShaderProgram.setMat4(projection, "projection");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  bool my_tool_active = true;

  bool isOrtho = true;
  bool isViewChanging = false;
  bool isMoveFree = false;
  float left = -8, right = 8, bottom = -6, top = 6, _near = 0.1f, _far = 100;
  float angle = 45, width = 800, height = 600, __near = 0.1f, __far = 100;

  while (!glfwWindowShouldClose(window))
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("mode"))
		{
			if (ImGui::Checkbox("Ortho or Projection", &isOrtho)) {
				if (isOrtho) {
					view = glm::mat4(1.0f);
				}
				else
				{
					view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
				}
			}
			if (ImGui::Checkbox("ViewChanging", &isViewChanging)) {
				isMoveFree = false;
			}

			if (ImGui::Checkbox("Free Move (Press ESC to leave)", &isMoveFree)) {
				if (isMoveFree) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				isViewChanging = false;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (isOrtho) {
		ImGui::InputFloat("left", &left, 1, 10);
		ImGui::InputFloat("right", &right, 1, 10);
		ImGui::InputFloat("bottom", &bottom, 1, 10);
		ImGui::InputFloat("top", &top, 1, 10);
		ImGui::InputFloat("near", &_near, 1, 10);
		ImGui::InputFloat("far", &_far, 1, 10);
	}
	else {
		ImGui::InputFloat("angle", &angle, 1, 10);
		ImGui::InputFloat("width", &width, 10, 100);
		ImGui::InputFloat("height", &height, 10, 100);
		ImGui::InputFloat("near", &__near, 0.1, 10);
		ImGui::InputFloat("far", &__far, 1, 10);
	}
	ImGui::End();
    ImGui::Render();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	processInput(window, isMoveFree);

	if (isOrtho) {
		projection = glm::ortho(left, right, bottom, top, _near, _far);
		
	}
	else {
		projection = glm::perspective(glm::radians(angle), (float)(width / height) , __near, __far);
	}

    ShaderProgram.use();

	if (isViewChanging) {
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (isMoveFree) {
		view = camera.getViewMatrix();
	}

	ShaderProgram.setMat4(view, "view");
	ShaderProgram.setMat4(projection, "projection");
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

bool firstMouse = true;
float deltaTime = 0.0; 
float lastFrame = 0.0;
void processInput(GLFWwindow *window, bool &flag)
{	
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		flag = false;
	}

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveForward(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveBack(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLeft(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveRight(cameraSpeed);
	}
}



float lastX = 0;
float lastY = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; 
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera.rotate(xoffset, yoffset);
}

void installHW6(GLFWwindow *window) {
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	Shader cubeProgram("./hw6_0.vs", "./hw6_0.fs");
	Shader lightProgram("./hw6_1.vs", "./hw6_1.fs");
	Shader cubeProgramGouraud("./hw6_2.vs", "./hw6_2.fs");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int VBO, VAO, VAO2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float lightPosition[3] = { 2.0f, 1.0f, 2.0f };
	model = glm::translate(model, glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
	view = glm::translate(view, glm::vec3(-2.0f, -2.0f, -10.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightProgram.use();
	lightProgram.setMat4(model, "model");
	lightProgram.setMat4(view, "view");
	lightProgram.setMat4(projection, "projection");

	cubeProgram.use();
	cubeProgram.setMat4(glm::mat4(1.0f), "model");
	cubeProgram.setMat4(view, "view");
	cubeProgram.setMat4(projection, "projection");

	cubeProgramGouraud.use();
	cubeProgramGouraud.setMat4(glm::mat4(1.0f), "model");
	cubeProgramGouraud.setMat4(view, "view");
	cubeProgramGouraud.setMat4(projection, "projection");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	bool my_tool_active = true;

	float objectColor[3] = { 1.0f, 0.5f, 0.31f }, lightColor[3] = { 1.0f, 1.0f, 1.0f }
	, viewPosition[3] = { 2.0f, 2.0f, 10.0f };
	float ambientFactor = 0.5f, specularFactor = 0.5f, diffuseFactor = 0.5f;
	int reflectDegree = 2;

	bool isPhongShading = true, isMoving = false;
	float theta = 0;

	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{	
			
			if (ImGui::BeginMenu("mode"))
			{
				ImGui::Checkbox("Phong Shading", &isPhongShading);
				ImGui::Checkbox("Move Light", &isMoving);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::SliderFloat("ambient", &ambientFactor, 0, 1);
		ImGui::SliderFloat("diffuse", &diffuseFactor, 0, 1);
		ImGui::SliderFloat("specular", &specularFactor, 0, 10);
		ImGui::SliderInt("shiny", &reflectDegree, 1, 100);
		
		ImGui::End();
		ImGui::Render();


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (isMoving) {
			theta += deltaTime;
			lightPosition[0] = 2 * cos(theta);
			lightPosition[2] = 2 * sin(theta);
		}

		lightProgram.use();
		model = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
		lightProgram.setMat4(model, "model");
		if (isPhongShading) {
			cubeProgram.use();

			cubeProgram.setVec3(objectColor, "objectColor");
			cubeProgram.setVec3(lightColor, "lightColor");
			cubeProgram.setVec3(lightPosition, "lightPosition");
			cubeProgram.setVec3(viewPosition, "viewPosition");
			cubeProgram.setFloat(ambientFactor, "ambientFactor");
			cubeProgram.setFloat(diffuseFactor, "diffuseFactor");
			cubeProgram.setFloat(specularFactor, "specularFactor");
			cubeProgram.setInt(reflectDegree, "reflectDegree");
		}
		else {
			cubeProgramGouraud.use();
			cubeProgramGouraud.setVec3(objectColor, "objectColor");
			cubeProgramGouraud.setVec3(lightColor, "lightColor");
			cubeProgramGouraud.setVec3(lightPosition, "lightPosition");
			cubeProgramGouraud.setVec3(viewPosition, "viewPosition");
			cubeProgramGouraud.setFloat(ambientFactor, "ambientFactor");
			cubeProgram.setFloat(diffuseFactor, "diffuseFactor");
			cubeProgramGouraud.setFloat(specularFactor, "specularFactor");
			cubeProgramGouraud.setInt(reflectDegree, "reflectDegree");
		}
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightProgram.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}