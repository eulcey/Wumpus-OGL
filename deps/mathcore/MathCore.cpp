#include "MathCore.hpp"

using namespace matc;

Matrix4x4::Matrix4x4()
{
  setIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4 &m)
{
  const float* v = m.asArray();
  this->values = std::vector<float>(v, v + MAT_DIM*MAT_DIM);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &other) const
{
  Matrix4x4 res;
  for(int i = 0; i < MAT_DIM; i++)
    {
      for(int j = 0; j < MAT_DIM; j++)
	{
	  res.set(i,j, this->asArray()[i*MAT_DIM+j] + other.asArray()[i*MAT_DIM+j]);
	}
    }
  return res;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const
{
  Matrix4x4 res;
   for(int i = 0; i < MAT_DIM; i++)
    {
      for(int j = 0; j < MAT_DIM; j++)
	{
	  float v = 0;
	  int this_row = i;
	  int other_col = j;
	  for(int k = 0; k < MAT_DIM; k++)
	    {
	      v += (other.asArray()[(this_row*MAT_DIM)+k]) * (this->asArray()[(k*MAT_DIM)+other_col]) ;
	    }
	  res.set(i, j, v);
	}
    }
   return res;
}

/*
Vector4 Matrix4x4::operator*(const Vector4 &v) const
{
  Vector4 res;
  Vector4 *vp = const_cast<Vector4*>(&v);
  for(int row = 0; row < MAT_DIM; row++)
    {
      float val = 0;
      for(int col = 0; col < MAT_DIM; col++)
	{
	  val += this->asArray()[row*MAT_DIM+col]*(*vp)[col];
	}
      res[row] = val;
    }
  return res;
}
*/

Matrix4x4 Matrix4x4::transpose() const
{
  Matrix4x4 res;
  for(int row = 0; row < MAT_DIM; row++)
    {
      for(int col = 0; col < MAT_DIM; col++)
	{
	  res.set(col, row, this->values.data()[row*MAT_DIM + col]);
	}
    }
  return res;
}

Matrix4x4 Matrix4x4::invert() const
{
  Matrix4x4 res;
  // from
  // https://code.google.com/p/simplex3d/source/browse/trunk/Simplex3dMath/src/float/simplex3d/math/floatx/functions.scala
  float a0 = values[0]*values[5] - values[4]*values[1];
  float a1 = values[0]*values[9] - values[8]*values[1];
  float a2 = values[0]*values[13] - values[12]*values[1];
  float a3 = values[4]*values[9] - values[8]*values[5];
  float a4 = values[4]*values[13] - values[12]*values[5];
  float a5 = values[8]*values[13] - values[12]*values[9];

  float b0 = values[2]*values[7] - values[6]*values[3];
  float b1 = values[2]*values[11] - values[10]*values[3];
  float b2 = values[2]*values[15] - values[14]*values[3];
  float b3 = values[6]*values[11] - values[10]*values[7];
  float b4 = values[6]*values[15] - values[14]*values[7];
  float b5 = values[10]*values[15] - values[14]*values[11];

  float det = (a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0);
  if (det < 0.0001 && det > -0.0001) {
    std::cerr << "Determinant is 0, returning Identity" << std::endl;
    return res;
  }

  res.set(0,0, (values[5]*b5 - values[9]*b4 + values[13]*b3)/det);
  res.set(0,1, (0-values[1]*b5 + values[9]*b2 - values[13]*b1)/det);
  res.set(0,2, (values[1]*b4 - values[5]*b2 + values[13]*b0)/det);
  res.set(0,3, (0-values[1]*b3 + values[5]*b1 - values[9]*b0)/det);

  res.set(1,0, (0-values[4]*b5 + values[8]*b4 - values[12]*b3)/det);
  res.set(1,1, (values[0]*b5 - values[8]*b2 + values[12]*b1)/det);
  res.set(1,2, (0-values[0]*b4 + values[4]*b2 - values[12]*b0)/det);
  res.set(1,3, (values[0]*b3 - values[4]*b1 + values[8]*b0)/det);

  res.set(2,0, (values[7]*a5 - values[11]*a4 + values[15]*a3)/det);
  res.set(2,1, (0-values[3]*a5 + values[11]*a2 - values[15]*a1)/det);
  res.set(2,2, (values[3]*a4 - values[7]*a2 + values[15]*a0)/det);
  res.set(2,3, (0-values[3]*a3 + values[7]*a1 - values[11]*a0)/det);

  res.set(3,0, (0-values[6]*a5 + values[10]*a4 - values[14]*a3)/det);
  res.set(3,1, (values[2]*a5 - values[10]*a2 + values[14]*a1)/det);
  res.set(3,2, (0-values[2]*a4 + values[6]*a2 - values[14]*a0)/det);
  res.set(3,3, (values[2]*a3 - values[6]*a1 + values[10]*a0)/det);
  
  return res;
}

void Matrix4x4::setIdentity()
{
  this->values.clear();
  this->values.resize(MAT_DIM*MAT_DIM,0);
  float *p = this->values.data();
  p[0] = 1.0f;
  p[MAT_DIM + 1] = 1.0f;
  p[2*MAT_DIM + 2] = 1.0f;
  p[3*MAT_DIM + 3] = 1.0f;
}

const std::vector<float> Matrix4x4::getValues() const
{
  return this->values;
}

const float* Matrix4x4::asArray () const
{
  return this->values.data();
}

std::string Matrix4x4::toString()
{
  std::string s = "[ ";

  for(int row = 0; row < MAT_DIM; row++)
    {
      s += "(";
      for(int col = 0; col < MAT_DIM; col++)
	{
	  s += std::to_string(values[MAT_DIM*row + col]);
	  s += " ";
	}
      s += ")";
    }
  s += "]";

  return s;
}

void Matrix4x4::set(int row, int col, float v)
{
  float *p = this->values.data();
  p[MAT_DIM*row + col] = v; // 11 und 14 falsch
}

std::ostream& matc::operator<<(std::ostream &out, Matrix4x4 &m)
{
  out << m.toString() << "\n";
  return out;
}
//================================
//---Vector 3 ----------
//================================
Vector3::Vector3(float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3::Vector3(float *arr)
{
  this->x = arr[0];
  this->y = arr[1];
  this->z = arr[2];
}

Vector3::Vector3(const Vector3 &v)
{
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
}

Vector3::Vector3(const Vector4 &v)
{
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
}

Vector3 Vector3::operator+(const Vector3 &other) const
{
  return Vector3(this->x+other.x, this->y+other.y, this->z+other.z);
}

Vector3 Vector3::operator-(const Vector3 &other) const
{
  return Vector3(this->x-other.x, this->y-other.y, this->z-other.z);
} 

Vector3 Vector3::operator*(const float d) const
{
  return Vector3(this->x*d, this->y *d, this->z *d);
}

Vector3 Vector3::operator/(const float d) const
{
  return Vector3(this->x/d, this->y/d, this->z/d);
}

float Vector3::dot(const Vector3 &other) const
{
  return this->x*other.x + this->y*other.y + this->z*other.z;
}

Vector3 Vector3::cross(const Vector3 &other) const
{
  Vector3 res;
  res.x = this->y*other.z - this->z*other.y;
  res.y = this->z*other.x - this->x*other.z;
  res.z = this->x*other.y - this->y*other.x;
  return res;
}

Vector3 Vector3::normalize() const
{
  Vector3 res(*this);
  float l_s = this->x*this->x + this->y*this->y + this->z*this->z;
  float l = sqrt(l_s);

  res.x /= l;
  res.y /= l;
  res.z /= l;
  return res;
}

float Vector3::lengthSquared() const
{
  return (x*x) + (y*y) + (z*z);
}

std::string Vector3::toString()
{
  std::string s = "[ ";
  s += std::to_string(x);
  s += " ";
  s += std::to_string(y);
  s += " ";
  s += std::to_string(z);
  s += " ]";
  return s;
}

std::ostream& matc::operator<<(std::ostream &out, Vector3 &v)
{
  out << v.toString() << "\n";
  return out;
}

//===================================
//------------Vector 4---------------
//===================================

Vector4::Vector4(const Vector4 &v)
{
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
  this->w = v.w;
}

Vector4::Vector4(float x, float y, float z, float w)
{
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

std::string Vector4::toString()
{
  std::string s = "[ ";
  s += std::to_string(x);
  s += " ";
  s += std::to_string(y);
  s += " ";
  s += std::to_string(z);
  s += " ";
  s += std::to_string(w);
  s += " ]";
  return s;
}

float& Vector4::operator[](int pos)
{
  switch(pos)
    {
    case 0: return x;
      break;
    case 1: return y;
      break;
    case 2: return z;
      break;
    case 3: return w;
    }
  // not sure what to do if out of bounds
  return x;
}

Vector4 Vector4::operator-(const Vector4 &other) const
{
  return Vector4(this->x-other.x, this->y-other.y, this->z-other.z, this->w-other.w);
}

std::ostream& matc::operator<<(std::ostream &out, Vector4 &v)
{
  out << v.toString() << "\n";
  return out;
}

//==================================
//-----------Vector2----------------
//==================================

Vector2::Vector2(const Vector2 &v)
{
  this->x = v.x;
  this->y = v.y;
}

Vector2::Vector2(float *arr)
{
  this->x = arr[0];
  this->y = arr[1];
}

std::string Vector2::toString()
{
  std::string s = "[ ";
  s += std::to_string(x);
  s += " ";
  s += std::to_string(y);
  s += " ]";
  return s;
}

std::ostream& matc::operator<<(std::ostream &out, Vector2 &v)
{
  out << v.toString() << "\n";
  return out;
}

//==================================
//-----------Vector2i----------------
//==================================

Vector2i::Vector2i(const Vector2i &v)
{
  this->x = v.x;
  this->y = v.y;
}

Vector2i::Vector2i(int *arr)
{
  this->x = arr[0];
  this->y = arr[1];
}

std::string Vector2i::toString()
{
  std::string s = "[ ";
  s += std::to_string(x);
  s += " ";
  s += std::to_string(y);
  s += " ]";
  return s;
}

std::ostream& matc::operator<<(std::ostream &out, Vector2i &v)
{
  out << v.toString() << "\n";
  return out;
}

Vector2i Vector2i::operator+(const Vector2i &other) const
{
  return Vector2i(this->x + other.x, this->y + other.y);
}

Vector2i Vector2i::operator*(const int &other) const
{
  return Vector2i(this->x * other, this->y * other);
}

bool matc::operator==(const Vector2i &v1, const Vector2i &v2)
{
  return (v1.x == v2.x && v1.y == v2.y);
}

bool matc::operator!= (const Vector2i &v1, const Vector2i &v2)
{
  return !(v1 == v2);
}

bool matc::operator<(const Vector2i &v1, const Vector2i &v2)
{
  return (v1.x < v2.x) || (v1.x == v2.x && v1.y < v2.y);
}
//==================================
//---Matrix Construction functions--
//==================================

Matrix4x4 matc::translate(const Matrix4x4 &m, const Vector3 &v)
{
  matc::Matrix4x4 res(m);
  const float* p = m.asArray();
  res.set(MAT_DIM-1, 0, p[MAT_DIM * (MAT_DIM-1) + 0] + v.x);
  res.set(MAT_DIM-1, 1, p[MAT_DIM * (MAT_DIM-1) + 1] + v.y);
  res.set(MAT_DIM-1, 2, p[MAT_DIM * (MAT_DIM-1) + 2] + v.z);
  return res;
}

Matrix4x4 matc::lookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
  Matrix4x4 res;

  Vector3 z_axis = (center - eye).normalize();
  Vector3 x_axis = (up.cross(z_axis)).normalize();
  Vector3 y_axis = z_axis.cross(x_axis);

  res.set(0,0, -x_axis.x); // false
  //res.set(0,1, y_axis.x);
  res.set(1,0, x_axis.y);
  //res.set(0,2, z_axis.x);
  res.set(2,0, -x_axis.z);
  res.set(3,0, x_axis.dot(eye));
 
  res.set(0,1, y_axis.x);
  res.set(1,1, y_axis.y);
  res.set(2,1, y_axis.z); // false
  res.set(3,1, -y_axis.dot(eye));
 
  //res.set(2,0, x_axis.z);
  res.set(0,2, -z_axis.x);
  //res.set(2,1, -y_axis.z);
  res.set(1,2, -z_axis.y);
  res.set(2,2, -z_axis.z); // false
  res.set(3,2, z_axis.dot(eye));

  //res.set(3, 0, x_axis.dot(eye));
  // res.set(3, 1, -y_axis.dot(eye));
  //res.set(3, 2, z_axis.dot(eye)); // false
  

  return res;
}


Matrix4x4 matc::perspective(float fovy, float aspect, float zNear, float zFar)
{
  Matrix4x4 res(0.0f);

  float f = 1/tan(fovy/2);//*3.14f/360.0f);
  res.set(0,0, f/aspect);
  res.set(1,1, f);
  res.set(2,2, (zFar+zNear)/(zNear-zFar));
  res.set(2,3, -1);
  res.set(3,2, (2.0*zFar*zNear)/(zNear-zFar));

  return res;
}

Matrix4x4 matc::rotate(const Matrix4x4 &m, float angle, const Vector3 &axis)
{
  Matrix4x4 res(m);
  Matrix4x4 rot;

  float c = cos(angle);
  float s = sin(angle);
  Vector3 norm = axis.normalize();

  rot.set(0,0, norm.x*norm.x*(1-c) + c);
  rot.set(1,0, norm.x*norm.y*(1-c)-norm.z*s);
  rot.set(2,0, (norm.x*norm.z*(1-c)+norm.y*s)); // -

  rot.set(0,1, norm.y*norm.x*(1-c)+norm.z*s);
  rot.set(1,1, norm.y*norm.y*(1-c)+c);
  rot.set(2,1, norm.y*norm.z*(1-c)-norm.x*s);

  rot.set(0,2, (norm.x*norm.z*(1-c)-norm.y*s)); // -
  rot.set(1,2, norm.y*norm.z*(1-c)+norm.x*s);
  rot.set(2,2, norm.z*norm.z*(1-c)+c);

  return rot*res;
}

Matrix4x4 matc::scale(const Matrix4x4 &m, float sx, float sy, float sz)
{
  Matrix4x4 res(m);
  Matrix4x4 scaling;

  scaling.set(0,0, sx);
  scaling.set(1,1, sy);
  scaling.set(2,2, sz);

  return scaling*res;
}

Matrix4x4 matc::ortho(float left, float right, float bottom, float top, float nearVal, float farVal)
{
  float r_l = 1/(right-left);
  float t_b = 1/(top-bottom);
  float f_n = 1/(farVal-nearVal);

  Matrix4x4 res;
  res.set(0,0, 2*r_l);
  res.set(1,1, 2*t_b);
  res.set(2,2, (-2)*f_n);
  
  res.set(3,0, -(right+left)*r_l);
  res.set(3,1, -(top+bottom)*t_b);
  res.set(3,2, -(farVal+nearVal)*f_n);

  return res;
}

std::vector<float> matc::toFloatVector(const std::vector<matc::Vector3> &v)
{
  std::vector<float> res;
  //res.resize(v.size()*3, 1.0f);
  res.reserve(v.size()*3);
  for(size_t i = 0; i < v.size(); i++)
    {
      matc::Vector3 v3 = v[i];
      res.push_back(v3.x);
      res.push_back(v3.y);
      res.push_back(v3.z);
      //res[i] = v3.x;
      //res[i+1] = v3.y;
      //res[i+2] = v3.z;
    }
  std::cout << "toflaot " << v.size() << std::endl;
  return res;
}

std::vector<float> matc::toFloatVector(const std::vector<matc::Vector2> &v)
{
  std::vector<float> res;
  //res.resize(v.size()*2);
  res.reserve(v.size()*3);
  for(size_t i = 0; i < v.size(); i++)
    {
      matc::Vector2 v2 = v[i];
      res.push_back(v2.x);
      res.push_back(v2.y);
    }
  return res;
}
