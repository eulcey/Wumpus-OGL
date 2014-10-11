#include "Spring.hpp"

using namespace matc;

Spring::Spring(std::string name, matc::Vector3 start, matc::Vector3 end, float speed): TransformNode(name, Matrix4x4()), startVec(start), endVec(end)
{
  Vector3 dist = end - start;
  cycleTranslation = dist*speed;
}

Spring::~Spring()
{
}

bool Spring::accept(NodeVisitor& visitor)
{
  return TransformNode::accept(visitor);
}

void Spring::update(float deltaTime)
{
  
  Vector3 deltaTrans = cycleTranslation*deltaTime;
  
  transform = translate(transform, deltaTrans);
  
  const float* val = transform.asArray();
  Vector3 pos(val[12], val[13], val[14]);
  
  Vector3 distEndStart = endVec - startVec;
  float lenEndStart = distEndStart.lengthSquared();
  
  Vector3 distToEnd = endVec - pos;
  float lenToEnd = distToEnd.lengthSquared();

  Vector3 distToStart = startVec - pos;
  float lenToStart = distToStart.lengthSquared();

  if(lenToEnd > lenEndStart || lenToStart > lenEndStart) {
    cycleTranslation = cycleTranslation * -1.0f;
  }
}

float Spring::percentToEnd()
{
  Vector3 distEndStart = endVec - startVec;
  float lenEndStart = distEndStart.lengthSquared();
  
  const float* val = transform.asArray();
  Vector3 pos(val[12], val[13], val[14]);
  
  Vector3 distToEnd = endVec - pos;
  float lenToEnd = distToEnd.lengthSquared();

  return lenToEnd/lenEndStart;
}
