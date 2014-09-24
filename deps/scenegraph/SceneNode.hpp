#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <iostream>
#include <list>
#include <string>
class NodeVisitor;

class SceneNode
{
public:
  SceneNode(std::string newName): name(newName) {}
  virtual ~SceneNode() {}//TODO how to: destroy(); }

  void release() { delete this; }

  virtual void update()
  {
     for(std::list<SceneNode*>::iterator i = lstChildren.begin();
	i != lstChildren.end();
	i++)
      {
	(*i)->update();
      }
  }

  virtual bool accept(NodeVisitor& visitor) = 0;
  
  void destroy()
  {
    for(std::list<SceneNode*>::iterator i = lstChildren.begin();
	i != lstChildren.end();
	i++)
      {
      	(*i)->release();
      }
    lstChildren.clear();
  }

  void addChild(SceneNode* node)
  {
    lstChildren.push_back(node);
  }

  bool removeChild(SceneNode &node)
  {
    std::list<SceneNode*>::iterator iter;
    for(iter = lstChildren.begin();
	iter != lstChildren.end();
	iter++)
      {
	if ((*iter) == &node) {
	  std::cout << "SCENENODE: erased: " << node << std::endl;
	  lstChildren.erase(iter);
	  return true;
	}
      }
    return false;
  }

  std::string getName() { return this->name; }
  
  friend std::ostream& operator<<(std::ostream &out, SceneNode& node)
  {
    node.doStream(out);
    return out;
  }
  
protected:
  virtual void doStream(std::ostream& os)
  {
    os << name;
  }
  std::list<SceneNode*> lstChildren;
  std::string name;
};

#endif
