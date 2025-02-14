#include "world.h"
#include <fstream>


std::string s;
World::World()
{

	loadResources();
}

void World::loadResources()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH = "data/";
    channelAudios = 0;
#else
	std::string PATH = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
    
#endif
	for (size_t i = 0; i < MAXBULLETS; i++)
	{
		bullets[i] = new sBullet();
	}
	loadObjectFile((PATH + s.assign("objects.txt")).c_str());


	this->sky = new EntityMesh();
	this->sky->mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
	this->sky->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());
	marketMision3 = new EntityMesh();
	marketMision3->mesh = Mesh::Get((PATH + s.assign("Props/marketSand1.obj")).c_str()); ;
	marketMision3->texture = Texture::Get((PATH + s.assign("Props/marketSand1.png")).c_str());;
	marketMision3->model.setTranslation(0,0,20);
	marketMision3->model.scale(5,5,5);

	ocean = new EntityMesh();
	ocean->mesh = new Mesh();
	ocean->mesh->createPlane(20000);
	ocean->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());
	ocean->name = "ocean";


	mainCharacter = new EntityCharacter();
	mainCharacter->mesh = Mesh::Get((PATH + s.assign("Hiccup/HiccupIntro.mesh")).c_str());
	mainCharacter->texture = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
	mainCharacter->animations.push_back(Animation::Get((PATH + s.assign("Hiccup/HiccupIdle.skanim")).c_str()));
	mainCharacter->animations.push_back(Animation::Get((PATH + s.assign("Hiccup/HiccupWalking.skanim")).c_str()));
	mainCharacter->animations.push_back(Animation::Get((PATH + s.assign("Hiccup/HiccupRunning.skanim")).c_str()));
	mainCharacter->animations.push_back(Animation::Get((PATH + s.assign("Hiccup/HiccupTwerk.skanim")).c_str()));
	mainCharacter->animations.push_back(Animation::Get((PATH + s.assign("Hiccup/HiccupPet.skanim")).c_str()));

	mainCharacter->model.setTranslation(-10, 0, 0);


	ground = new EntityMesh();
	ground->mesh = new Mesh();
	ground->mesh->createPlane(100);
	ground->texture = Texture::Get((PATH + s.assign("grass.jpg")).c_str());
	ground->tiling = 100.0f;

}
#pragma region READ/WRITE MESHES
void World::loadObjectFile(const char* path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH2 = "data/";
#else
	std::string PATH2 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	std::vector<EntityMesh*>* entities;
	std::vector<EntityCharacterDragon*>* entitiesDragons;
	std::string type = "";
	bool entityB = false;
	EntityMesh* entity;
	EntityCharacterDragon* entityDragon;

	while (!ss.eof()) {
		std::string out;
		ss >> out;
		if (strcmp(out.c_str(), "TYPE") == 0) {
			ss >> out;
			type.assign(out);
			if (strcmp(type.c_str(), "STATICENTITIES") == 0) {
				entities = &this->staticEntities;
			}
			else if (strcmp(type.c_str(), "STATICCHARACTER") == 0) {
				entities = &this->staticEntitiesCharacter;
			}
			else if (strcmp(type.c_str(), "STATICDRAGONS") == 0) {
				entities = &this->staticEntitiesDragons;
			}
			else if (strcmp(type.c_str(), "DYNAMICDRAGONS") == 0) {
				entitiesDragons = &this->dynamicEntitiesDragons;
			}
			else if (strcmp(type.c_str(), "BUILD") == 0) {
				entities = &this->buildWorld;
			}
			else if (strcmp(type.c_str(), "MISSION1") == 0) {
				entities = &this->mission1Entities;
			}
			else if (strcmp(type.c_str(), "PLANTS") == 0) {
				entities = &this->staticEntitiesPlants;
			}
			else if (strcmp(type.c_str(), "MISSION2") == 0) {
				entities = &this->mission2Entities;
			}
			else if (strcmp(type.c_str(), "MISSION3") == 0) {
				entities = &this->mission3Entities;
			}
		}

		if (strcmp(type.c_str(), "DYNAMICDRAGONS") != 0) {

			readEntitiesAttributes(ss, out, entityB, entity, PATH2, entities);
		}
		else {
			readEntitiesCharacterDragonAttributes(ss, out, entityB, entityDragon, PATH2, entitiesDragons);
		}

		//staticCharacterDragonRead(type, ss, out, entityB, entity, PATH2, entities);
	}

}

void World::writeObjectFile(const char* path)
{
	std::ofstream outdata; // outdata is like cin
	

	outdata.open(path); // opens the file
	if (!outdata) { // file couldn't be opened
		std::cerr << "Error: file could not be opened" << std::endl;
		exit(1);
	}
	std::cerr << "Saving file" << std::endl;
	for (int i = 0; i < this->staticEntities.size();i++) {
		EntityMesh* entity = staticEntities[i];
		staticEntitiesWrite(outdata, entity, "STATICENTITIES");
	}
	for (int i = 0; i < this->staticEntitiesCharacter.size(); i++) {
		EntityMesh* entity = staticEntitiesCharacter[i];
		staticEntitiesWrite(outdata, entity, "STATICCHARACTER");
	}
	for (int i = 0; i < this->staticEntitiesDragons.size(); i++) {
		EntityMesh* entity = staticEntitiesDragons[i];
		staticEntitiesWrite(outdata, entity, "STATICDRAGONS");
	}
	for (int i = 0; i < this->mission1Entities.size(); i++) {
		EntityMesh* entity = mission1Entities[i];
		staticEntitiesWrite(outdata, entity, "MISSION1");
	}
	for (int i = 0; i < this->buildWorld.size(); i++) {
		EntityMesh* entity = buildWorld[i];
		staticEntitiesWrite(outdata, entity, "BUILD");
	}
	for (int i = 0; i < this->staticEntitiesPlants.size(); i++) {
		EntityMesh* entity = staticEntitiesPlants[i];
		staticEntitiesWrite(outdata, entity, "PLANTS");
	}
	for (int i = 0; i < this->mission2Entities.size(); i++) {
		EntityMesh* entity = mission2Entities[i];
		staticEntitiesWrite(outdata, entity, "MISSION2");
	}
	for (int i = 0; i < this->mission3Entities.size(); i++) {
		EntityMesh* entity = mission3Entities[i];
		staticEntitiesWrite(outdata, entity, "MISSION3");
	}
	for (int i = 0; i < this->dynamicEntitiesDragons.size(); i++) {
		EntityCharacterDragon* entity = dynamicEntitiesDragons[i];
		std::string type;
		writeDynamicDragons(outdata, entity, "DYNAMICDRAGONS");
		

	}
	outdata.close();
}

void World::writeDynamicDragons(std::ofstream& outdata,  EntityCharacterDragon* entity, std::string type)
{
	outdata << "TYPE " + type << std::endl;
	outdata << "MESH " + entity->mesh->name << std::endl;
	outdata << "TEX " + entity->texture->filename << std::endl;
	outdata << "MESH2 " + entity->characterMesh->name << std::endl;
	outdata << "TEX2 " + entity->characterTex->filename << std::endl;
	if (!entity->animations.empty()) {
		for (size_t i = 0; i < entity->animations.size(); i++)
		{
			outdata << "ANIM " + entity->animations[i]->filename << std::endl;
		}
	}
	Vector3 entityPos = entity->getPosition();
	outdata << "POS " + std::to_string(entityPos.x) + " " + std::to_string(entityPos.y) + " " + std::to_string(entityPos.z) << std::endl;
	Vector3 entityOff = entity->characterOffset.getTranslation();
	outdata << "OFFSET " + std::to_string(entityOff.x) + " " + std::to_string(entityOff.y) + " " + std::to_string(entityOff.z) << std::endl;
	/*float* entityRot = entity->model.getRotationMatrix();
	std::string rotMatrix;
	for (size_t i = 0; i < 9; i++)
	{
		rotMatrix.append(std::to_string(entityRot[i]));
		rotMatrix.append(" ");

	}
	outdata << "ROT "+ rotMatrix << std::endl;*/
	if (strcmp(entity->name.c_str(), "") != 0) {
		outdata << "NAME " + entity->name << std::endl;
	}
	outdata << "END" << std::endl;
}

void World::staticEntitiesWrite(std::ofstream& outdata, EntityMesh* entity, std::string type)
{
	outdata << "TYPE " + type << std::endl;
	
	outdata << "MESH " + entity->mesh->name << std::endl;
	
	
	outdata << "TEX " + entity->texture->filename << std::endl;

	if (!entity->animations.empty()) {
		for (size_t i = 0; i < entity->animations.size(); i++)
		{

			outdata << "ANIM " + entity->animations[i]->filename << std::endl;
		}
	}
	if (strcmp(type.c_str(), "BUILD") == 0) {
		if (strcmp(entity->name.c_str(), "") != 0) {
			outdata << "NAME " + entity->name << std::endl;
		}
	}
	
	if (strcmp(type.c_str(), "BUILD") != 0) {
		Vector3 entityPos = entity->getPosition();
		outdata << "POS " + std::to_string(entityPos.x) + " " + std::to_string(entityPos.y) + " " + std::to_string(entityPos.z) << std::endl;
		Vector3 entityScale = entity->model.getScale();
		outdata << "SCALE " + std::to_string(entityScale.x) + " " + std::to_string(entityScale.y) + " " + std::to_string(entityScale.z) << std::endl;
		float* entityRot = entity->model.getRotationMatrix();
		std::string rotMatrix;
		for (size_t i = 0; i < 16; i++)
		{
			rotMatrix.append(std::to_string(entityRot[i]));
			rotMatrix.append(" ");

		}
		outdata << "ROT " + rotMatrix << std::endl;
		if (strcmp(entity->name.c_str(), "") != 0) {
			outdata << "NAME " + entity->name << std::endl;
		}
		if (!entity->audios.empty()) {
			outdata << "AUDIO"  << std::endl;
			outdata << entity->audios.size() << std::endl;
			for (size_t i = 0; i < entity->audios.size(); i++)
			{
				outdata <<  entity->audios[i] << std::endl;
			}

		}
		
	}
	
	outdata << "END" << std::endl;
}

void World::readEntitiesCharacterDragonAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityCharacterDragon*& entityDragon, std::string& PATH2, std::vector<EntityCharacterDragon*>* entitiesDragons)
{
	ss >> out;
	if (!entityB) {


		entityDragon = new EntityCharacterDragon();
		entityB = true;
	}
	if (strcmp(out.c_str(), "MESH") == 0) {
		ss >> out;
		entityDragon->mesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX") == 0) {
		ss >> out;
		entityDragon->texture = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "MESH2") == 0) {
		ss >> out;
		entityDragon->characterMesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX2") == 0) {
		ss >> out;
		entityDragon->characterTex = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "ANIM") == 0) {
		ss >> out;
		entityDragon->animations.push_back(Animation::Get((PATH2 + out).c_str()));
		ss >> out;
	}
	if (strcmp(out.c_str(), "POS") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entityDragon->model.translate(x, y, z);
		ss >> out;

	}
	if (strcmp(out.c_str(), "OFFSET") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entityDragon->characterOffset.translate(x, y, z);
		ss >> out;

	}
	//if (strcmp(out.c_str(), "ROT") == 0) {
	//	
	//	entityDragon->model.rotate(x * DEG2RAD, Vector3(1, 0, 0));
	//	
	//	ss >> out;
	//}
	if (strcmp(out.c_str(), "NAME") == 0) {
		ss >> out;
		entityDragon->name = out;
		ss >> out;
	}
	if (strcmp(out.c_str(), "END") == 0) {
		entityB = false;
		entitiesDragons->push_back(entityDragon);

	}
}

void World::readEntitiesAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities)
{
	ss >> out;
	if (!entityB) {


		entity = new EntityMesh();
		entityB = true;
	}
	if (strcmp(out.c_str(), "MESH") == 0) {
		ss >> out;
		entity->mesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX") == 0) {
		ss >> out;
		entity->texture = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "ANIM") == 0) {
		ss >> out;
		entity->animations.push_back(Animation::Get((PATH2 + out).c_str()));
		ss >> out;
	}
	if (strcmp(out.c_str(), "POS") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entity->model.setTranslation(x, y, z);
		ss >> out;

	}
	if (strcmp(out.c_str(), "SCALE") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entity->model.scale(x, y, z);
		ss >> out;
	}
	if (strcmp(out.c_str(), "ROT") == 0) {
		float a1, a2, a3, a4, b1, b2, b3,b4, c1, c2, c3,c4, d1,d2,d3,d4;
		ss >> a1 >> a2 >> a3 >> a4>> b1 >> b2 >> b3 >> b4 >> c1 >> c2 >> c3 >> c4 >> d1 >> d2 >> d3 >> d4 ;
		entity->model.setMatrix(a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4);
		ss >> out;
	}
	if (strcmp(out.c_str(), "NAME") == 0) {
		ss >> out;
		entity->name = out;
		ss >> out;
	}
	if (strcmp(out.c_str(), "AUDIO") == 0) {
		int length;
		ss >> length;
		for (int i = 0; i < length; i++) {
			ss >> out;
			entity->audios.push_back(out);
		}
		ss >> out;
	}
	if (strcmp(out.c_str(), "END") == 0) {
		entityB = false;
		entities->push_back(entity);

	}
}


#pragma endregion

void World::Mision1(std::vector<EntityMesh*>& entities){
    World* world = Game::instance->world;
    EntityCharacterDragon* currentDragon = world->dynamicEntitiesDragons[world->currentDragon];
    for (size_t i = 0; i < entities.size(); i++) {
        EntityMesh* currentEntity = entities[i];
        Vector3 center = currentDragon->nexPos + Vector3(0, 3, 0);
        Vector3 coll;
        Vector3 collnorm;
        //comprobamos si colisiona el objeto con la esfera (radio 3)
        if (!currentEntity->mesh->testSphereCollision(currentEntity->model, center, 1.0f, coll, collnorm))
            continue; //si no colisiona, pasamos al siguiente objeto
        
        RemoveSelected(entities, currentEntity);
		//audio pescaito
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		std::string PATH = "data/";
#else
		std::string PATH = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";

#endif
		Audio::Play((PATH + s.assign("sounds/CogerMonedaAro.wav")).c_str(), NULL);
    
    }
    Camera* cam = Game::instance->camera;
    checkFrustrumStatic(entities, cam->eye);
    
}

void World::Mision2(std::vector<EntityMesh*>& entities)
{
	World* world = Game::instance->world;
	EntityCharacterDragon* currentDragon = world->dynamicEntitiesDragons[world->currentDragon];
	
	Camera* cam = Game::instance->camera;
	checkFrustrumStatic(entities, cam->eye);

	for (size_t i = 0; i < MAXBULLETS; i++)
	{
		sBullet* currentBullet = world->bullets[i];
		if (!currentBullet->isActive()) {
			continue;
		}
		Vector3 currentPos = currentBullet->model.getTranslation();

		currentBullet->model.setTranslation(currentPos.x,currentPos.y,currentPos.z);

		//Render bullet
		Shader*	shader = Game::instance->shader;
		if (!shader) return;
		//enable shader
		shader->enable();
		//upload uniforms
		Camera* cam = Game::instance->camera;
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
		shader->setUniform("u_texture", currentBullet->tex, 0);
		shader->setUniform("u_time", time);
		shader->setUniform("u_model", currentBullet->model);
		shader->setUniform("u_tex_tiling", 1.0f);
		currentBullet->mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();


	}
}

void World::Mision3(std::vector<EntityMesh*>& entities) {
	World* world = Game::instance->world;
	EntityCharacterDragon* currentDragon = world->dynamicEntitiesDragons[world->currentDragon];
	for (size_t i = 0; i < entities.size(); i++) {
		EntityMesh* currentEntity = entities[i];
		Vector3 center = currentDragon->nexPos + Vector3(0, 0, 0);
		Vector3 coll;
		Vector3 collnorm;
		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if (!currentEntity->mesh->testSphereCollision(currentEntity->model, center, 2.0f, coll, collnorm))
			continue; //si no colisiona, pasamos al siguiente objeto
		if (currentDragon->angle3 > 24.0) {

			RemoveSelected(entities, currentEntity);
			//audio pescaito
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
			std::string PATH = "data/";
#else
			std::string PATH = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";

#endif
			Audio::Play((PATH + s.assign("sounds/CogerPez.wav")).c_str(), NULL);
		}


	}
	Camera* cam = Game::instance->camera;
	checkFrustrumStatic(entities, cam->eye);
	checkFrustrumEntity(marketMision3, cam->eye);

}


