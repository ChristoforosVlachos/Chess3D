// Include C/C++ headers
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>

// Include OS header
#include <Windows.h> // for PlaySound()
#include <Shlobj.h>  // for SHGetKnownFolderPath()

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h>
#include <SOIL.h>

// Include header files for classes
#include "Piece.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

// Include ImGui and related libs
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "external/ImGuiFileDialog-0.6.4/ImGuiFileDialog.h"

using namespace std;
using namespace glm;

struct glsl_type_set {
	GLenum      type;
	const char* name;
}
type_set[] = {
  GL_INVALID_ENUM,                              "invalid",
  GL_FLOAT,                                     "float",
  GL_FLOAT_VEC2,                                "vec2",
  GL_FLOAT_VEC3,                                "vec3",
  GL_FLOAT_VEC4,                                "vec4",
  GL_DOUBLE,                                    "double",
  GL_DOUBLE_VEC2,                               "dvec2",
  GL_DOUBLE_VEC3,                               "dvec3",
  GL_DOUBLE_VEC4,                               "dvec4",
  GL_INT,                                       "int",
  GL_INT_VEC2,                                  "ivec2",
  GL_INT_VEC3,                                  "ivec3",
  GL_INT_VEC4,                                  "ivec4",
  GL_UNSIGNED_INT,                              "unsigned int",
  GL_UNSIGNED_INT_VEC2,                         "uvec2",
  GL_UNSIGNED_INT_VEC3,                         "uvec3",
  GL_UNSIGNED_INT_VEC4,                         "uvec4",
  GL_BOOL,                                      "bool",
  GL_BOOL_VEC2,                                 "bvec2",
  GL_BOOL_VEC3,                                 "bvec3",
  GL_BOOL_VEC4,                                 "bvec4",
  GL_FLOAT_MAT2,                                "mat2",
  GL_FLOAT_MAT3,                                "mat3",
  GL_FLOAT_MAT4,                                "mat4",
  GL_FLOAT_MAT2x3,                              "mat2x3",
  GL_FLOAT_MAT2x4,                              "mat2x4",
  GL_FLOAT_MAT3x2,                              "mat3x2",
  GL_FLOAT_MAT3x4,                              "mat3x4",
  GL_FLOAT_MAT4x2,                              "mat4x2",
  GL_FLOAT_MAT4x3,                              "mat4x3",
  GL_DOUBLE_MAT2,                               "dmat2",
  GL_DOUBLE_MAT3,                               "dmat3",
  GL_DOUBLE_MAT4,                               "dmat4",
  GL_DOUBLE_MAT2x3,                             "dmat2x3",
  GL_DOUBLE_MAT2x4,                             "dmat2x4",
  GL_DOUBLE_MAT3x2,                             "dmat3x2",
  GL_DOUBLE_MAT3x4,                             "dmat3x4",
  GL_DOUBLE_MAT4x2,                             "dmat4x2",
  GL_DOUBLE_MAT4x3,                             "dmat4x3",
  GL_SAMPLER_1D,                                "sampler1D",
  GL_SAMPLER_2D,                                "sampler2D",
  GL_SAMPLER_3D,                                "sampler3D",
  GL_SAMPLER_CUBE,                              "samplerCube",
  GL_SAMPLER_1D_SHADOW,                         "sampler1DShadow",
  GL_SAMPLER_2D_SHADOW,                         "sampler2DShadow",
  GL_SAMPLER_1D_ARRAY,                          "sampler1DArray",
  GL_SAMPLER_2D_ARRAY,                          "sampler2DArray",
  GL_SAMPLER_1D_ARRAY_SHADOW,                   "sampler1DArrayShadow",
  GL_SAMPLER_2D_ARRAY_SHADOW,                   "sampler2DArrayShadow",
  GL_SAMPLER_2D_MULTISAMPLE,                    "sampler2DMS",
  GL_SAMPLER_2D_MULTISAMPLE_ARRAY,              "sampler2DMSArray",
  GL_SAMPLER_CUBE_SHADOW,                       "samplerCubeShadow",
  GL_SAMPLER_BUFFER,                            "samplerBuffer",
  GL_SAMPLER_2D_RECT,                           "sampler2DRect",
  GL_SAMPLER_2D_RECT_SHADOW,                    "sampler2DRectShadow",
  GL_INT_SAMPLER_1D,                            "isampler1D",
  GL_INT_SAMPLER_2D,                            "isampler2D",
  GL_INT_SAMPLER_3D,                            "isampler3D",
  GL_INT_SAMPLER_CUBE,                          "isamplerCube",
  GL_INT_SAMPLER_1D_ARRAY,                      "isampler1DArray",
  GL_INT_SAMPLER_2D_ARRAY,                      "isampler2DArray",
  GL_INT_SAMPLER_2D_MULTISAMPLE,                "isampler2DMS",
  GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,          "isampler2DMSArray",
  GL_INT_SAMPLER_BUFFER,                        "isamplerBuffer",
  GL_INT_SAMPLER_2D_RECT,                       "isampler2DRect",
  GL_UNSIGNED_INT_SAMPLER_1D,                   "usampler1D",
  GL_UNSIGNED_INT_SAMPLER_2D,                   "usampler2D",
  GL_UNSIGNED_INT_SAMPLER_3D,                   "usampler3D",
  GL_UNSIGNED_INT_SAMPLER_CUBE,                 "usamplerCube",
  GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,             "usampler2DArray",
  GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,             "usampler2DArray",
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,       "usampler2DMS",
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "usampler2DMSArray",
  GL_UNSIGNED_INT_SAMPLER_BUFFER,               "usamplerBuffer",
  GL_UNSIGNED_INT_SAMPLER_2D_RECT,              "usampler2DRect",
  GL_IMAGE_1D,                                  "image1D",
  GL_IMAGE_2D,                                  "image2D",
  GL_IMAGE_3D,                                  "image3D",
  GL_IMAGE_2D_RECT,                             "image2DRect",
  GL_IMAGE_CUBE,                                "imageCube",
  GL_IMAGE_BUFFER,                              "imageBuffer",
  GL_IMAGE_1D_ARRAY,                            "image1DArray",
  GL_IMAGE_2D_ARRAY,                            "image2DArray",
  GL_IMAGE_2D_MULTISAMPLE,                      "image2DMS",
  GL_IMAGE_2D_MULTISAMPLE_ARRAY,                "image2DMSArray",
  GL_INT_IMAGE_1D,                              "iimage1D",
  GL_INT_IMAGE_2D,                              "iimage2D",
  GL_INT_IMAGE_3D,                              "iimage3D",
  GL_INT_IMAGE_2D_RECT,                         "iimage2DRect",
  GL_INT_IMAGE_CUBE,                            "iimageCube",
  GL_INT_IMAGE_BUFFER,                          "iimageBuffer",
  GL_INT_IMAGE_1D_ARRAY,                        "iimage1DArray",
  GL_INT_IMAGE_2D_ARRAY,                        "iimage2DArray",
  GL_INT_IMAGE_2D_MULTISAMPLE,                  "iimage2DMS",
  GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,            "iimage2DMSArray",
  GL_UNSIGNED_INT_IMAGE_1D,                     "uimage1D",
  GL_UNSIGNED_INT_IMAGE_2D,                     "uimage2D",
  GL_UNSIGNED_INT_IMAGE_3D,                     "uimage3D",
  GL_UNSIGNED_INT_IMAGE_2D_RECT,                "uimage2DRect",
  GL_UNSIGNED_INT_IMAGE_CUBE,                   "uimageCube",
  GL_UNSIGNED_INT_IMAGE_BUFFER,                 "uimageBuffer",
  GL_UNSIGNED_INT_IMAGE_1D_ARRAY,               "uimage1DArray",
  GL_UNSIGNED_INT_IMAGE_2D_ARRAY,               "uimage2DArray",
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,         "uimage2DMS",
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,   "uimage2DMSArray",
  GL_UNSIGNED_INT_ATOMIC_COUNTER,               "atomic_uint"
};

// Creating a structure to store the material parameters of an object
struct Material {
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float Ns;
};

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

void uploadLight(const Light& light, int lightId);
void uploadMaterial(const Material& mtl);
void shiny_pass(mat4 viewMatrix, mat4 projectionMatrix, unsigned int pieceSkip);
void water_pass(mat4 viewMatrix, mat4 projectionMatrix);
void depth_pass(vector<mat4> shadowTransforms, int lightId);
void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix);
void renderShiny();
void startNewGame();
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void confirmSelection();
void finalizeMove(string type);
void switchCameraSide();
void cleanupLegalMoves(vector<struct move>& moves, Piece* p);
bool testForCheck(Piece* selPiece, struct move m, int faction);
void dumpCurrentGame(ofstream& fout);
void loadGameFromFile(ifstream& fin);
unsigned int loadCubemap(vector<std::string> faces);
void lockSelector();
void unlockSelector();
void selectWithRayCasting();
void renderHelpingWindow();

// Constants
#define W_WIDTH 1280
#define W_HEIGHT 720
#define TITLE "Chess3D"

#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT 2048

#define REFLECTION_WIDTH 320
#define REFLECTION_HEIGHT 180

#define SHINY_WIDTH 128
#define SHINY_HEIGHT 128

#define MAX_LIGHTS 4

// Global Variables
GLFWwindow* window;
Camera* camera;
Light* light[MAX_LIGHTS];
GLuint shaderProgram, depthProgram, selectorProgram, skyboxProgram, waterProgram;
GLuint boardDiffuseTexture;
GLuint depthFrameBuffer[MAX_LIGHTS];
GLuint depthCubemap[MAX_LIGHTS];
GLuint skyboxVAO, skyboxVBO, cubemapTexture;
GLuint morphVAO, pawnVerticesVBO, pawnNormalsVBO, pawnUVsVBO, queenVerticesVBO, queenNormalsVBO, queenUVsVBO;
GLuint waterReflectionFrameBuffer;
GLuint waterReflectionColorTexture, waterReflectionDepthTexture;
GLuint waterTexture, dudvMapTexture;
GLuint pieceFrameBuffer;
GLuint pieceCubemap[32];
GLuint pieceDepthBuffer[32];

Drawable* board_border_model, * board_grid_model, * rook_model, * knight_model;
Drawable* bishop_model, * queen_model, * king_model, * pawn_model, * selector_model;
Drawable* rook_model_low, * knight_model_low, * bishop_model_low;
Drawable* queen_model_low, * king_model_low, * pawn_model_low;
Drawable* lightSphere;

vector<Piece*> gameBoard;
int selCol = 1, selRow = 1;
Piece* selPiece = NULL;
int playersTurn = 1; // 1->white, 2->black
bool player1InCheck = false, player2InCheck = false;
float sphereScale = 0.1;
vector<struct move> legalMoves;
bool shouldDisplaySphereOnLight = false;
bool selectorLocked = false;
int capture_index = 0;
int castle_index = -1;
bool promoting = false;
int lightsCount = 1;
int selectedLight = 0;
bool enableTranslucent = false;
bool enableWater = false;
float moveFactor = 0;
double lastX = W_WIDTH / 2.0f;
double lastY = W_HEIGHT / 2.0f;
int reflectRefractCoeff = 5;
vec4 player1_color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
vec4 player2_color = vec4(0.12f, 0.12f, 0.12f, 1.0f);
vec4 square1_color = vec4(0.55f, 0.55f, 0.55f, 1.0f);
vec4 square2_color = vec4(0.1f, 0.1f, 0.1f, 1.0f);

wstring saveFilesLocation;

// locations for shaderProgram
GLuint viewMatrixLocation;
GLuint projectionMatrixLocation;
GLuint modelMatrixLocation;
GLuint KaLocation, KdLocation, KsLocation, NsLocation;
GLuint LaLocation[MAX_LIGHTS], LdLocation[MAX_LIGHTS], LsLocation[MAX_LIGHTS];
GLuint lightPositionLocation[MAX_LIGHTS];
GLuint lightPowerLocation[MAX_LIGHTS];
GLuint diffuseColorSampler; 
GLuint useTextureLocation;
GLuint depthMapSampler[MAX_LIGHTS];
GLuint lightFarPlaneLocation[MAX_LIGHTS];
GLuint viewPosLocation;
GLuint deleteHeightLocation;
GLuint topLocation;
GLuint bottomLocation;
GLuint lightsCountLocation;
GLuint cameraPositionLocation;
GLuint enviroMapLocation;
GLuint isTranslucentLocation;
GLuint refractionCoeffLocation;
GLuint morphFactorLocation;

// locations for depthProgram
GLuint depthMLocation;
GLuint shadowMatricesLocation;
GLuint lightPosLocation;
GLuint farPlaneLocation;

// locations for selectorProgram
GLuint selectorMVPLocation;
GLuint selectorModeLocation;

// locations for skyboxProgram
GLuint skyboxViewLocation;
GLuint skyboxProjectionLocation;
GLuint skyboxSampler;

// locations for waterProgram
GLuint waterMLocation;
GLuint waterVLocation;
GLuint waterPLocation;
GLuint waterTextureLocation;
GLuint reflectionSampler;
GLuint dudvMapLocation;
GLuint moveFactorLocation;

// Create materials
Material Piece1{
	vec4{0.3f * vec3(player1_color), 1.0f},
	player1_color,
	vec4{0.7, 0.7, 0.7, 1.0},
	32.0f
};

Material Piece2{
	vec4{0.3f * vec3(player2_color), 1.0f},
	player2_color,
	vec4{0.7, 0.7, 0.7, 1.0},
	32.0f
};

Material Square1{
	vec4{0.3f * vec3(square1_color), 1.0f},
	square1_color,
	square1_color,
	1.0f
};

Material Square2{
	vec4{0.3f * vec3(square2_color), 1.0f},
	square2_color,
	square2_color,
	1.0f
};

Material LightSphereMtl{
	vec4{1.0, 1.0, 1.0, 1.0},
	vec4{1.0, 1.0, 1.0, 1.0},
	vec4{1.0, 1.0, 1.0, 1.0},
	1.0f
};


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


void eTB_GLSL__print_uniforms(GLuint program) {
	GLint uniform_count;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

	GLchar name[256];

	for (GLint i = 0; i < uniform_count; i++) {
		memset(name, '\0', 256);
		GLint  size;
		GLenum type;

		glGetActiveUniform(program, i, 255, NULL, &size, &type, name);

		GLint location = glGetUniformLocation(program, name);

		for (int j = 0; j < sizeof(type_set) / sizeof(glsl_type_set); j++) {
			if (type_set[j].type != type)
				continue;

			const char* type_name = type_set[j].name;

			if (size > 1)
				printf("Uniform %d (loc=%d):\t%20s %-20s <Size: %d>\n",
					i, location, type_name, name, size);
			else
				printf("Uniform %d (loc=%d):\t%20s %-20s\n",
					i, location, type_name, name);

			break;
		}

		if (i == (uniform_count - 1))
			printf("\n");
	}
}


// Creating a function to upload (make uniform) the light parameters to the shader program
void uploadLight(const Light& light, int lightId) {
	glUniform4f(LaLocation[lightId], light.La.r, light.La.g, light.La.b, light.La.a);
	glUniform4f(LdLocation[lightId], light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
	glUniform4f(LsLocation[lightId], light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
	glUniform3f(lightPositionLocation[lightId], light.lightPosition_worldspace.x,
		light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
	glUniform1f(lightPowerLocation[lightId], light.power);
	glUniform1f(lightFarPlaneLocation[lightId], light.farPlane);
}

// Creating a function to upload the material parameters of a model to the shader program
void uploadMaterial(const Material& mtl) {
	glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
	glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
	glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
	glUniform1f(NsLocation, mtl.Ns);
}

void createContext() {
	// Create and complile our GLSL programs from the shaders
	shaderProgram = loadShaders("resources/shaders/ShadowMapping.vertexshader", "resources/shaders/ShadowMapping.fragmentshader");
	selectorProgram = loadShaders("resources/shaders/Selector.vertexshader", "resources/shaders/Selector.fragmentshader");
	depthProgram = loadShaders("resources/shaders/Depth.vertexshader", "resources/shaders/Depth.fragmentshader", "resources/shaders/Depth.geometryshader");
	skyboxProgram = loadShaders("resources/shaders/Skybox.vertexshader", "resources/shaders/Skybox.fragmentshader");
	waterProgram = loadShaders("resources/shaders/Water.vertexshader", "resources/shaders/Water.fragmentshader");

	// Draw wire frame triangles or fill: GL_LINE, or GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Get pointers to uniforms

	// --- shaderProgram ---
	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	cameraPositionLocation = glGetUniformLocation(shaderProgram, "cameraPosition");
	isTranslucentLocation = glGetUniformLocation(shaderProgram, "isTranslucent");
	deleteHeightLocation = glGetUniformLocation(shaderProgram, "deleteHeight");
	topLocation = glGetUniformLocation(shaderProgram, "top");
	bottomLocation = glGetUniformLocation(shaderProgram, "bottom");
	lightsCountLocation = glGetUniformLocation(shaderProgram, "lights_count");
	enviroMapLocation = glGetUniformLocation(shaderProgram, "enviroMap");
	refractionCoeffLocation = glGetUniformLocation(shaderProgram, "refractionCoeff");
	morphFactorLocation = glGetUniformLocation(shaderProgram, "morphFactor");

	// for phong lighting
	KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
	KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
	KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
	NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");
	for (int i = 0; i < MAX_LIGHTS; i++) {
		LaLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].La").c_str());
		LdLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].Ld").c_str());
		LsLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].Ls").c_str());
		lightPositionLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].lightPosition_worldspace").c_str());
		lightPowerLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].power").c_str());
		lightFarPlaneLocation[i] = glGetUniformLocation(shaderProgram, ("light[" + to_string(i) + "].far_plane").c_str());
	}
	diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture"); 

	// locations for shadow rendering
	for (int i = 0; i < MAX_LIGHTS; i++)
		depthMapSampler[i] = glGetUniformLocation(shaderProgram, ("depthMap[" + to_string(i) + "]").c_str());
	viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
	
	// --- depthProgram ---
	depthMLocation = glGetUniformLocation(depthProgram, "M");
	shadowMatricesLocation = glGetUniformLocation(depthProgram, "shadowMatrices");
	lightPosLocation = glGetUniformLocation(depthProgram, "lightPos");
	farPlaneLocation = glGetUniformLocation(depthProgram, "far_plane");

	// --- selectorProgram ---
	selectorMVPLocation = glGetUniformLocation(selectorProgram, "MVP");
	selectorModeLocation = glGetUniformLocation(selectorProgram, "mode");

	// --- skyboxProgram ---
	skyboxViewLocation = glGetUniformLocation(skyboxProgram, "V");
	skyboxProjectionLocation = glGetUniformLocation(skyboxProgram, "P");
	skyboxSampler = glGetUniformLocation(skyboxProgram, "skyboxSampler");

	// --- waterProgram ---
	waterMLocation = glGetUniformLocation(waterProgram, "modelMatrix");
	waterVLocation = glGetUniformLocation(waterProgram, "viewMatrix");
	waterPLocation = glGetUniformLocation(waterProgram, "projectionMatrix");
	waterTextureLocation = glGetUniformLocation(waterProgram, "waterTexture");
	reflectionSampler = glGetUniformLocation(waterProgram, "reflectionTexture");
	dudvMapLocation = glGetUniformLocation(waterProgram, "dudvMap");
	moveFactorLocation = glGetUniformLocation(waterProgram, "moveFactor");

	// Load textures
	boardDiffuseTexture = loadSOIL("resources/textures/board.bmp");
	waterTexture = loadSOIL("resources/textures/water.png");
	dudvMapTexture = loadSOIL("resources/textures/waterDUDV.png");


	// Load models

	// 1. Board border
	board_border_model = new Drawable("resources/models/border.obj");

	// 2. Board grid (squares)
	vector<vec3> boardGridVertices = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, -1.0f)
	};
	vector<vec2> boardGridUVs = {
		vec2(0.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f)
	};
	vector<vec3> boardGridNormals = {
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	};

	board_grid_model = new Drawable(boardGridVertices, boardGridUVs, boardGridNormals);
	
	// 3. Chess piece models
	rook_model = new Drawable("resources/models/rook.obj");
	knight_model = new Drawable("resources/models/knight.obj");
	bishop_model = new Drawable("resources/models/bishop.obj");
	queen_model = new Drawable("resources/models/queen.obj");
	king_model = new Drawable("resources/models/king.obj");
	pawn_model = new Drawable("resources/models/pawn.obj");

	// 4. Low-poly versions of previous models
	rook_model_low = new Drawable("resources/models/lowpoly/rook_0.05.obj");
	knight_model_low = new Drawable("resources/models/lowpoly/knight_0.05.obj");
	bishop_model_low = new Drawable("resources/models/lowpoly/bishop_0.05.obj");
	queen_model_low = new Drawable("resources/models/lowpoly/queen_0.05.obj");
	king_model_low = new Drawable("resources/models/lowpoly/king_0.05.obj");
	pawn_model_low = new Drawable("resources/models/lowpoly/pawn_0.05.obj");

	// 5. A square to be used as a selector
	vector<vec3> selectorVertices = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, -1.0f)
	};

	selector_model = new Drawable(selectorVertices);

	// 6. Sphere to display on lights
	// We'll have to invert its normals to make it shine from the light insdide
	vector<vec3> sphereVertices, sphereNormals;
	vector<vec2> sphereUVs;
	loadOBJWithTiny("resources/models/sphere.obj", sphereVertices, sphereUVs, sphereNormals);
	for (int i = 0; i < sphereNormals.size(); i++) {
		sphereNormals[i] *= -1.0f;
	}
	
	lightSphere = new Drawable(sphereVertices, sphereUVs, sphereNormals);

	// 7. Skybox
	float skyboxVertices[] = {        
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<string> faces{
		"resources/textures/skybox/right.jpg",
		"resources/textures/skybox/left.jpg",
		"resources/textures/skybox/top.jpg",
		"resources/textures/skybox/bottom.jpg",
		"resources/textures/skybox/front.jpg",
		"resources/textures/skybox/back.jpg"
	};
	cubemapTexture = loadCubemap(faces);

	// For morphing
	vector<vec3> pawnVertices, pawnNormals;
	vector<vec2> pawnUVs;
	vector<unsigned int> pawnIndices;
	loadOBJWithTiny("resources/models/pawn.obj", pawnVertices, pawnUVs, pawnNormals, pawnIndices);

	vector<vec3> queenVertices, queenNormals;
	vector<vec2> queenUVs;
	vector<unsigned int> queenIndices;
	loadOBJWithTiny("resources/models/queen_like_pawn.obj", queenVertices, queenUVs, queenNormals, queenIndices);

	glGenVertexArrays(1, &morphVAO);
	glBindVertexArray(morphVAO);

	glGenBuffers(1, &pawnVerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pawnVerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, pawnVertices.size() * sizeof(vec3), &pawnVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &pawnNormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pawnNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, pawnNormals.size() * sizeof(vec3), &pawnNormals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &pawnUVsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pawnUVsVBO);
	glBufferData(GL_ARRAY_BUFFER, pawnUVs.size() * sizeof(vec2), &pawnUVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &queenVerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, queenVerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, queenVertices.size() * sizeof(vec3), &queenVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &queenNormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, queenNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, queenNormals.size() * sizeof(vec3), &queenNormals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &queenUVsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, queenUVsVBO);
	glBufferData(GL_ARRAY_BUFFER, queenUVs.size() * sizeof(vec2), &queenUVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(5);


	// Framebuffers etc.

	// 1. Cubemap as a depthmap for point lights
	for (int j = 0; j < MAX_LIGHTS; j++) {
		glGenFramebuffers(1, &depthFrameBuffer[j]);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer[j]);
		glGenTextures(1, &depthCubemap[j]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[j]);
		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[j], 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 2. Cubemap for reflection on chess pieces
	// create the fbo
	glGenFramebuffers(1, &pieceFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, pieceFrameBuffer);

	for (unsigned int j = 0; j < 32; j++) {

		// create the cubemap
		glGenTextures(1, &pieceCubemap[j]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pieceCubemap[j]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// set textures
		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, SHINY_WIDTH, SHINY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// create the uniform depth buffer
		glGenRenderbuffers(1, &pieceDepthBuffer[j]);
		glBindRenderbuffer(GL_RENDERBUFFER, pieceDepthBuffer[j]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHINY_WIDTH, SHINY_HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// attach it
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pieceDepthBuffer[j]); // this might be wrong

		// attach only the +X cubemap texture (for completeness)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, pieceCubemap[j], 0);

		// verify
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Problem creating framebuffer: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << endl;
	}

	// disable
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// 3. Reflection on water surface
	// framebuffer
	glGenFramebuffers(1, &waterReflectionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// color texture
	glGenTextures(1, &waterReflectionColorTexture);
	glBindTexture(GL_TEXTURE_2D, waterReflectionColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, waterReflectionColorTexture, 0);

	// depth texture
	glGenTextures(1, &waterReflectionDepthTexture);
	glBindTexture(GL_TEXTURE_2D, waterReflectionDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, waterReflectionDepthTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void free() {
	// Delete Shader Programs
	glDeleteProgram(shaderProgram);
	glDeleteProgram(selectorProgram);
	glDeleteProgram(depthProgram);
	glDeleteProgram(skyboxProgram);
	glDeleteProgram(waterProgram);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void shiny_pass(mat4 viewMatrix, mat4 projectionMatrix, unsigned int pieceSkip) {
	// set size of viewport
	glViewport(0, 0, SHINY_WIDTH, SHINY_HEIGHT);
	
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glUniform1i(lightsCountLocation, lightsCount);
	glUniform1i(isTranslucentLocation, 0);

	// uploading the light parameters to the shader program
	for (int i = 0; i < lightsCount; i++) {
		uploadLight(*light[i], i);
		glUniform3f(viewPosLocation, camera->position.x, camera->position.y, camera->position.z);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
		glUniform1i(depthMapSampler[i], i);
	}

	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //

	// creating a model matrix
	mat4 modelMatrix = translate(mat4(), vec3(3.0135, 0, -3.0135)) * scale(mat4(), vec3(0.01f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(MAX_LIGHTS + GL_TEXTURE0);					// Activate texture position
	glBindTexture(GL_TEXTURE_2D, boardDiffuseTexture);			// Assign texture to position 
	glUniform1i(diffuseColorSampler, MAX_LIGHTS);				// Assign sampler to that position


	// Inside the fragment shader, there is an if statement whether to use  
	// the material of an object or sample a texture
	glUniform1i(useTextureLocation, 1);


	// Draw models
	
	// Draw board border
	board_border_model->bind();
	board_border_model->draw();

	// Draw board grid
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			modelMatrix = scale(mat4(), vec3(0.753375f, 0.753375f, 0.753375f)) * translate(mat4(), vec3(i, 0, -j));
			if ((i + j) % 2 == 0) {
				uploadMaterial(Square2);
			}
			else {
				uploadMaterial(Square1);
			}
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			glUniform1i(useTextureLocation, 0);
			board_grid_model->bind();
			board_grid_model->draw();
		}
	}

	// Draw chess pieces
	for (unsigned int i = 0; i < gameBoard.size(); i++) {
		if (i == pieceSkip) continue; // skip the piece whose reflection we're calculating
		Piece* piece = gameBoard[i];
		int row = piece->getRow();
		int col = piece->getColumn();
		int faction = piece->getFaction();

		modelMatrix = piece->getModelMatrix(false);

		if (faction == 1) {
			uploadMaterial(Piece1);
		}
		else {
			modelMatrix = modelMatrix * rotate(mat4(), (float)radians(180.0f), vec3(0, 1, 0));
			uploadMaterial(Piece2);
		}
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniform1i(useTextureLocation, 0);

		if (piece->moving == GET_CAPTURED) {
			glUniform1f(deleteHeightLocation, piece->deleteHeight);
			glUniform1f(topLocation, piece->max_y);
			glUniform1f(bottomLocation, piece->min_y);
		}
		else {
			glUniform1f(deleteHeightLocation, 0);
		}

		// draw low-poly versions of pieces to reduce drawing time
		Drawable* model = piece->getModelLowPoly();
		model->bind();
		model->draw();
	}

	// Draw spheres on lights
	if (shouldDisplaySphereOnLight) {
		for (int i = 0; i < lightsCount; i++) {
			modelMatrix = translate(mat4(), light[i]->lightPosition_worldspace) * scale(mat4(), vec3(sphereScale));
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			uploadMaterial(LightSphereMtl);
			glUniform1i(useTextureLocation, 0);

			lightSphere->bind();
			lightSphere->draw();
		}
	}


	// Draw water
	if (enableWater) {
		glUseProgram(waterProgram);
		glUniform1f(moveFactorLocation, moveFactor);

		glActiveTexture(GL_TEXTURE0);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, waterTexture);					// Assign texture to position 
		glUniform1i(waterTextureLocation, 0);						// Assign sampler to that position

		glActiveTexture(GL_TEXTURE1);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, waterReflectionColorTexture);	// Assign texture to position 
		glUniform1i(reflectionSampler, 1);							// Assign sampler to that position

		glActiveTexture(GL_TEXTURE2);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, dudvMapTexture);				// Assign texture to position 
		glUniform1i(dudvMapLocation, 2);							// Assign sampler to that position

		modelMatrix = translate(mat4(), vec3(0, 0.005, 0)) * scale(mat4(), vec3(6.027, 0, 6.027));
		glUniformMatrix4fv(waterMLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(waterVLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(waterPLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		board_grid_model->bind();
		board_grid_model->draw();
	}

	// Draw skybox last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyboxProgram);
	mat4 viewMatrixSkybox = mat4(mat3(viewMatrix)); // remove translation from the view matrix
	glUniformMatrix4fv(skyboxViewLocation, 1, GL_FALSE, &viewMatrixSkybox[0][0]);
	glUniformMatrix4fv(skyboxProjectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(skyboxSampler, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

}

void water_pass(mat4 viewMatrix, mat4 projectionMatrix) {
	glEnable(GL_CLIP_DISTANCE0); // enable clipping to only draw above the water level

	// Step 1: Binding a frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBuffer);
	glViewport(0, 0, REFLECTION_WIDTH, REFLECTION_HEIGHT);

	// Step 2: Clearing color and depth info
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step 3: Selecting shader program
	glUseProgram(shaderProgram);

	// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glUniform1i(lightsCountLocation, lightsCount);
	glUniform1i(isTranslucentLocation, 0);

	// uploading the light parameters to the shader program
	for (int i = 0; i < lightsCount; i++) {
		uploadLight(*light[i], i);
		glUniform3f(viewPosLocation, camera->position.x, camera->position.y, camera->position.z);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
		glUniform1i(depthMapSampler[i], i);
	}


	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //

	// creating a model matrix
	mat4 modelMatrix = translate(mat4(), vec3(3.0135, 0, -3.0135)) * scale(mat4(), vec3(0.01f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(MAX_LIGHTS + GL_TEXTURE0);					// Activate texture position
	glBindTexture(GL_TEXTURE_2D, boardDiffuseTexture);			// Assign texture to position 
	glUniform1i(diffuseColorSampler, MAX_LIGHTS);				// Assign sampler to that position

	// Inside the fragment shader, there is an if statement whether to use  
	// the material of an object or sample a texture
	glUniform1i(useTextureLocation, 1);

	// Draw board border
	board_border_model->bind();
	board_border_model->draw();

	// Draw spheres on lights
	if (shouldDisplaySphereOnLight) {
		for (int i = 0; i < lightsCount; i++) {
			modelMatrix = translate(mat4(), light[i]->lightPosition_worldspace) * scale(mat4(), vec3(sphereScale));
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			uploadMaterial(LightSphereMtl);
			glUniform1i(useTextureLocation, 0);

			lightSphere->bind();
			lightSphere->draw();
		}
	}

	// Draw chess pieces
	for (Piece* piece : gameBoard) {
		int row = piece->getRow();
		int col = piece->getColumn();
		int faction = piece->getFaction();

		modelMatrix = piece->getModelMatrix(false);

		if (faction == 1) {
			uploadMaterial(Piece1);
		}
		else {
			modelMatrix = modelMatrix * rotate(mat4(), (float)radians(180.0f), vec3(0, 1, 0));
			uploadMaterial(Piece2);
		}
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniform1i(useTextureLocation, 0);

		if (piece->moving == GET_CAPTURED) {
			glUniform1f(deleteHeightLocation, piece->deleteHeight);
			glUniform1f(topLocation, piece->max_y);
			glUniform1f(bottomLocation, piece->min_y);
		}
		else {
			glUniform1f(deleteHeightLocation, 0);
		}

		Drawable* model = piece->getModel();
		model->bind();
		model->draw();
	}


	//camera->position.y += distance;
	//camera->verticalAngle *= -1;

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyboxProgram);
	mat4 viewMatrixSkybox = mat4(mat3(viewMatrix)); // remove translation from the view matrix
	glUniformMatrix4fv(skyboxViewLocation, 1, GL_FALSE, &viewMatrixSkybox[0][0]);
	glUniformMatrix4fv(skyboxProjectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(skyboxSampler, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CLIP_DISTANCE0); // this might not work on some drivers :/
}

void depth_pass(vector<mat4> shadowTransforms, int lightId) {
	// Setting viewport to shadow map size
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Binding the depth framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer[lightId]);

	// Cleaning the framebuffer depth information (stored from the last render)
	glClear(GL_DEPTH_BUFFER_BIT);

	// Selecting the new shader program that will output the depth component
	glUseProgram(depthProgram);

	// sending the uniform variables to the shader
	glUniformMatrix4fv(shadowMatricesLocation, 6, GL_FALSE, &shadowTransforms[0][0][0]);
	glUniform1f(farPlaneLocation, light[lightId]->farPlane);
	glUniform3f(lightPosLocation, light[lightId]->lightPosition_worldspace.x, light[lightId]->lightPosition_worldspace.y, light[lightId]->lightPosition_worldspace.z);

	//glUseProgram(shaderProgram);
	// ---- rendering the scene ---- //
	mat4 modelMatrix = translate(mat4(), vec3(3.0135, 0, -3.0135)) * scale(mat4(), vec3(0.01f));
	glUniformMatrix4fv(depthMLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Draw board border
	board_border_model->bind();
	board_border_model->draw();

	// Draw board grid
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			modelMatrix = scale(mat4(), vec3(0.753375f, 0.753375f, 0.753375f)) * translate(mat4(), vec3(i, 0, -j));
			glUniformMatrix4fv(depthMLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			board_grid_model->bind();
			board_grid_model->draw();
		}
	}

	// Draw chess pieces
	for (Piece* piece : gameBoard) {
		int row = piece->getRow();
		int col = piece->getColumn();
		int faction = piece->getFaction();

		modelMatrix = piece->getModelMatrix(false);
		if (faction == 2) {
			modelMatrix = modelMatrix * rotate(mat4(), (float)radians(180.0f), vec3(0, 1, 0));
		}
		glUniformMatrix4fv(depthMLocation, 1, GL_FALSE, &modelMatrix[0][0]);

		Drawable* model = piece->getModel();
		model->bind();
		model->draw();
	}

	modelMatrix = translate(mat4(), vec3(0, 0.005, 0)) * scale(mat4(), vec3(6.027, 0, 6.027));
	glUniformMatrix4fv(depthMLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	board_grid_model->bind();
	board_grid_model->draw();

	// binding the default framebuffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//*/
}

void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {
	// Step 1: Binding a frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, W_WIDTH, W_HEIGHT);

	// Step 2: Clearing color and depth info
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step 3: Selecting shader program
	glUseProgram(shaderProgram);

	//// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glUniform1i(lightsCountLocation, lightsCount);
	glUniform1i(isTranslucentLocation, 0);

	//// uploading the light parameters to the shader program
	for (int i = 0; i < lightsCount; i++) {
		uploadLight(*light[i], i);
		glUniform3f(viewPosLocation, camera->position.x, camera->position.y, camera->position.z);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
		glUniform1i(depthMapSampler[i], i);
	}

	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //
	
	// creating a model matrix
	mat4 modelMatrix = translate(mat4(), vec3(3.0135, 0, -3.0135)) * scale(mat4(), vec3(0.01f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(MAX_LIGHTS + GL_TEXTURE0);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, boardDiffuseTexture);			// Assign texture to position 
	glUniform1i(diffuseColorSampler, MAX_LIGHTS);						// Assign sampler to that position

	// Inside the fragment shader, there is an if statement whether to use  
	// the material of an object or sample a texture
	glUniform1i(useTextureLocation, 1);

	// Draw board border
	board_border_model->bind();
	board_border_model->draw();

	// Draw board grid
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			modelMatrix = scale(mat4(), vec3(0.753375f, 0.753375f, 0.753375f)) * translate(mat4(), vec3(i, 0, -j));
			if ((i + j) % 2 == 0) {
				uploadMaterial(Square2);
			}
			else {
				uploadMaterial(Square1);
			}
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			glUniform1i(useTextureLocation, 0);
			board_grid_model->bind();
			board_grid_model->draw();
		}
	}

	// Draw spheres on lights
	if (shouldDisplaySphereOnLight) {
		for (int i = 0; i < lightsCount; i++) {
			modelMatrix = translate(mat4(), light[i]->lightPosition_worldspace) * scale(mat4(), vec3(sphereScale));
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
			uploadMaterial(LightSphereMtl);
			glUniform1i(useTextureLocation, 0);

			lightSphere->bind();
			lightSphere->draw();
		}
	}

	// Draw chess pieces
	glUniform1i(isTranslucentLocation, enableTranslucent);
	glUniform3f(cameraPositionLocation, camera->position.x, camera->position.y, camera->position.z);
	glUniform1f(refractionCoeffLocation, reflectRefractCoeff / 10.0f);

	for (unsigned int i = 0; i < gameBoard.size(); i++) {
		Piece* piece = gameBoard[i];
		int row = piece->getRow();
		int col = piece->getColumn();
		int faction = piece->getFaction();

		modelMatrix = piece->getModelMatrix(true);

		if (faction == 1) {
			uploadMaterial(Piece1);
		}
		else {
			modelMatrix = modelMatrix * rotate(mat4(), (float)radians(180.0f), vec3(0, 1, 0));
			uploadMaterial(Piece2);
		}
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniform1i(useTextureLocation, 0);

		if (piece->moving == GET_CAPTURED) {
			glUniform1f(deleteHeightLocation, piece->deleteHeight);
			glUniform1f(topLocation, piece->max_y);
			glUniform1f(bottomLocation, piece->min_y);
		}
		else {
			glUniform1f(deleteHeightLocation, 0);
		}

		glActiveTexture(MAX_LIGHTS + GL_TEXTURE1);								// Activate texture position
		glBindTexture(GL_TEXTURE_CUBE_MAP, pieceCubemap[i]);			// Assign texture to position 
		glUniform1i(enviroMapLocation, MAX_LIGHTS + 1);						// Assign sampler to that position

		if (piece->moving == GET_PROMOTED) {
			// draw morph
			glUniform1f(morphFactorLocation, (glfwGetTime() - piece->movementStartTime) / 2);
			glBindVertexArray(morphVAO);
			glDrawArrays(GL_TRIANGLES, 0, 66054);
			glUniform1f(morphFactorLocation, 0);
		}
		else {
			Drawable* model = piece->getModel();
			model->bind();
			model->draw();
		}
	}

	// Draw selector
	glUseProgram(selectorProgram);
	modelMatrix = translate(mat4(), vec3(2 * (selCol - 1) * 0.3766875, 0.001, 2 * (selRow - 1) * -0.3766875)) * scale(mat4(), vec3(0.753375f, 0.753375f, 0.753375f));
	mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(selectorMVPLocation, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(selectorModeLocation, -1);

	selector_model->bind();
	selector_model->draw();

	for (auto i : legalMoves) {
		modelMatrix = translate(mat4(), vec3(2 * (i.column - 1) * 0.3766875, 0.001, 2 * (i.row - 1) * -0.3766875)) * scale(mat4(), vec3(0.753375f, 0.753375f, 0.753375f));
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(selectorMVPLocation, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(selectorModeLocation, i.type);

		selector_model->bind();
		selector_model->draw();
	}

	// Draw water
	if (enableWater) {
		glUseProgram(waterProgram);
		glUniform1f(moveFactorLocation, moveFactor);

		glActiveTexture(GL_TEXTURE0);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, waterTexture);					// Assign texture to position 
		glUniform1i(waterTextureLocation, 0);						// Assign sampler to that position

		glActiveTexture(GL_TEXTURE1);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, waterReflectionColorTexture);	// Assign texture to position 
		glUniform1i(reflectionSampler, 1);							// Assign sampler to that position

		glActiveTexture(GL_TEXTURE2);								// Activate texture position
		glBindTexture(GL_TEXTURE_2D, dudvMapTexture);				// Assign texture to position 
		glUniform1i(dudvMapLocation, 2);							// Assign sampler to that position

		modelMatrix = translate(mat4(), vec3(0, 0.005, 0)) * scale(mat4(), vec3(6.027, 0, 6.027));
		glUniformMatrix4fv(waterMLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(waterVLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(waterPLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		board_grid_model->bind();
		board_grid_model->draw();
	}

	// Draw skybox last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyboxProgram);
	mat4 viewMatrixSkybox = mat4(mat3(viewMatrix)); // remove translation from the view matrix
	glUniformMatrix4fv(skyboxViewLocation, 1, GL_FALSE, &viewMatrixSkybox[0][0]);
	glUniformMatrix4fv(skyboxProjectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(skyboxSampler, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderShiny() {
	glBindFramebuffer(GL_FRAMEBUFFER, pieceFrameBuffer);

	for (unsigned int i = 0; i < gameBoard.size(); i++) {
		vec3 pos = vec3((2 * (gameBoard[i]->getColumn() - 1) + 1) * 0.3766875, 0.5, (2 * (gameBoard[i]->getRow() - 1) + 1) * -0.3766875);

		for (int face = 0; face < 6; face++) {
			// attach new texture and renderbuffer to fbo
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, pieceCubemap[i], 0);

			// clear
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mat4 projection = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
			mat4 view;

			// setup lookat depending on current face
			switch (face) {
			case 0:
				view = lookAt(vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, -1, 0));
				break;
			case 1:
				view = lookAt(vec3(0, 0, 0), vec3(-1, 0, 0), vec3(0, -1, 0));
				break;
			case 2:
				view = lookAt(vec3(0, 0, 0), vec3(0, 10, 0), vec3(0, 0, 1));
				break;
			case 3:
				view = lookAt(vec3(0, 0, 0), vec3(0, -10, 0), vec3(0, 0, -1));
				break;
			case 4:
				view = lookAt(vec3(0, 0, 0), vec3(0, 0, 10), vec3(0, -1, 0));
				break;
			case 5:
				view = lookAt(vec3(0, 0, 0), vec3(0, 0, -10), vec3(0, -1, 0));
				break;

			default:
				cout << "Error!" << endl;
			}
			view = translate(view, -pos);

			shiny_pass(view, projection, i);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mainLoop() {
	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Game Loop
	do {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//glDisable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update colors
		Piece1.Ka = vec4(0.3f * vec3(player1_color), 1.0f);
		Piece1.Kd = player1_color;
		//Piece1.Ks = player1_color;
		Piece1.Ks = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		Piece2.Ka = vec4(0.3f * vec3(player2_color), 1.0f);
		Piece2.Kd = player2_color;
		//Piece2.Ks = player2_color;
		Piece2.Ks = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		Square1.Ka = vec4(0.3f * vec3(square1_color), 1.0f);
		Square1.Kd = square1_color;
		Square1.Ks = square1_color;
		Square2.Ka = vec4(0.3f * vec3(square2_color), 1.0f);
		Square2.Kd = square2_color;
		Square2.Ks = square2_color;

		// depth pass
		for (int i = 0; i < lightsCount; i++) {
			light[i]->update();
			vector<mat4> shadowTransforms = light[i]->shadowTransforms;
			depth_pass(shadowTransforms, i);
		}

		mat4 projectionMatrix;
		mat4 viewMatrix;

		// water pass (for reflection on water surface)
		if (enableWater) {
			// Getting camera information from below (for reflection)
			camera->viewFromBelow();
			camera->update();
			projectionMatrix = camera->projectionMatrix;
			viewMatrix = camera->viewMatrix;

			water_pass(viewMatrix, projectionMatrix);

		}

		camera->update();
		projectionMatrix = camera->projectionMatrix;
		viewMatrix = camera->viewMatrix;

		// shiny pass (for reflection on chess pieces)
		if (enableTranslucent) {
			renderShiny();
		}

		// lighting pass
		lighting_pass(viewMatrix, projectionMatrix);

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// increase move factor (used by the water shader)
		moveFactor += 0.1 * deltaTime;

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

		// finalize movement after completing the animation
		if (selPiece && selPiece->shouldFinalizeMove) {
			if (promoting) {
				finalizeMove("promoting");
			}
			if (castle_index == -1) {
				finalizeMove("normal");
			}
			else if (gameBoard[castle_index]->moving == STOP) {
				castle_index = -1;
			}
		}
		else if (capture_index < gameBoard.size() && gameBoard[capture_index]->shouldFinalizeMove) {
			finalizeMove("capture");
		}

		// inform each light whether it's allowed to move or not
		for (int i = 0; i < lightsCount; i++) {
			light[i]->allowMovement = selectedLight == i;
		}

		// update selector
		if (!selectorLocked) {
			selectWithRayCasting();
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// display ImGui window
		renderHelpingWindow();

		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

}

void initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	glGetError(); // reset error flag (GL_INVALID_ENUM raised by glewInit)

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set window icon
	GLFWimage icons[2];
	icons[0].pixels = SOIL_load_image("resources/textures/icon.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	icons[1].pixels = SOIL_load_image("resources/textures/icon_small.png", &icons[1].width, &icons[1].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 2, icons);
	SOIL_free_image_data(icons[0].pixels);
	SOIL_free_image_data(icons[1].pixels);

	// Hide the mouse and enable unlimited movement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

	// Gray background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Keyboard and mouse calback functions
	glfwSetKeyCallback(window, pollKeyboard);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Task 6.1
	// Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// enable texturing and bind the depth texture
	//glEnable(GL_TEXTURE_2D);

	// Log
	logGLParameters();

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	// Create camera
	camera = new Camera(window);

	// Create lights
	for (int i = 0; i < MAX_LIGHTS; i++) {
		light[i] = new Light(window,
			vec4{ 1, 1, 1, 1 },
			vec4{ 1, 1, 1, 1 },
			vec4{ 1, 1, 1, 1 },
			vec3{ 8, 5, -3.0135 },
			85.0f
		);
	}

	PWSTR localAppData;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &localAppData);
	saveFilesLocation = localAppData + wstring(L"\\Chess3D\\saves");
	filesystem::create_directories(saveFilesLocation);
}

void startNewGame() {
	cout << "Starting new game" << endl;
	gameBoard.clear();
	selPiece = NULL;
	legalMoves.clear();
	if (playersTurn != 1) {
		playersTurn = 1;
		switchCameraSide();
	}
	player1InCheck = false;
	player2InCheck = false;

	Rook* rook1 = new Rook(gameBoard, rook_model, rook_model_low, 1, 1, 1);
	Rook* rook2 = new Rook(gameBoard, rook_model, rook_model_low, 1, 1, 8);
	Knight* knight1 = new Knight(gameBoard, knight_model, knight_model_low, 1, 1, 2);
	Knight* knight2 = new Knight(gameBoard, knight_model, knight_model_low, 1, 1, 7);
	Bishop* bishop1 = new Bishop(gameBoard, bishop_model, bishop_model_low, 1, 1, 3);
	Bishop* bishop2 = new Bishop(gameBoard, bishop_model, bishop_model_low, 1, 1, 6);
	Queen* queen1 = new Queen(gameBoard, queen_model, queen_model_low, 1, 1, 4);
	King* king1 = new King(gameBoard, king_model, king_model_low, 1, 1, 5);
	Pawn* pawn1 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 1);
	Pawn* pawn2 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 2);
	Pawn* pawn3 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 3);
	Pawn* pawn4 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 4);
	Pawn* pawn5 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 5);
	Pawn* pawn6 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 6);
	Pawn* pawn7 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 7);
	Pawn* pawn8 = new Pawn(gameBoard, pawn_model, pawn_model_low, 1, 2, 8);
	//---------------------------------------------------------------------
	Rook* rook3 = new Rook(gameBoard, rook_model, rook_model_low, 2, 8, 1);
	Rook* rook4 = new Rook(gameBoard, rook_model, rook_model_low, 2, 8, 8);
	Knight* knight3 = new Knight(gameBoard, knight_model, knight_model_low, 2, 8, 2);
	Knight* knight4 = new Knight(gameBoard, knight_model, knight_model_low, 2, 8, 7);
	Bishop* bishop3 = new Bishop(gameBoard, bishop_model, bishop_model_low, 2, 8, 3);
	Bishop* bishop4 = new Bishop(gameBoard, bishop_model, bishop_model_low, 2, 8, 6);
	Queen* queen2 = new Queen(gameBoard, queen_model, queen_model_low, 2, 8, 4);
	King* king2 = new King(gameBoard, king_model, king_model_low, 2, 8, 5);
	Pawn* pawn9 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 1);
	Pawn* pawn10 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 2);
	Pawn* pawn11 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 3);
	Pawn* pawn12 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 4);
	Pawn* pawn13 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 5);
	Pawn* pawn14 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 6);
	Pawn* pawn15 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 7);
	Pawn* pawn16 = new Pawn(gameBoard, pawn_model, pawn_model_low, 2, 7, 8);
	unlockSelector();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		lockSelector();
		glfwGetCursorPos(window, &lastX, &lastY);
		camera->allowLookAround = true;
		glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		unlockSelector();
		camera->allowLookAround = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(window, lastX, lastY);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (!selectorLocked) {
			confirmSelection();
		}
	}
}

void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		enableWater = !enableWater;
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		enableTranslucent = !enableTranslucent;
	}
	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		lightsCount = std::max(lightsCount - 1, 1);
		selectedLight = lightsCount - 1;
	}
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
		lightsCount = std::min(lightsCount + 1, MAX_LIGHTS);
		selectedLight = lightsCount - 1;
	}
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
		selectedLight = (selectedLight + 1) % lightsCount;
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS && !(mods & GLFW_MOD_CONTROL)) {
		cout << "Saving state in slot 1...";
		ofstream fout;
		try {
			fout.open(saveFilesLocation + wstring(L"\\save1.state"));
		}
		catch(int e) {
			cout << endl << "Error: Could not create file \"save1.state\"!" << endl;
			return;
		}
		dumpCurrentGame(fout);
		cout << " Success" << endl;
		fout.close();
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS && !(mods & GLFW_MOD_CONTROL)) {
		cout << "Saving state in slot 2...";
		ofstream fout;
		try {
			fout.open(saveFilesLocation + wstring(L"\\save2.state"));
		}
		catch (int e) {
			cout << endl << "Error: Could not create file \"save2.state\"!" << endl;
			return;
		}
		dumpCurrentGame(fout);
		cout << " Success" << endl;
		fout.close();
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS && !(mods & GLFW_MOD_CONTROL)) {
		cout << "Saving state in slot 3...";
		ofstream fout;
		try {
			fout.open(saveFilesLocation + wstring(L"\\save3.state"));
		}
		catch (int e) {
			cout << endl << "Error: Could not create file \"save3.state\"!" << endl;
			return;
		}
		dumpCurrentGame(fout);
		cout << " Success" << endl;
		fout.close();
	}
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS && !(mods & GLFW_MOD_CONTROL)) {
		cout << "Saving state in slot 4...";
		ofstream fout;
		try {
			fout.open(saveFilesLocation + wstring(L"\\save4.state"));
		}
		catch (int e) {
			cout << endl << "Error: Could not create file \"save4.state\"!" << endl;
			return;
		}
		dumpCurrentGame(fout);
		cout << " Success" << endl;
		fout.close();
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
		cout << "Loading state from slot 1...";
		ifstream fin;
		try {
			fin.open(saveFilesLocation + wstring(L"\\save1.state"));
		}
		catch (int e) {
			cout << endl << "Error: File \"save1.state\" could not be loaded!" << endl;
			return;
		}
		if (!fin) {
			cout << "Failed" << endl;
			cout << "Save slot 1 is empty!" << endl;
			return;
		}
		loadGameFromFile(fin);
		cout << " Success" << endl;
		fin.close();
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
		cout << "Loading state from slot 2...";
		ifstream fin;
		try {
			fin.open(saveFilesLocation + wstring(L"\\save2.state"));
		}
		catch (int e) {
			cout << endl << "Error: File \"save2.state\" could not be loaded!" << endl;
			return;
		}
		if (!fin) {
			cout << "Failed" << endl;
			cout << "Save slot 2 is empty!" << endl;
			return;
		}
		loadGameFromFile(fin);
		cout << " Success" << endl;
		fin.close();
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
		cout << "Loading state from slot 3...";
		ifstream fin;
		try {
			fin.open(saveFilesLocation + wstring(L"\\save3.state"));
		}
		catch (int e) {
			cout << endl << "Error: File \"save3.state\" could not be loaded!" << endl;
			return;
		}
		if (!fin) {
			cout << "Failed" << endl;
			cout << "Save slot 3 is empty!" << endl;
			return;
		}
		loadGameFromFile(fin);
		cout << " Success" << endl;
		fin.close();
	}
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
		cout << "Loading state from slot 4...";
		ifstream fin;
		try {
			fin.open(saveFilesLocation + wstring(L"\\save4.state"));
		}
		catch (int e) {
			cout << endl << "Error: File \"save4.state\" could not be loaded!" << endl;
			return;
		}
		if (!fin) {
			cout << "Failed" << endl;
			cout << "Save slot 4 is empty!" << endl;
			return;
		}
		loadGameFromFile(fin);
		cout << " Success" << endl;
		fin.close();
	}
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
		camera->cameraLock = !camera->cameraLock;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL) {
		startNewGame();
	}
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		shouldDisplaySphereOnLight = !shouldDisplaySphereOnLight;
	}
	if (!selectorLocked) {
		//if (playersTurn == 1) {
		//	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		//		if (selCol < 8) {
		//			selCol += 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		//		if (selCol > 1) {
		//			selCol -= 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		//		if (selRow < 8) {
		//			selRow += 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		//		if (selRow > 1) {
		//			selRow -= 1;
		//		}
		//	}
		//}
		//else { // playersTurn == 2
		//	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		//		if (selCol < 8) {
		//			selCol += 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		//		if (selCol > 1) {
		//			selCol -= 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		//		if (selRow < 8) {
		//			selRow += 1;
		//		}
		//	}
		//	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		//		if (selRow > 1) {
		//			selRow -= 1;
		//		}
		//	}
		//}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			confirmSelection();
		}
	}
}

void confirmSelection() {
	if (!selPiece) {
		for (Piece* p : gameBoard) {
			if (p->getColumn() == selCol && p->getRow() == selRow && p->getFaction() == playersTurn) {
				selPiece = p;

				p->getLegalMoves(gameBoard, legalMoves);
				cleanupLegalMoves(legalMoves, selPiece);
				break;
			}
		}
	}
	else {
		if (selRow == legalMoves[0].row && selCol == legalMoves[0].column) {
			selPiece->setColumn(selCol);
			selPiece->setRow(selRow);
			selPiece = NULL;
			legalMoves.clear();
		}
		else {
			for (Piece* p : gameBoard) { // ability to change selected piece without putting previous one down
				if (p->getColumn() == selCol && p->getRow() == selRow && p->getFaction() == playersTurn) {
					selPiece->setColumn(selPiece->getColumn());
					selPiece->setRow(selPiece->getRow());
					selPiece = p;
					legalMoves.clear();
					p->getLegalMoves(gameBoard, legalMoves);
					cleanupLegalMoves(legalMoves, selPiece);
					return;
				}
			}

			for (auto i : legalMoves) {
				if (selRow == i.row && selCol == i.column) { // make the move
					// reset en passant threats
					for (Piece* p : gameBoard) {
						if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
							pawn->canBeCapturedEnPassant = false;
						}
					}

					if (i.type == CAPTURE || i.type == CAPTURE_AND_PROMOTE) {
						for (int j = 0; j < gameBoard.size(); j++) {
							if (i.row == gameBoard[j]->getRow() && i.column == gameBoard[j]->getColumn()) {
								capture_index = j;
								lockSelector();
								(gameBoard[j])->startMoving(GET_CAPTURED);
								return;
							}
						}
					}
					else if (i.type == EN_PASSANT_THREAT) {
						Pawn* pawn = dynamic_cast<Pawn*>(selPiece);
						pawn->canBeCapturedEnPassant = true;
					}
					else if (i.type == EN_PASSANT_CAPTURE) {
						int front = playersTurn == 1 ? 1 : -1;

						for (int j = 0; j < gameBoard.size(); j++) {
							if (i.row - front == gameBoard[j]->getRow() && i.column == gameBoard[j]->getColumn()) {
								capture_index = j;
								lockSelector();
								(gameBoard[j])->startMoving(GET_CAPTURED);
								return;
							}
						}
					}

					if (dynamic_cast<Knight*>(selPiece)) {
						selPiece->startMoving(JUMP);
					}
					else {
						selPiece->startMoving(WALK);
					}

					selPiece->setColumn(selCol);
					selPiece->setRow(selRow);

					if (i.type == CASTLING_KINGSIDE) {
						for (int j = 0; j < gameBoard.size(); j++) {
							if (gameBoard[j]->getRow() == i.row && gameBoard[j]->getColumn() == 8) {
								castle_index = j;
								gameBoard[j]->startMoving(ROOK_CASTLE);
								gameBoard[j]->setColumn(6);
								//gameBoard[j]->setRow(selRow);
								PlaySound("resources/sound/castle.wav", NULL, SND_ASYNC | SND_FILENAME);
								break;
							}
						}
					}
					else if (i.type == CASTLING_QUEENSIDE) {
						for (int j = 0; j < gameBoard.size(); j++) {
							if (gameBoard[j]->getRow() == i.row && gameBoard[j]->getColumn() == 1) {
								castle_index = j;
								gameBoard[j]->startMoving(ROOK_CASTLE);
								gameBoard[j]->setColumn(4);
								//gameBoard[j]->setRow(selRow);
								PlaySound("resources/sound/castle.wav", NULL, SND_ASYNC | SND_FILENAME);
								break;
							}
						}
					}
					else if (i.type == PROMOTE || i.type == CAPTURE_AND_PROMOTE) {
						//for (int j = 0; j < gameBoard.size(); j++) {
						//	if (selRow == gameBoard[j]->getRow() && selCol == gameBoard[j]->getColumn()) {
						//		gameBoard.erase(gameBoard.begin() + j);
						//		break;
						//	}
						//}
						//selPiece = new Queen(gameBoard, queen_model, queen_model_low, playersTurn, selRow, selCol);
						//PlaySound("resources/sound/promote.wav", NULL, SND_ASYNC | SND_FILENAME);
						//selPiece->startMoving(GET_PROMOTED);
						promoting = true;
						lockSelector();
						return;
					}

					if (Pawn* pawn = dynamic_cast<Pawn*>(selPiece)) pawn->hasMoved = true;
					if (Rook* rook = dynamic_cast<Rook*>(selPiece)) rook->hasMoved = true;
					if (King* king = dynamic_cast<King*>(selPiece)) king->hasMoved = true;

					// test whether last move put opponent's king in check
					for (Piece* piece : gameBoard) {
						vector<struct move> potentialMoves;
						piece->getLegalMoves(gameBoard, potentialMoves);
						cleanupLegalMoves(potentialMoves, piece);
						for (auto m : potentialMoves) {
							if (m.type == CAPTURE || m.type == CAPTURE_AND_PROMOTE) {
								for (Piece* p : gameBoard) {
									if (p->getColumn() == m.column && p->getRow() == m.row && dynamic_cast<King*>(p)) {
										PlaySound("resources/sound/check.wav", NULL, SND_ASYNC | SND_FILENAME);
										if (playersTurn == 1) {
											player2InCheck = true;
											cout << "Player 2 in check" << endl;
											goto check;
										}
										else {
											player1InCheck = true;
											cout << "Player 1 in check" << endl;
											goto check;
										}
									}
								}
							}
						}
					}

					if (playersTurn == 1) {
						player2InCheck = false;
					}
					else {
						player1InCheck = false;
					}

				check:
					lockSelector();
					break;
				}
			}
		}
	}
}

void finalizeMove(string type) {
	if (type == "normal") {
		selPiece->shouldFinalizeMove = false;
		unlockSelector();
		selPiece = NULL;
		legalMoves.clear();
		playersTurn = playersTurn % 2 + 1; // 1->2, 2->1
		switchCameraSide();

		// test whether next player has legal moves
		for (Piece* p : gameBoard) {
			if (p->getFaction() == playersTurn) {
				vector<struct move> potentialMoves;
				p->getLegalMoves(gameBoard, potentialMoves);
				cleanupLegalMoves(potentialMoves, p);
				if (potentialMoves.size() > 1) return;
			}
		}
		// if execution gets here no legal moves were found
		if (player1InCheck && playersTurn == 1) {
			PlaySound("resources/sound/checkmate.wav", NULL, SND_ASYNC | SND_FILENAME);
			cout << "Player 2 wins (by checkmate)" << endl;
		}
		else if (player2InCheck && playersTurn == 2) {
			PlaySound("resources/sound/checkmate.wav", NULL, SND_ASYNC | SND_FILENAME);
			cout << "Player 1 wins (by checkmate)" << endl;
		}
		else {
			PlaySound("resources/sound/stalemate.wav", NULL, SND_ASYNC | SND_FILENAME);
			cout << "Game ends in draw (by stalemate)" << endl;
		}
		lockSelector();
		cout << "Use Ctrl+N to start a new game or load a previously saved state." << endl;
	}
	else if (type == "capture") {
		gameBoard.erase(gameBoard.begin() + capture_index);
		if (!promoting) {
			unlockSelector();
		}
		confirmSelection();
	}
	else { // if (type == "promoting")
		lockSelector();
		for (int j = 0; j < gameBoard.size(); j++) {
			if (selRow == gameBoard[j]->getRow() && selCol == gameBoard[j]->getColumn()) {
				gameBoard.erase(gameBoard.begin() + j);
				break;
			}
		}
		selPiece = new Queen(gameBoard, queen_model, queen_model_low, playersTurn, selRow, selCol);
		PlaySound("resources/sound/promote.wav", NULL, SND_ASYNC | SND_FILENAME);
		selPiece->startMoving(GET_PROMOTED);
		promoting = false;
		confirmSelection();
		//finalizeMove("normal");
	}
}

void switchCameraSide() {
	camera->position.z -= 2 * (3.0135 + camera->position.z);
	camera->position.x = 6.027 - camera->position.x;
	camera->changeSide = true;
}

void cleanupLegalMoves(vector<struct move>& moves, Piece* p) {
	// remove moves that would put the king in check
	for (int i = 1; i < moves.size(); i++) { // skip first legal move (RESET)
		if (testForCheck(p, moves[i], playersTurn)) {
			moves.erase(moves.begin() + i);
			i--; // copmensate for deleted element
		}
	}
}

bool testForCheck(Piece* selPiece, struct move m, int faction) {
	int kingRow, kingColumn;

	int prevRow = selPiece->getRow();
	int prevColumn = selPiece->getColumn();

	// cannot castle out of or through check
	if (m.type == CASTLING_KINGSIDE) {
		if (testForCheck(selPiece, { prevRow, prevColumn, NORMAL }, faction) ||
			testForCheck(selPiece, { prevRow, prevColumn + 1, NORMAL }, faction) ||
			testForCheck(selPiece, { prevRow, prevColumn + 2, NORMAL }, faction)) {
			return true;
		}
	}
	else if (m.type == CASTLING_QUEENSIDE) {
		if (testForCheck(selPiece, { prevRow, prevColumn, NORMAL }, faction) ||
			testForCheck(selPiece, { prevRow, prevColumn - 1, NORMAL }, faction) ||
			testForCheck(selPiece, { prevRow, prevColumn - 2, NORMAL }, faction)) {
			return true;
		}
	}
	
	selPiece->setRow(m.row);
	selPiece->setColumn(m.column);

	for (Piece* p : gameBoard) {
		if (p->getFaction() == faction && dynamic_cast<King*>(p)) {
			kingRow = p->getRow();
			kingColumn = p->getColumn();
			break;
		}
	}

	for (Piece* p : gameBoard) {
		if (p->getFaction() == faction) continue;
		if (p->getColumn() == m.column && p->getRow() == m.row) continue; // it would get captured in this move
		vector<struct move> potentialMoves;
		p->getLegalMoves(gameBoard, potentialMoves);
		for (auto i : potentialMoves) {
			if ((i.type == CAPTURE || i.type == CAPTURE_AND_PROMOTE) && i.row == kingRow && i.column == kingColumn) {
				selPiece->setRow(prevRow);
				selPiece->setColumn(prevColumn);
				return true;
			}
		}
	}

	selPiece->setRow(prevRow);
	selPiece->setColumn(prevColumn);
	return false;
}

void dumpCurrentGame(ofstream& fout) {
	fout << playersTurn << player1InCheck << player2InCheck;
	for (Piece* p : gameBoard) {
		if (Rook* rook = dynamic_cast<Rook*>(p)) {
			fout << endl << "R" << rook->getFaction() << rook->getRow() << rook->getColumn() << rook->hasMoved;
		}
		else if (Knight* knight = dynamic_cast<Knight*>(p)) {
			fout << endl << "N" << knight->getFaction() << knight->getRow() << knight->getColumn();
		}
		else if (Bishop* bishop = dynamic_cast<Bishop*>(p)) {
			fout << endl << "B" << bishop->getFaction() << bishop->getRow() << bishop->getColumn();
		}
		else if (Queen* queen = dynamic_cast<Queen*>(p)) {
			fout << endl << "Q" << queen->getFaction() << queen->getRow() << queen->getColumn();
		}
		else if (King* king = dynamic_cast<King*>(p)) {
			fout << endl << "K" << king->getFaction() << king->getRow() << king->getColumn() << king->hasMoved;
		}
		else if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
			fout << endl << "P" << pawn->getFaction() << pawn->getRow() << pawn->getColumn() << pawn->hasMoved << pawn->canBeCapturedEnPassant;
		}
	}
}

void loadGameFromFile(ifstream& fin) {
	string line;
	getline(fin, line);

	selPiece = NULL;
	legalMoves.clear();

	if (playersTurn != line[0] - '0') {
		playersTurn = line[0] - '0';
		switchCameraSide();
	}
	
	player1InCheck = line[1] == '1' ? true : false;
	player2InCheck = line[2] == '1' ? true : false;

	gameBoard.clear();
	while (fin) {
		if (fin.eof()) break; // otherwise last line would be read twice
		getline(fin, line);
		switch (line[0]) {
			case 'R':
				Rook* rook;
				rook = new Rook(gameBoard, rook_model, rook_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				rook->hasMoved = (bool) (line[4] - '0');
				break;
			case 'N':
				Knight* knight;
				knight = new Knight(gameBoard, knight_model, knight_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				break;
			case 'B':
				Bishop* bishop;
				bishop = new Bishop(gameBoard, bishop_model, bishop_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				break;
			case 'Q':
				Queen* queen;
				queen = new Queen(gameBoard, queen_model, queen_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				break;
			case 'K':
				King* king;
				king = new King(gameBoard, king_model, king_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				king->hasMoved = (bool)(line[4] - '0');
				break;
			case 'P':
				Pawn* pawn;
				pawn = new Pawn(gameBoard, pawn_model, pawn_model_low, line[1] - '0', line[2] - '0', line[3] - '0');
				pawn->hasMoved = (bool)(line[4] - '0');
				pawn->canBeCapturedEnPassant = (bool)(line[5] - '0');
				break;
		}
	}
	unlockSelector();
}

unsigned int loadCubemap(vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void lockSelector() {
	selectorLocked = true;
}

void unlockSelector() {
	selectorLocked = false;
}

void selectWithRayCasting() {
	// Mouse Picking with Ray Casting
	// Adopted from Dr Anton Gerdelan's work (https://antongerdelan.net/opengl/raycasting.html)

	// Traverse the transformation pipeline backwords

	// Step 0: 2d Viewport Coordinates
	// range [0:width, height:0]
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	//cout << "x: " << mouse_x << ", y: " << mouse_y << endl;

	// Step 1: 3d Normalised Device Coordinates
	// range [-1:1, -1:1, -1:1]
	float x = (2.0f * mouse_x) / W_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / W_HEIGHT;
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);

	// Step 2: 4d Homogeneous Clip Coordinates
	// range [-1:1, -1:1, -1:1, -1:1]
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	// Step 3: 4d Eye (Camera) Coordinates
	// range [-x:x, -y:y, -z:z, -w:w]
	vec4 ray_eye = inverse(camera->projectionMatrix) * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	// Step 4: 4d World Coordinates
	// range [-x:x, -y:y, -z:z, -w:w]
	vec3 ray_wor = vec3(inverse(camera->viewMatrix) * ray_eye);
	ray_wor = normalize(ray_wor);


	// Ray vs. Plane(y=0) intersection
	vec3 n = vec3(0, 1, 0); // plane normal

	float t = -(dot(camera->position, n)) / dot(ray_wor, n);
	if (t <= 0) return; // no intersection
	
	vec3 intersection = camera->position + ray_wor * t;

	int row = (int)(intersection.z / -0.753375 + 1);
	int col = (int)(intersection.x / 0.753375 + 1);

	if (row < 1 || row > 8 || col < 1 || col > 8) return; // intersection outside board

	selCol = col;
	selRow = row;
}

void renderHelpingWindow() {
	ImGui::Begin("Helper Window", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Game", "Ctrl+N")) {
				startNewGame();
			}
			if (ImGui::MenuItem("Save State")) {
				const char* filters = "State Files (*.state){.state}";
				time_t t = time(0);
				tm* now = localtime(&t);
				string def_name = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday) +
					"--" + to_string(now->tm_hour) + "-" + to_string(now->tm_min) + "-" + to_string(now->tm_sec) + ".state";
				ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", filters, string(saveFilesLocation.begin(), saveFilesLocation.end()) + "\\", def_name, 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
			}
			if (ImGui::MenuItem("Load State")) {
				const char* filters = "State Files (*.state){.state}";
				ImGuiFileDialog::Instance()->OpenDialog("LoadFileDlgKey", "Choose File", filters, string(saveFilesLocation.begin(), saveFilesLocation.end()) + "\\");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::ColorEdit3("Player 1 Color", &player1_color[0]);
	ImGui::SameLine();
	if (ImGui::Button("Reset###resetPlayer1Color")) {
		player1_color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	}
	ImGui::ColorEdit3("Player 2 Color", &player2_color[0]);
	ImGui::SameLine();
	if (ImGui::Button("Reset###resetPlayer2Color")) {
		player2_color = vec4(0.12f, 0.12f, 0.12f, 1.0f);
	}
	ImGui::Separator();
	ImGui::ColorEdit3("Square 1 Color", &square1_color[0]);
	ImGui::SameLine();
	if (ImGui::Button("Reset###resetSquare1Color")) {
		square1_color = vec4(0.55f, 0.55f, 0.55f, 1.0f);
	}
	ImGui::ColorEdit3("Square 2 Color", &square2_color[0]);
	ImGui::SameLine();
	if (ImGui::Button("Reset###resetSquare2Color")) {
		square2_color = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	}
	
	ImGui::Separator();
	ImGui::Checkbox("Flooded Board", &enableWater);
	ImGui::SameLine();
	ImGui::Checkbox("Translucent Pieces", &enableTranslucent);

	if (enableTranslucent) {
		ImGui::Text("Reflection");
		ImGui::SameLine();
		ImGui::SliderInt("Refraction", &reflectRefractCoeff, 0, 10, "", ImGuiSliderFlags_NoInput);
	}
	ImGui::Separator();
	ImGui::Text("Lights: %d", lightsCount);
	ImGui::SameLine();
	if (ImGui::Button("-###removeLight")) {
		lightsCount = std::max(lightsCount - 1, 1);
		selectedLight = lightsCount - 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("+###addLight")) {
		lightsCount = std::min(lightsCount + 1, MAX_LIGHTS);
		selectedLight = lightsCount - 1;
	}
	
	ImGui::Checkbox("Display a sphere on the location of each light###sphereLightLoc", &shouldDisplaySphereOnLight);
	for (unsigned int i = 0; i < lightsCount; i++) {
		float lightPos[3] = { light[i]->lightPosition_worldspace.x,  light[i]->lightPosition_worldspace.y, light[i]->lightPosition_worldspace.z };
		ImGui::SliderFloat3(("Light " + to_string(i + 1) + " Position###light" + to_string(i)).c_str(), &lightPos[0], -10.0f, 10.0f);
		light[i]->lightPosition_worldspace = vec3(lightPos[0], lightPos[1], lightPos[2]);

		ImGui::SliderFloat(("Light " + to_string(i + 1) + " Power###powerLight" + to_string(i)).c_str(), &light[i]->power, 1.0f, 150.0f);
	}

	// Load
	if (ImGuiFileDialog::Instance()->Display("LoadFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

			cout << "Loading saved game from file \"" << fileName << "\"...";
			ifstream fin;
			try {
				fin.open(filePathName);
			}
			catch (int e) {
				cout << endl << "Error: File \""<< fileName << "\" could not be loaded!" << endl;
				return;
			}
			if (!fin) {
				cout << "Failed" << endl;
				return;
			}
			loadGameFromFile(fin);
			cout << " Success" << endl;
			fin.close();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	// Save
	if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

			if (fileName.size() < 6 || fileName.compare(fileName.length() - 6, 6, ".state") != 0) {
				fileName = fileName + ".state";
				filePathName = filePathName + ".state";
			}
			cout << "Saving state in file \"" << fileName << "\"...";
			ofstream fout;
			try {
				fout.open(filePathName);
			}
			catch (int e) {
				cout << endl << "Error: Could not create file \"" << fileName << "\"!" << endl;
				return;
			}
			dumpCurrentGame(fout);
			cout << " Success" << endl;
			fout.close();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::Separator();
	ImGui::Text("Performance %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void) {
	try {
		initialize();
		createContext();
		cout << "Shader program locations:" << endl;
		eTB_GLSL__print_uniforms(shaderProgram);
		cout << "Water program locations:" << endl;
		eTB_GLSL__print_uniforms(waterProgram);
		startNewGame();
		mainLoop();
		free();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		getchar();
		free();
		return -1;
	}

	return 0;
}