#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Prot?tipo da fun??o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot?tipos das fun??es
int setupGeometry();

// Dimens?es da janela (pode ser alterado em tempo de execu??o)
const GLuint WIDTH = 800, HEIGHT = 600;

// Fun??o MAIN
int main()
{
	// Inicializa??o da GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Cria??o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun??o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa??es de vers?o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens?es da viewport com as mesmas dimens?es da janela da aplica??o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	Shader shader("../shaders/transformations.vs", "../shaders/transformations.frag");

	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "contourColor");
	assert(colorLoc > -1);
	glUseProgram(shader.ID);
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f); //para cor de contorno

	// Para enviar o identificador de modo de desenho
	GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");

	// Loop da aplica??o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		//aqui recupera o tamanho atual da janela, para corre??o do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(400, 300, width, height);

		glm::mat4 model = glm::mat4(1);

		// Create transformations -- a first "camera" -- Orthographic Camera
		glm::mat4 ortho = glm::mat4(1);;

		//Matriz de modelo - Transforma??es no objeto
		model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//precisa comentar esta parte para o exercicio 3 funcionar

		//Matriz ortogr?fica - C?mera 2D
		//ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); 

		//Exercicio 1
		//ortho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

		//Exercicio 2 & 3
		//ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

		//Exerc?cio 4
		ortho = glm::ortho(-1.0f, 3.0f, -1.0f, 3.0f, -1.0f, 1.0f);
			   

		////corrigindo o aspecto
		//float ratio;
		//float xMin = -1.0, xMax = 1.0, yMin = -1.0, yMax = 1.0;
		//float zNear = -1.0, zFar = 1.0;
		//if (width >= height)
		//{
		//	ratio = width / (float)height;
		//	ortho =  glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax,zNear,zFar);
		//}
		//else
		//{
		//	ratio = height / (float)width;
		//	ortho =  glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
		//}

		// Obtem seus identificadores de localiza??o no shader
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");
		// Passa suas informa??es efetivamente para o shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));


		glLineWidth(5);
		glPointSize(10);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glUniform1i(rasterCodeLoc, 0); //zero ? preenchido
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Chamada de desenho - drawcall
		// Contorno do pol?gono - GL_LINE_LOOP
		glUniform1i(rasterCodeLoc, 1); //1 ? contornado
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		glBindVertexArray(0);

	
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f, 1.0, 0.0, 0.0, //Superior esquerdo
		0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, //Inferior esquerdo
	   -0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0, //Superior esquerdo


		0.5f, -0.5f, 0.0f, 1.0, 1.0, 0.0,
	   -0.5f, -0.5f, 0.0, 1.0, 0.0, 1.0,  //Inferior esquerdo
	   -0.5f,  0.5f, 0.0f, 0.0, 1.0, 1.0
	};

	//EXERCICIO 3
	//GLfloat vertices[] = {
	//	600.0f,  450.0f, 0.0f, 1.0, 0.0, 0.0, //Superior esquerdo
	//	600.0f, 150.0f, 0.0f, 0.0, 1.0, 0.0, //Inferior esquerdo
	//	200.0f, 450.5f, 0.0f, 0.0, 0.0, 1.0, //Superior esquerdo


	//	600.0f, 150.0f, 0.0f, 1.0, 1.0, 0.0,
	//	200.0f, 150.0f, 0.0, 1.0, 0.0, 1.0,  //Inferior esquerdo
	//	200.0f, 450.0f, 0.0f, 0.0, 1.0, 1.0
	//};

	GLuint VBO, VAO;

	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), ((GLvoid*)(3 * sizeof(GLfloat))));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}