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
//Camara -------------------------------------------------------------------------------------------------------------------------
Camera camera(glm::vec3(-150.0f, 40.0f, 210.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;
//Luces -------------------------------------------------------------------------------------------------------------------------
glm::vec3 colores(0.0f, 0.0f, 0.0f);
double myVariable = 0.0;
//Variables -------------------------------------------------------------------------------------------------------------------------
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

//Keyframes -------------------------------------------------------------------------------------------------------------------------
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		movBrazoIzq = 0.0f,
		movTorzo = 0.0f;
float	posX2 = 0.0f,
		posY2 = 0.0f,
		posZ2 = 0.0f,
		rotRodIzq2 = 0.0f,
		rotX = 0.0f,
		giroMonito2 = 0.0f,
		movBrazoIzq2 = 0.0f,
		movTorzo2 = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoIzqInc = 0.0f,
		movTorzoInc = 0.0f;

float	incX2 = 0.0f,
		incY2 = 0.0f,
		incZ2 = 0.0f,
		rotInc2 = 0.0f,
		rotXInc = 0.0f,
		giroMonitoInc2 = 0.0f,
		movBrazoIzqInc2 = 0.0f,
		movTorzoInc2 = 0.0f;

#define MAX_FRAMES 22
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame{
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float rotX;
	float giroMonito;
	float movBrazoIzq;
	float movTorzo;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
FRAME KeyFrame2[MAX_FRAMES];

int FrameIndex = 22;
bool play = false;
int playIndex = 0;
int indice = 1;

void saveFrame(void){
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotX = rotX;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;
	KeyFrame[FrameIndex].movTorzo = movTorzo;
	
	std::cout << "X: " << KeyFrame[FrameIndex].posX << std::endl;
	std::cout << "Y: " << KeyFrame[FrameIndex].posY << std::endl;
	std::cout << "Z: " << KeyFrame[FrameIndex].posZ << std::endl;
	std::cout << "Torzo: " << KeyFrame[FrameIndex].movTorzo << std::endl;
	std::cout << "Brazo: " << KeyFrame[FrameIndex].movBrazoIzq << std::endl;
	std::cout << "Pie: " << KeyFrame[FrameIndex].rotRodIzq << std::endl;
	std::cout << "RotX: " << KeyFrame[FrameIndex].rotX << std::endl;
	std::cout << "Giro: " << KeyFrame[FrameIndex].giroMonito << std::endl;
	
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

	posX2 = KeyFrame2[0].posX;
	posY2 = KeyFrame2[0].posY;
	posZ2 = KeyFrame2[0].posZ;
	rotRodIzq2 = KeyFrame2[0].rotRodIzq;
	rotX = KeyFrame2[0].rotX;
	giroMonito2 = KeyFrame2[0].giroMonito;
	movBrazoIzq2 = KeyFrame2[0].movBrazoIzq;
	movTorzo2 = KeyFrame2[0].movTorzo;
}
void interpolation(void){
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	incX2 = (KeyFrame2[playIndex + 1].posX - KeyFrame2[playIndex].posX) / i_max_steps;
	incY2 = (KeyFrame2[playIndex + 1].posY - KeyFrame2[playIndex].posY) / i_max_steps;
	incZ2 = (KeyFrame2[playIndex + 1].posZ - KeyFrame2[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	rotInc2 = (KeyFrame2[playIndex + 1].rotRodIzq - KeyFrame2[playIndex].rotRodIzq) / i_max_steps;

	rotXInc = (KeyFrame2[playIndex + 1].rotX - KeyFrame2[playIndex].rotX) / i_max_steps;

	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;
	movTorzoInc = (KeyFrame[playIndex + 1].movTorzo - KeyFrame[playIndex].movTorzo) / i_max_steps;

	giroMonitoInc2 = (KeyFrame2[playIndex + 1].giroMonito - KeyFrame2[playIndex].giroMonito) / i_max_steps;
	movBrazoIzqInc2 = (KeyFrame2[playIndex + 1].movBrazoIzq - KeyFrame2[playIndex].movBrazoIzq) / i_max_steps;
	movTorzoInc2 = (KeyFrame2[playIndex + 1].movTorzo - KeyFrame2[playIndex].movTorzo) / i_max_steps;
}
//Funciones -------------------------------------------------------------------------------------------------------------------------
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

			posX2 += incX2;
			posY2 += incY2;
			posZ2 += incZ2;

			rotRodIzq += rotInc;
			rotRodIzq2 += rotInc2;

			rotX += rotXInc;

			giroMonito += giroMonitoInc;
			movBrazoIzq += movBrazoIzqInc;
			movTorzo += movTorzoInc;

			giroMonito2 += giroMonitoInc2;
			movBrazoIzq2 += movBrazoIzqInc2;
			movTorzo2 += movTorzoInc2;

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
//Main -------------------------------------------------------------------------------------------------------------------------
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
	//Carga de modelos -------------------------------------------------------------------------------------------------------------------------
	Model piso("resources/objects/piso/piso.obj");

	Model piernaDer("resources/objects/nuevos/modelos/st_pd.obj");
	Model piernaIzq("resources/objects/nuevos/modelos/st_pi.obj");
	Model torso("resources/objects/nuevos/modelos/st_cuerpo.obj");
	Model brazoDer("resources/objects/nuevos/modelos/st_bd.obj");
	Model brazoIzq("resources/objects/nuevos/modelos/st_bi.obj");

	Model piernaDer2("resources/objects/nuevos/modelos/br_pd.obj");
	Model piernaIzq2("resources/objects/nuevos/modelos/br_pi.obj");
	Model torso2("resources/objects/nuevos/modelos/br_cuerpo.obj");
	Model brazoDer2("resources/objects/nuevos/modelos/br_bd.obj");
	Model brazoIzq2("resources/objects/nuevos/modelos/br_bi.obj");
	/*
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model pino("resources/objects/nuevos/casita/pino.obj");*/
	Model cascaron("resources/objects/nuevos/casita/cascaron.obj");
	Model escalera("resources/objects/nuevos/casita/escalera.obj");

	KeyFrame2[0].posX = 0.0f;
	KeyFrame2[0].posY = 0.0f;
	KeyFrame2[0].posZ = 0.0f;
	KeyFrame2[0].movTorzo = 0.0f;
	KeyFrame2[0].movBrazoIzq = 0.0f;
	KeyFrame2[0].rotRodIzq = 0.0f;
	KeyFrame2[0].rotX = 0.0f;
	KeyFrame2[0].giroMonito = 0.0f;

	KeyFrame2[1].posX = 25.0f;
	KeyFrame2[1].posY = 0.0f;
	KeyFrame2[1].posZ = 27.0f;
	KeyFrame2[1].movTorzo = 0.0f;
	KeyFrame2[1].movBrazoIzq = -60.0f;
	KeyFrame2[1].rotRodIzq = 13.0f;
	KeyFrame2[1].rotX = 0.0f;
	KeyFrame2[1].giroMonito = 32.0f;

	KeyFrame2[2].posX = 76.0f;
	KeyFrame2[2].posY = 0.0f;
	KeyFrame2[2].posZ = 33.0f;
	KeyFrame2[2].movTorzo = 0.0f;
	KeyFrame2[2].movBrazoIzq = -87.0f;
	KeyFrame2[2].rotRodIzq = 26.0f;
	KeyFrame2[2].giroMonito = 68.0f;

	KeyFrame2[3].posX = 139.0f;
	KeyFrame2[3].posY = 0.0f;
	KeyFrame2[3].posZ = 41.0f;
	KeyFrame2[3].movTorzo = 0.0f;
	KeyFrame2[3].movBrazoIzq = 39.0f;
	KeyFrame2[3].rotRodIzq = -21.0f;
	KeyFrame2[3].giroMonito = 82.0f;

	KeyFrame2[4].posX = 169.0f;
	KeyFrame2[4].posY = 0.0f;
	KeyFrame2[4].posZ = 57.0f;
	KeyFrame2[4].movTorzo = 0.0f;
	KeyFrame2[4].movBrazoIzq = 39.0f;
	KeyFrame2[4].rotRodIzq = -21.0f;
	KeyFrame2[4].giroMonito = 56.0f;

	KeyFrame2[5].posX = 218.0f;
	KeyFrame2[5].posY = 0.0f;
	KeyFrame2[5].posZ = 83.0f;
	KeyFrame2[5].movTorzo = 0.0f;
	KeyFrame2[5].movBrazoIzq = 78.0f;
	KeyFrame2[5].rotRodIzq = -38.0f;
	KeyFrame2[5].giroMonito = 36.0f;

	KeyFrame2[6].posX = 269.0f;
	KeyFrame2[6].posY = 0.0f;
	KeyFrame2[6].posZ = 130.0f;
	KeyFrame2[6].movTorzo = 0.0f;
	KeyFrame2[6].movBrazoIzq = 78.0f;
	KeyFrame2[6].rotRodIzq = -38.0f;
	KeyFrame2[6].giroMonito = 19.0f;

	KeyFrame2[7].posX = 267.0f;
	KeyFrame2[7].posY = 0.0f;
	KeyFrame2[7].posZ = 184.0f;
	KeyFrame2[7].movTorzo = 0.0f;
	KeyFrame2[7].movBrazoIzq = -90.0f;
	KeyFrame2[7].rotRodIzq = 15.0f;
	KeyFrame2[7].giroMonito = 10.0f;

	KeyFrame2[8].posX = 267.0f;
	KeyFrame2[8].posY = 0.0f;
	KeyFrame2[8].posZ = 236.0f;
	KeyFrame2[8].movTorzo = -14.0f;
	KeyFrame2[8].movBrazoIzq = -102.0f;
	KeyFrame2[8].rotRodIzq = 27.0f;
	KeyFrame2[8].giroMonito = 8.0f;

	KeyFrame2[9].posX = 267.0f;
	KeyFrame2[9].posY = 0.0f;
	KeyFrame2[9].posZ = 287.0f;
	KeyFrame2[9].movTorzo = -30.0f;
	KeyFrame2[9].movBrazoIzq = 45.0f;
	KeyFrame2[9].rotRodIzq = -24.0f;
	KeyFrame2[9].giroMonito = -3.0f;

	KeyFrame2[10].posX = 269.0f;
	KeyFrame2[10].posY = 0.0f;
	KeyFrame2[10].posZ = 331.0f;
	KeyFrame2[10].movTorzo = -68.0f;
	KeyFrame2[10].movBrazoIzq = -72.0f;
	KeyFrame2[10].rotRodIzq = 30.0f;
	KeyFrame2[10].giroMonito = -3.0f;

	KeyFrame2[11].posX = 271.0f;
	KeyFrame2[11].posY = 0.0f;
	KeyFrame2[11].posZ = 393.0f;
	KeyFrame2[11].movTorzo = -102.0f;
	KeyFrame2[11].movBrazoIzq = 63.0f;
	KeyFrame2[11].rotRodIzq = -18.0f;
	KeyFrame2[11].giroMonito = -40.0f;

	KeyFrame2[12].posX = 248.0f;
	KeyFrame2[12].posY = 0.0f;
	KeyFrame2[12].posZ = 410.0f;
	KeyFrame2[12].movTorzo = -108.0f;
	KeyFrame2[12].movBrazoIzq = -33.0f;
	KeyFrame2[12].rotRodIzq = 27.0f;
	KeyFrame2[12].giroMonito = -72.0f;

	KeyFrame2[13].posX = 165.0f;
	KeyFrame2[13].posY = 0.0f;
	KeyFrame2[13].posZ = 411.0f;
	KeyFrame2[13].movTorzo = -136.0f;
	KeyFrame2[13].movBrazoIzq = 66.0f;
	KeyFrame2[13].rotRodIzq = -24.0f;
	KeyFrame2[13].giroMonito = -93.0f;

	KeyFrame2[14].posX = 122.0f;
	KeyFrame2[14].posY = 0.0f;
	KeyFrame2[14].posZ = 414.0f;
	KeyFrame2[14].movTorzo = -166.0f;
	KeyFrame2[14].movBrazoIzq = -48.0f;
	KeyFrame2[14].rotRodIzq = 24.0f;
	KeyFrame2[14].giroMonito = -116.0f;

	KeyFrame2[15].posX = 67.0f;
	KeyFrame2[15].posY = 0.0f;
	KeyFrame2[15].posZ = 381.0f;
	KeyFrame2[15].movTorzo = -186.0f;
	KeyFrame2[15].movBrazoIzq = 63.0f;
	KeyFrame2[15].rotRodIzq = -31.0f;
	KeyFrame2[15].giroMonito = -116.0f;

	KeyFrame2[16].posX = 13.0f;
	KeyFrame2[16].posY = 0.0f;
	KeyFrame2[16].posZ = 341.0f;
	KeyFrame2[16].movTorzo = -186.0f;
	KeyFrame2[16].movBrazoIzq = -42.0f;
	KeyFrame2[16].rotRodIzq = 28.0f;
	KeyFrame2[16].rotX = 30.0f;
	KeyFrame2[16].giroMonito = -116.0f;

	KeyFrame2[17].posX = -35.0f;
	KeyFrame2[17].posY = 0.0f;
	KeyFrame2[17].posZ = 321.0f;
	KeyFrame2[17].movTorzo = -186.0f;
	KeyFrame2[17].movBrazoIzq = 39.0f;
	KeyFrame2[17].rotRodIzq = -26.0f;
	KeyFrame2[17].rotX = 35.0f;
	KeyFrame2[17].giroMonito = -116.0f;

	KeyFrame2[18].posX = -75.0f;
	KeyFrame2[18].posY = 0.0f;
	KeyFrame2[18].posZ = 283.0f;
	KeyFrame2[18].movTorzo = -186.0f;
	KeyFrame2[18].movBrazoIzq = 39.0f;
	KeyFrame2[18].rotRodIzq = 3.0f;
	KeyFrame2[18].rotX = 35.0f;
	KeyFrame2[18].giroMonito = -116.0f;

	KeyFrame2[19].posX = -75.0f;
	KeyFrame2[19].posY = 0.0f;
	KeyFrame2[19].posZ = 281.0f;
	KeyFrame2[19].movTorzo = -196.0f;
	KeyFrame2[19].movBrazoIzq = -6.0f;
	KeyFrame2[19].rotRodIzq = 3.0f;
	KeyFrame2[19].rotX = 54.0f;
	KeyFrame2[19].giroMonito = -124.0f;

	KeyFrame2[20].posX = -117.0f;
	KeyFrame2[20].posY = 0.0f;
	KeyFrame2[20].posZ = 234.0f;
	KeyFrame2[20].movTorzo = -202.0f;
	KeyFrame2[20].movBrazoIzq = -6.0f;
	KeyFrame2[20].rotRodIzq = 3.0f;
	KeyFrame2[20].rotX = 66.0f;
	KeyFrame2[20].giroMonito = -124.0f;

	KeyFrame2[21].posX = -162.0f;
	KeyFrame2[21].posY = 0.0f;
	KeyFrame2[21].posZ = 194.0f;
	KeyFrame2[21].movTorzo = -210.0f;
	KeyFrame2[21].movBrazoIzq = -6.0f;
	KeyFrame2[21].rotRodIzq = 3.0f;
	KeyFrame2[21].rotX = 77.0f;
	KeyFrame2[21].giroMonito = -124.0f;

	


	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].movTorzo = 0.0f;
	KeyFrame[0].movBrazoIzq = 0.0f;
	KeyFrame[0].rotRodIzq = 0.0f;
	KeyFrame[0].giroMonito = 0.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 0.0f;
	KeyFrame[1].posZ = 18.0f;
	KeyFrame[1].movTorzo = 0.0f;
	KeyFrame[1].movBrazoIzq = -33.0f;
	KeyFrame[1].rotRodIzq = 10.0f;
	KeyFrame[1].giroMonito = 57.0f;

	KeyFrame[2].posX = 33.0f;
	KeyFrame[2].posY = 0.0f;
	KeyFrame[2].posZ = 29.0f;
	KeyFrame[2].movTorzo = 0.0f;
	KeyFrame[2].movBrazoIzq = -69.0f;
	KeyFrame[2].rotRodIzq = 16.0f;
	KeyFrame[2].giroMonito = 73.0f;

	KeyFrame[3].posX = 70.0f;
	KeyFrame[3].posY = 0.0f;
	KeyFrame[3].posZ = 32.0f;
	KeyFrame[3].movTorzo = 0.0f;
	KeyFrame[3].movBrazoIzq = 3.0f;
	KeyFrame[3].rotRodIzq = -10.0f;
	KeyFrame[3].giroMonito = 73.0f;

	KeyFrame[4].posX = 104.0f;
	KeyFrame[4].posY = 0.0f;
	KeyFrame[4].posZ = 32.0f;
	KeyFrame[4].movTorzo = 0.0f;
	KeyFrame[4].movBrazoIzq = 66.0f;
	KeyFrame[4].rotRodIzq = -25.0f;
	KeyFrame[4].giroMonito = 73.0f;

	KeyFrame[5].posX = 146.0f;
	KeyFrame[5].posY = 0.0f;
	KeyFrame[5].posZ = 54.0f;
	KeyFrame[5].movTorzo = 0.0f;
	KeyFrame[5].movBrazoIzq = -18.0f;
	KeyFrame[5].rotRodIzq = 10.0f;
	KeyFrame[5].giroMonito = 42.0f;

	KeyFrame[6].posX = 189.0f;
	KeyFrame[6].posY = 0.0f;
	KeyFrame[6].posZ = 70.0f;
	KeyFrame[6].movTorzo = 0.0f;
	KeyFrame[6].movBrazoIzq = -60.0f;
	KeyFrame[6].rotRodIzq = 20.0f;
	KeyFrame[6].giroMonito = 42.0f;

	KeyFrame[7].posX = 250.0f;
	KeyFrame[7].posY = 0.0f;
	KeyFrame[7].posZ = 101.0f;
	KeyFrame[7].movTorzo = 0.0f;
	KeyFrame[7].movBrazoIzq = 33.0f;
	KeyFrame[7].rotRodIzq = -23.0f;
	KeyFrame[7].giroMonito = 42.0f;

	KeyFrame[8].posX = 307.0f;
	KeyFrame[8].posY = 0.0f;
	KeyFrame[8].posZ = 100.0f;
	KeyFrame[8].movTorzo = 0.0f;
	KeyFrame[8].movBrazoIzq = 72.0f;
	KeyFrame[8].rotRodIzq = -33.0f;
	KeyFrame[8].giroMonito = 35.0f;

	KeyFrame[9].posX = 341.0f;
	KeyFrame[9].posY = 0.0f;
	KeyFrame[9].posZ = 133.0f;
	KeyFrame[9].movTorzo = 0.0f;
	KeyFrame[9].movBrazoIzq = -15.0f;
	KeyFrame[9].rotRodIzq = 16.0f;
	KeyFrame[9].giroMonito = 2.0f;

	KeyFrame[10].posX = 341.0f;
	KeyFrame[10].posY = 0.0f;
	KeyFrame[10].posZ = 169.0f;
	KeyFrame[10].movTorzo = 0.0f;
	KeyFrame[10].movBrazoIzq = -60.0f;
	KeyFrame[10].rotRodIzq = 24.0f;
	KeyFrame[10].giroMonito = 2.0f;

	KeyFrame[11].posX = 341.0f;
	KeyFrame[11].posY = 0.0f;
	KeyFrame[11].posZ = 202.0f;
	KeyFrame[11].movTorzo = -4.0f;
	KeyFrame[11].movBrazoIzq = 30.0f;
	KeyFrame[11].rotRodIzq = -19.0f;
	KeyFrame[11].giroMonito = 2.0f;

	KeyFrame[12].posX = 341.0f;
	KeyFrame[12].posY = 0.0f;
	KeyFrame[12].posZ = 237.0f;
	KeyFrame[12].movTorzo = -22.0f;
	KeyFrame[12].movBrazoIzq = 78.0f;
	KeyFrame[12].rotRodIzq = -30.0f;
	KeyFrame[12].giroMonito = 2.0f;

	KeyFrame[13].posX = 341.0f;
	KeyFrame[13].posY = 0.0f;
	KeyFrame[13].posZ = 277.0f;
	KeyFrame[13].movTorzo = -46.0f;
	KeyFrame[13].movBrazoIzq = -33.0f;
	KeyFrame[13].rotRodIzq = 21.0f;
	KeyFrame[13].giroMonito = 2.0f;

	KeyFrame[14].posX = 341.0f;
	KeyFrame[14].posY = 0.0f;
	KeyFrame[14].posZ = 322.0f;
	KeyFrame[14].movTorzo = -66.0f;
	KeyFrame[14].movBrazoIzq = -66.0f;
	KeyFrame[14].rotRodIzq = 29.0f;
	KeyFrame[14].giroMonito = 2.0f;

	KeyFrame[15].posX = 341.0f;
	KeyFrame[15].posY = 0.0f;
	KeyFrame[15].posZ = 363.0f;
	KeyFrame[15].movTorzo = -92.0f;
	KeyFrame[15].movBrazoIzq = 27.0f;
	KeyFrame[15].rotRodIzq = -30.0f;
	KeyFrame[15].giroMonito = 2.0f;

	KeyFrame[16].posX = 341.0f;
	KeyFrame[16].posY = 0.0f;
	KeyFrame[16].posZ = 407.0f;
	KeyFrame[16].movTorzo = -104.0f;
	KeyFrame[16].movBrazoIzq = -33.0f;
	KeyFrame[16].rotRodIzq = 24.0f;
	KeyFrame[16].giroMonito = -45.0f;

	KeyFrame[17].posX = 264.0f;
	KeyFrame[17].posY = 0.0f;
	KeyFrame[17].posZ = 408.0f;
	KeyFrame[17].movTorzo = -126.0f;
	KeyFrame[17].movBrazoIzq = -72.0f;
	KeyFrame[17].rotRodIzq = 36.0f;
	KeyFrame[17].giroMonito = -90.0f;

	KeyFrame[18].posX = 221.0f;
	KeyFrame[18].posY = 0.0f;
	KeyFrame[18].posZ = 413.0f;
	KeyFrame[18].movTorzo = -150.0f;
	KeyFrame[18].movBrazoIzq = -6.0f;
	KeyFrame[18].rotRodIzq = -25.0f;
	KeyFrame[18].giroMonito = -90.0f;

	KeyFrame[19].posX = 161.0f;
	KeyFrame[19].posY = 0.0f;
	KeyFrame[19].posZ = 395.0f;
	KeyFrame[19].movTorzo = -160.0f;
	KeyFrame[19].movBrazoIzq = 45.0f;
	KeyFrame[19].rotRodIzq = 20.0f;
	KeyFrame[19].giroMonito = -130.0f;

	KeyFrame[20].posX = 83.0f;
	KeyFrame[20].posY = 0.0f;
	KeyFrame[20].posZ = 333.0f;
	KeyFrame[20].movTorzo = -180.0f;
	KeyFrame[20].movBrazoIzq = 87.0f;
	KeyFrame[20].rotRodIzq = -41.0f;
	KeyFrame[20].giroMonito = -130.0f;

	KeyFrame[21].posX = 21.0f;
	KeyFrame[21].posY = 0.0f;
	KeyFrame[21].posZ = 271.0f;
	KeyFrame[21].movTorzo = -180.0f;
	KeyFrame[21].movBrazoIzq = -27.0f;
	KeyFrame[21].rotRodIzq = 15.0f;
	KeyFrame[21].giroMonito = -130.0f;

	KeyFrame[22].posX = 21.0f;
	KeyFrame[22].posY = 0.0f;
	KeyFrame[22].posZ = 271.0f;
	KeyFrame[22].movTorzo = -180.0f;
	KeyFrame[22].movBrazoIzq = -27.0f;
	KeyFrame[22].rotRodIzq = 15.0f;
	KeyFrame[22].giroMonito = -130.0f;
	


	//While -------------------------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)){
		skyboxShader.setInt("skybox", 0);
		lastFrame = SDL_GetTicks();
		animate();
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Luces -------------------------------------------------------------------------------------------------------------------------
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

		//Escenario -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		cascaron.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		escalera.Draw(staticShader);

		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);
		
		
		
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
		*/

		//Stewie
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f, 40.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(posX, movTorzo, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);//Torso
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);//Pierna Der
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(-movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);//Brazo izquierdo 

		//Brian
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-75.0f, 45.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(posX2, movTorzo2, posZ2));
		model = model = glm::rotate(model, glm::radians(giroMonito2), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		torso2.Draw(staticShader);//Torso
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer2.Draw(staticShader);//Pierna Der
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotRodIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq2.Draw(staticShader);//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(-movBrazoIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer2.Draw(staticShader);//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq2), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq2.Draw(staticShader);//Brazo izquierdo 

		//*/
		//piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		//Termina Escenario -------------------------------------------------------------------------------------------------------------------------

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
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		giroMonito++;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		rotX--;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		rotX++;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movBrazoIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		movBrazoIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		movTorzo += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
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