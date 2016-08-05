#ifndef ENTITY_SELECT_SAMPLE_H
#define ENTITY_SELECT_SAMPLE_H

#include "TerrainSample.h"

enum eCreatureMode { ROBOT = 1, NINJA = 2 };

class CCreature
{
public:
	CCreature(std::string name, bool robotMode, Ogre::Entity* entity, Ogre::SceneNode* node, float speed);
	~CCreature();

	void UpdateRotation();
	void Move(float dt, Ogre::TerrainGroup* terrainGroup);

	std::string GetName() {return m_name; }
	Ogre::SceneNode* GetNode() { return m_node; }
	void SetAnimation(std::string name);
	void SetTarget(Ogre::Vector3 scenePos);
	bool IsFinished(float step) { return (m_distance -= step) <= 0.f; }

protected:
	std::string m_name;
	eCreatureMode m_mode;
	Ogre::Entity* m_entity;
	Ogre::SceneNode* m_node;
	Ogre::AnimationState* mAnimation;
	float m_speed;
	float m_distance;
	Ogre::Vector3 m_direction;
};

class EntitySelectSample : public TerrainSample
{
public:
	EntitySelectSample();
	virtual ~EntitySelectSample();

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	virtual void CreateNodeUnderCursor();
	virtual bool SelectNodeUnderCursor();
	virtual void MoveNodeUnderCursor();
	virtual void ShowNodesBoxes(bool show);
	virtual CCreature* FindCreatureByName(std::string name);
	virtual CCreature* GetSelectedCreature();

private:
	std::vector<CCreature*> m_creatures;
	Ogre::RaySceneQuery* m_rayScnQuery;
	bool m_leftMouseDown, m_rightMouseDown, m_altDown;
	bool m_movableFound;
	bool m_mode;
	double m_rotateSpeed;
};

#endif