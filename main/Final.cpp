/*------------- Alumno: Pachuca Cortes Santiago Emilio-----*/
/*------------- No. Cuenta: 314305846----------------------*/
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <string.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;
void getResolution(void);
// camera
Camera camera(glm::vec3(-150.0f, 40.0f, 210.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;
//Lighting
glm::vec3 colores(0.0f, 0.0f, 0.0f);
double myVariable = 0.0;

//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 210.0f,
		movAuto_y = 0.0f,
		movAuto_z = 30.0f,
		orienta = 180.0f;
int estado = 0,
	estado_anterior = 0; 

float giroLlantas = 0.0f;

bool	animacion = false,
		animacion1 = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		movBrazoIzq = 0.0f,
		movCabeza = 0.0f,
		movTorzo = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoIzqInc = 0.0f,
		movCabezaInc = 0.0f,
		movTorzoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoIzq;
	float movTorzo;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;
int indice = 1;

void saveFrame(void){
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;
	KeyFrame[FrameIndex].movTorzo = movTorzo;

	FrameIndex++;
}
void resetElements(void){
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	movBrazoIzq = KeyFrame[0].movBrazoIzq;
	movTorzo = KeyFrame[0].movTorzo;
}
void interpolation(void){
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;
	movTorzoInc = (KeyFrame[playIndex + 1].movTorzo - KeyFrame[playIndex].movTorzo) / i_max_steps;
}
void animate(void){
	colores.x = 1 * cos(myVariable);
	colores.y = 1 * sin(myVariable);
	colores.z = 1 * cos(myVariable);

	//lightPosition.x = 400 * cos(myVariable);
	//lightPosition.z = 400 * sin(myVariable);

	myVariable += 0.06;

	if (play){
		if (i_curr_steps >= i_max_steps){ //end of animation between frames?
			playIndex++;
			if (playIndex > FrameIndex - 2){	//end of total animation?
				std::cout << "Animation ended" << std::endl;
				playIndex = 0;
				play = false;
			}
			else{ //Next frame interpolations
				i_curr_steps = 0; //Reset counter
				interpolation();  //Interpolation
			}
		}
		else{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			movBrazoIzq += movBrazoIzqInc;
			movTorzo += movTorzoInc;

			i_curr_steps++;
		}
	}
	//Vehículo
	if (animacion){
		switch (estado) {
		case 0:
			movAuto_z -= 3.0f;
			giroLlantas -= 5.0f;
			if (movAuto_z <= -50.0f)
				estado = 1;
			break;
		case 1:
			movAuto_y += 3.0f;
			if (movAuto_y >= 70.0f)
				estado = 2;
			break;
		case 2:
			movAuto_z += 3.0f;
			giroLlantas += 5.0f;
			if (movAuto_z >= 150.0f)
				estado = 3;
			break;
		case 3:
			movAuto_y -= 3.0f;
			if (movAuto_y <= 0.0f)
				estado = 4;
			break;
		case 4:
			movAuto_z += 3.0f;
			giroLlantas += 5.0f;
			if (movAuto_z >= 200.0f) {
				estado_anterior = 4;
				estado = 0;
				animacion = false;
			}
			break;
		default:
			break;
		}
	}

	if (animacion1) {
		if (recorrido1) {
			movAuto_z -= 3.0f;
			orienta = 180.0f;
			giroLlantas += 9.0f;
			if (movAuto_z <= -200.0f) {
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2) {
			movAuto_x -= 3.0f;
			orienta = -90.0f;
			giroLlantas += 9.0f;
			if (movAuto_x <= -250.0f) {
				recorrido2 = false;
				//recorrido3 = true;
				recorrido5 = true;
			}
		}
		if(recorrido3){
			movAuto_z += 3.0f;
			orienta = 0.0f;
			giroLlantas += 9.0f;
			if (movAuto_z >= 200.0f) {
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4) {
			movAuto_x += 3.0f;
			orienta = -270.0f;
			giroLlantas += 9.0f;
			if (movAuto_x >= 0.0f) {
				recorrido4 = false;
				recorrido1 = true;
			}
		}
		if (recorrido5) {
			movAuto_x += (0.625*3.0f);
			movAuto_z += 3.0f;
			giroLlantas += 9.0f;
			orienta = 32.0f;
			if (movAuto_x >= 0.0f && movAuto_z >= 200.0){
				recorrido5 = false;
				recorrido1 = true;
			}
		}
	}
}

void getResolution(){
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}
int main()
{
	glfwInit();
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	monitors = glfwGetPrimaryMonitor();
	getResolution();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");
	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};
	Skybox skybox = Skybox(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	Model piso("resources/objects/piso/piso.obj");
	/*Model piernaDer("resources/objects/nuevos/modelos/br_pd.obj");
	Model piernaIzq("resources/objects/nuevos/modelos/br_pi.obj");
	Model torso("resources/objects/nuevos/modelos/br_cuerpo.obj");
	Model brazoDer("resources/objects/nuevos/modelos/br_bd.obj");
	Model brazoIzq("resources/objects/nuevos/modelos/br_bi.obj");
	
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	*/
	Model cascaron("resources/objects/nuevos/casita/cascaron.obj");
	Model pino("resources/objects/nuevos/casita/pino.obj");
	
	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++){
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// render loop
	while (!glfwWindowShouldClose(window)){
		skyboxShader.setInt("skybox", 0);
		lastFrame = SDL_GetTicks();
		animate();
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		staticShader.use();
		staticShader.setVec3("viewPos", camera.Position); 
		staticShader.setVec3("dirLight.direction", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		
		//ESQUINAS
		staticShader.setVec3("pointLight[0].position", glm::vec3(-8.0, 33.0f, 90.0f));//ESQ SE INF
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f);
		staticShader.setFloat("pointLight[0].linear", 0.09f);
		staticShader.setFloat("pointLight[0].quadratic", 0.35f);/*
		staticShader.setVec3("pointLight[1].position", glm::vec3(-9.0, 67.0f, 90.0f));//ESQ SE SUP
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.09f);
		staticShader.setFloat("pointLight[1].quadratic", 0.35f);
		staticShader.setVec3("pointLight[2].position", glm::vec3(-186.0, 33.0f, 88.0f));//ESQ SO INF
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.09f);
		staticShader.setFloat("pointLight[2].quadratic", 0.35f);
		staticShader.setVec3("pointLight[3].position", glm::vec3(-187.0, 67.0f, 88.0f));//ESQ SO SUP
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 1.0f);
		staticShader.setFloat("pointLight[3].linear", 0.09f);
		staticShader.setFloat("pointLight[3].quadratic", 0.35f);
		staticShader.setVec3("pointLight[4].position", glm::vec3(-8.0, 33.0f, -88.0f));//ESQ NE INF
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[4].constant", 1.0f);
		staticShader.setFloat("pointLight[4].linear", 0.09f);
		staticShader.setFloat("pointLight[4].quadratic", 0.35f);
		staticShader.setVec3("pointLight[5].position", glm::vec3(-9.0, 67.0f, -88.0f));//ESQ NE SUP
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[5].constant", 1.0f);
		staticShader.setFloat("pointLight[5].linear", 0.09f);
		staticShader.setFloat("pointLight[5].quadratic", 0.35f);
		staticShader.setVec3("pointLight[6].position", glm::vec3(-187.0, 67.0f, -88.0f));//ESQ NO INF
		staticShader.setVec3("pointLight[6].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[6].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[6].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[6].constant", 1.0f);
		staticShader.setFloat("pointLight[6].linear", 0.09f);
		staticShader.setFloat("pointLight[6].quadratic", 0.35f);
		staticShader.setVec3("pointLight[7].position", glm::vec3(-186.0, 33.0f, -88.0f));//ESQ NO
		staticShader.setVec3("pointLight[7].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[7].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[7].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[7].constant", 1.0f);
		staticShader.setFloat("pointLight[7].linear", 0.09f);
		staticShader.setFloat("pointLight[7].quadratic", 0.35f);
		//TERCIOS
		staticShader.setVec3("pointLight[8].position", glm::vec3(-145.0, 33.0f, 90.0f));//ESQ SE INF
		staticShader.setVec3("pointLight[8].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[8].diffuse", glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[8].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[8].constant", 1.0f);
		staticShader.setFloat("pointLight[8].linear", 0.09f);
		staticShader.setFloat("pointLight[8].quadratic", 0.35f);
		staticShader.setVec3("pointLight[9].position", glm::vec3(-145.0, 67.0f, 90.0f));//ESQ SE SUP
		staticShader.setVec3("pointLight[9].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[9].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[9].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[9].constant", 1.0f);
		staticShader.setFloat("pointLight[9].linear", 0.09f);
		staticShader.setFloat("pointLight[9].quadratic", 0.35f);
		staticShader.setVec3("pointLight[10].position", glm::vec3(-70.0, 33.0f, 90.0f));//ESQ SE INF
		staticShader.setVec3("pointLight[10].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[10].diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[10].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[10].constant", 1.0f);
		staticShader.setFloat("pointLight[10].linear", 0.09f);
		staticShader.setFloat("pointLight[10].quadratic", 0.35f);
		staticShader.setVec3("pointLight[11].position", glm::vec3(-70.0, 67.0f, 90.0f));//ESQ SE SUP
		staticShader.setVec3("pointLight[11].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[11].diffuse", glm::vec3(0.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[11].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[11].constant", 1.0f);
		staticShader.setFloat("pointLight[11].linear", 0.09f);
		staticShader.setFloat("pointLight[11].quadratic", 0.35f);
		staticShader.setVec3("pointLight[12].position", glm::vec3(-186.0, 33.0f, 30.0f));//ESQ SO INF
		staticShader.setVec3("pointLight[12].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[12].diffuse", glm::vec3(0.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[12].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[12].constant", 1.0f);
		staticShader.setFloat("pointLight[12].linear", 0.09f);
		staticShader.setFloat("pointLight[12].quadratic", 0.35f);
		staticShader.setVec3("pointLight[13].position", glm::vec3(-187.0, 67.0f, 30.0f));//ESQ SO SUP
		staticShader.setVec3("pointLight[13].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[13].diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[13].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[13].constant", 1.0f);
		staticShader.setFloat("pointLight[13].linear", 0.09f);
		staticShader.setFloat("pointLight[13].quadratic", 0.35f);
		staticShader.setVec3("pointLight[14].position", glm::vec3(-186.0, 33.0f, -30.0f));//ESQ SO INF
		staticShader.setVec3("pointLight[14].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[14].diffuse", glm::vec3(0.5f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[14].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[14].constant", 1.0f);
		staticShader.setFloat("pointLight[14].linear", 0.09f);
		staticShader.setFloat("pointLight[14].quadratic", 0.35f);
		staticShader.setVec3("pointLight[15].position", glm::vec3(-187.0, 67.0f, -30.0f));//ESQ SO SUP
		staticShader.setVec3("pointLight[15].ambient", glm::vec3(colores.y, colores.z, colores.x));
		staticShader.setVec3("pointLight[15].diffuse", glm::vec3(0.0f, 0.5f, 1.0f));
		staticShader.setVec3("pointLight[15].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[15].constant", 1.0f);
		staticShader.setFloat("pointLight[15].linear", 0.09f);
		staticShader.setFloat("pointLight[15].quadratic", 0.35f);
		staticShader.setVec3("pointLight[16].position", glm::vec3(-8.0, 33.0f, -30.0f));//ESQ NE INF
		staticShader.setVec3("pointLight[16].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[16].diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[16].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[16].constant", 1.0f);
		staticShader.setFloat("pointLight[16].linear", 0.09f);
		staticShader.setFloat("pointLight[16].quadratic", 0.35f);
		staticShader.setVec3("pointLight[17].position", glm::vec3(-9.0, 67.0f, -30.0f));//ESQ NE SUP
		staticShader.setVec3("pointLight[17].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[17].diffuse", glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[17].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[17].constant", 1.0f);
		staticShader.setFloat("pointLight[17].linear", 0.09f);
		staticShader.setFloat("pointLight[17].quadratic", 0.35f);
		staticShader.setVec3("pointLight[18].position", glm::vec3(-8.0, 33.0f, 30.0f));//ESQ NE INF
		staticShader.setVec3("pointLight[18].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[18].diffuse", glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[18].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[18].constant", 1.0f);
		staticShader.setFloat("pointLight[18].linear", 0.09f);
		staticShader.setFloat("pointLight[18].quadratic", 0.35f);
		staticShader.setVec3("pointLight[19].position", glm::vec3(-9.0, 67.0f, 30.0f));//ESQ NE SUP
		staticShader.setVec3("pointLight[19].ambient", glm::vec3(colores.z, colores.x, colores.y));
		staticShader.setVec3("pointLight[19].diffuse", glm::vec3(1.0f, 0.0f, 0.5f));
		staticShader.setVec3("pointLight[19].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[19].constant", 1.0f);
		staticShader.setFloat("pointLight[19].linear", 0.09f);
		staticShader.setFloat("pointLight[19].quadratic", 0.35f);
		staticShader.setVec3("pointLight[20].position", glm::vec3(-145.0, 67.0f, -88.0f));//ESQ NO INF
		staticShader.setVec3("pointLight[20].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[20].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[20].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[20].constant", 1.0f);
		staticShader.setFloat("pointLight[20].linear", 0.09f);
		staticShader.setFloat("pointLight[20].quadratic", 0.35f);
		staticShader.setVec3("pointLight[21].position", glm::vec3(-145.0, 33.0f, -88.0f));//ESQ NO
		staticShader.setVec3("pointLight[21].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[21].diffuse", glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[21].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[21].constant", 1.0f);
		staticShader.setFloat("pointLight[21].linear", 0.09f);
		staticShader.setFloat("pointLight[21].quadratic", 0.35f);
		staticShader.setVec3("pointLight[22].position", glm::vec3(-70.0, 67.0f, -88.0f));//ESQ NO INF
		staticShader.setVec3("pointLight[22].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[22].diffuse", glm::vec3(1.0f, 0.5f, 0.5f));
		staticShader.setVec3("pointLight[22].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[22].constant", 1.0f);
		staticShader.setFloat("pointLight[22].linear", 0.09f);
		staticShader.setFloat("pointLight[22].quadratic", 0.35f);
		staticShader.setVec3("pointLight[23].position", glm::vec3(-70.0, 33.0f, -88.0f));//ESQ NO
		staticShader.setVec3("pointLight[23].ambient", glm::vec3(colores.x, colores.y, colores.z));
		staticShader.setVec3("pointLight[32].diffuse", glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[23].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[23].constant", 1.0f);
		staticShader.setFloat("pointLight[23].linear", 0.09f);
		staticShader.setFloat("pointLight[23].quadratic", 0.35f);*/

		staticShader.setFloat("material_shininess", 32.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		// Personaje Animacion
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", glm::vec3(0.0f, 0.0f, 0.0f));
		animShader.setVec3("viewPos", camera.Position);

		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		cascaron.Draw(staticShader);
		/*
		// Carro
		model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movAuto_x, movAuto_y, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		
		//Stewie
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 200, 0));
		model = glm::translate(model, glm::vec3(posX, movTorzo, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);
		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);
		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);
		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(-movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);
		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);
		*/
		//piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		// draw skybox as last
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME){
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	skybox.Terminate();
	glfwTerminate();
	return 0;
}

void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		printf("hola");
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movBrazoIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		movBrazoIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movCabeza += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		movCabeza -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		movTorzo += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		movTorzo -= 2.0f;

	//Car animation
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && estado_anterior == 0)
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion1 ^= true;
		//animacion ^= true;

	if (key == GLFW_KEY_R && action == GLFW_PRESS && animacion == false && estado_anterior == 4) {
		movAuto_z = 0.0f;
		movAuto_y = 0.0f;
		estado_anterior = 0;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS){
		if (play == false && (FrameIndex > 1)){
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS){
		if (FrameIndex < MAX_FRAMES){
			saveFrame();
		}
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}