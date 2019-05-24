#include "homework.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GLFW/glfw3.h>


vector< vector<glm::vec3>> drawBezier(vector<glm::vec3> &cPoints, float step) {
	vector< vector<glm::vec3>> allPoints;
	vector<glm::vec3> currentPoints = cPoints;
	allPoints.push_back(currentPoints);
	if (step > 0 && step < 1) {
		while (currentPoints.size() >= 2) {
			vector<glm::vec3> temp;
			for (int i = 0; i < currentPoints.size() - 1; i++) {
				glm::vec3 p(currentPoints[i] + step * (currentPoints[i + 1] - currentPoints[i]));
				temp.push_back(p);
			}
			currentPoints = temp;
			allPoints.push_back(currentPoints);
		}
	}
	return allPoints;
}

int cij(int i, int j) {
	if (i < j) {
		return 0;
	}
	else if (i == j || j == 0) {
		return 1;
	}
	else {
		return cij(i - 1, j) + cij(i - 1, j - 1);
	}
}

vector<glm::vec3> drawBezier(vector<glm::vec3> &cPoints, int amount) {
	vector<glm::vec3> pointsWithFactor, points;
	const int nums = cPoints.size();
	for (int i = 0; i < nums; i++) {
		auto temp = cPoints[i];
		temp *= cij(nums - 1, i);
		pointsWithFactor.push_back(temp);
	}
	float step = 1 / static_cast<float>(amount);
	for (int i = 0; i <= amount; i++) {
		float t = i * step;
		glm::vec3 point(0);
		for (int j = 0; j < pointsWithFactor.size(); j++) {
			glm::vec3 temp = pointsWithFactor[j];
			temp *= pow(1 - t, nums - 1 - j) * pow(t, j);
			point += temp;
		}
		points.push_back(point);
	}
	return points;
}


void installHW8(GLFWwindow *window) {

	float vertex[2] = { 0, 1 };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	bool my_tool_active = true, isOrtho = true;
	vector<glm::vec3> controlPoints;

	Shader pointOrLineShader("./hw8_line.vs", "./hw8_line.fs");
	pointOrLineShader.use();
	pointOrLineShader.setVec3(glm::vec3(0.7), "color");

	double xPos, yPos;
	int windowWidth, windowHeight, amount = 100, speed = 50, maxSpeed = 100;
	bool firstClick = false, firstPress = false, playAnimation = false;

	double currentT = 0, lastFrame = 0;
	vector<glm::vec3> curvePoints;
	vector< vector<glm::vec3>> points;
	
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

		ImGui::Text("Click the left mouse button to place control point");
		ImGui::Text("Click the right mouse button to cancle last placement");
		ImGui::Text("Press SPACE button to display animation");
		ImGui::Text("Press Z to RESET");
		ImGui::SliderInt("line numbser", &amount, 10, 500);
		ImGui::SliderInt("speed", &speed, 1, maxSpeed);

		ImGui::End();
		ImGui::Render();

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		pointOrLineShader.use();

		glBindVertexArray(VAO);
		if (firstClick && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			firstClick = false;
			playAnimation = false;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			glfwGetCursorPos(window, &xPos, &yPos);
			cout << 2 * xPos / windowWidth - 1 << 1 - 2 * yPos / windowHeight << endl;
			controlPoints.push_back(glm::vec3(2 * xPos / windowWidth - 1, 1 - 2 * yPos / windowHeight, 0));
			curvePoints = drawBezier(controlPoints, amount);
		}
		else if (firstClick && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			firstClick = false;
			playAnimation = false;
			if (!controlPoints.empty()) {
				controlPoints.pop_back();
				curvePoints = drawBezier(controlPoints, amount);
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE 
			&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			firstClick = true;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			playAnimation = true;
			if (currentT > 0.1) {
				currentT = 0;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			controlPoints.clear();
			playAnimation = false;
		}

		glPointSize(10.0f);
		if (!controlPoints.empty()) {
			for (auto point : controlPoints) {
				pointOrLineShader.setVec3(point, "start");
				glDrawArrays(GL_POINTS, 0, 1);
			}

			for (int i = 0; i < controlPoints.size() - 1; i++) {
				pointOrLineShader.setVec3(controlPoints[i], "start");
				pointOrLineShader.setVec3(controlPoints[i + 1], "end");
				glDrawArrays(GL_LINES, 0, 2);
			}
			for (int i = 0; i < curvePoints.size() - 1; i++) {
				pointOrLineShader.setVec3(curvePoints[i], "start");
				pointOrLineShader.setVec3(curvePoints[i + 1], "end");
				glDrawArrays(GL_LINES, 0, 2);
			}
		}

		auto deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		if (playAnimation) {
			currentT = currentT + (deltaTime * speed) / (maxSpeed);
			points = drawBezier(controlPoints, static_cast<float>(currentT));

			glPointSize(4.0f);
			for (int i = 1; i < points.size() - 1; i++) {
				for (int j = 0; j < points[i].size(); j++) {
					pointOrLineShader.setVec3(points[i][j], "start");
					glDrawArrays(GL_POINTS, 0, 1);
				}
				for (int j = 0; j < points[i].size() - 1; j++) {
					pointOrLineShader.setVec3(points[i][j], "start");
					pointOrLineShader.setVec3(points[i][j + 1], "end");
					glDrawArrays(GL_LINES, 0, 2);
				}
			}

			if (points.size() > 1) {
				pointOrLineShader.setVec3(points[points.size()-1][0], "start");
				pointOrLineShader.setVec3(glm::vec3(1.0, 0, 0), "color");
				glPointSize(7.0f);
				glDrawArrays(GL_POINTS, 0, 1);
				pointOrLineShader.setVec3(glm::vec3(0.7), "color");
			}
		}
		else {
			currentT = 0;
		}
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}