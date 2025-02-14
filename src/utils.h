/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>

#include "includes.h"
#include "framework.h"
#include "camera.h"
#include "entityMesh.h"

class EntityMesh;
class sBullet {
public:
	Mesh* mesh;
	Texture* tex;
	Matrix44 model;
	Vector3 last_position;
	Vector3 velocity;
	float ttl;
	float power;
	int author;
	bool isActive() { return ttl > 0.0f; };
};

//General functions **************
long getTime();
bool readFile(const std::string& filename, std::string& content);
bool readFileBin(const std::string& filename, std::vector<unsigned char>& buffer);
void setUpCamera(Matrix44& model, Vector3 eyeVec, Vector3 centerVec, Vector3 upVec, Camera* camera);
void AddEntityInFront(Camera* cam, EntityMesh* entity, std::vector<EntityMesh*>& entities);
void RayPickCheck(Camera* cam, std::vector<EntityMesh*> entities);
void checkFrustrumStatic(std::vector<EntityMesh*>& entities, Vector3& camPos);
void checkFrustrumEntity(EntityMesh*& entity, Vector3& camPos);
void checkCollisionEntities(std::vector<EntityMesh*>& entities, Vector3& character_center, float dt, Vector3& nexPos, Vector3& currentPos);
void checkCollisionEntity(EntityMesh* entity, Vector3& character_center, float dt, Vector3& nexPos, Vector3& currentPos);
void RotateSelected(float angleDegrees, Vector3 rotationVector);
void MoveSelected(float x, float y, float z);
void ScaleSelected(float x, float y, float z);
void RemoveSelected(std::vector<EntityMesh*>& entities, Entity* selectedEntity);
sBullet* getFreeBullet();
bool spawnBullet(Matrix44 model, Vector3 last_position, Vector3 velocity, float ttl);
void deleteBullet(sBullet* b);
Vector3 getRayDirectionBullet(int nexPos_x, int nexPos_y, Vector3 currentPos, float window_width, float window_height);

void checkGameState();
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
void checkAudios();
#endif
//generic purposes fuctions

void RenderGUI(Mesh quad, Texture* tex, Vector4 color);
bool RenderButton(float x, float y, float w, float h, Texture* tex, bool wasLeftPressed);

void drawGrid();
bool drawText(float x, float y, std::string text, Vector3 c, float scale = 1);

//check opengl errors
bool checkGLErrors();

std::string getPath();
void stdlog(std::string str);

Vector2 getDesktopSize( int display_index = 0 );

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string join(std::vector<std::string>& strings, const char* delim);

std::string getGPUStats();
void drawGrid();

//Used in the MESH and ANIM parsers
char* fetchWord(char* data, char* word);
char* fetchFloat(char* data, float& f);
char* fetchMatrix44(char* data, Matrix44& m);
char* fetchEndLine(char* data);
char* fetchBufferFloat(char* data, std::vector<float>& vector, int num = 0);
char* fetchBufferVec3(char* data, std::vector<Vector3>& vector);
char* fetchBufferVec2(char* data, std::vector<Vector2>& vector);
char* fetchBufferVec3u(char* data, std::vector<Vector3u>& vector);
char* fetchBufferVec4ub(char* data, std::vector<Vector4ub>& vector);
char* fetchBufferVec4(char* data, std::vector<Vector4>& vector);


//struct sProp{
//	EntityMesh* bulletMesh;
//	Texture* bulletTex;
//};

#endif
