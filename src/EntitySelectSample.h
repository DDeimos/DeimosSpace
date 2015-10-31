#ifndef ENTITY_SELECT_SAMPLE_H
#define ENTITY_SELECT_SAMPLE_H

#include "TerrainSample.h"

enum eCreatureMode { ROBOT = 1, NINJA = 2 };

class CCreature
{
public:
	CCreature(std::string name, bool robotMode, Ogre::SceneNode* node);
	~CCreature();

	void SetOffset(Ogre::Vector2 offset) { m_offset = offset; }

	std::string GetName() {return m_name; }
	Ogre::Vector2 GetOffset() { return m_offset; }
	Ogre::SceneNode* GetNode() { return m_node; }

protected:
	std::string m_name;
	Ogre::Vector2 m_offset;
	eCreatureMode m_mode;
	Ogre::SceneNode* m_node;
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
	bool m_leftMouseDown, m_rightMouseDown;
	bool m_movableFound;
	bool m_mode;
	double m_rotateSpeed;
};

#endif