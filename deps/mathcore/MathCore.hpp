#ifndef MATH_CORE_HPP
#define MATH_CORE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

namespace matc
{
  class Vector4;

  const int MAT_DIM = 4;
  
  class Matrix4x4
  {
  public:
    Matrix4x4();
    Matrix4x4(float value): values(MAT_DIM*MAT_DIM, value) {}
    Matrix4x4(const Matrix4x4 &m);
    ~Matrix4x4() {}
    
    Matrix4x4 operator+(const Matrix4x4 &other) const;
    Matrix4x4 operator*(const Matrix4x4 &other) const;

    //Vector4 operator*(const Vector4 &v) const;

    Matrix4x4 transpose() const;
    Matrix4x4 invert() const;
    
    void setIdentity();
    
    const std::vector<float> getValues() const;
    const float* asArray() const;
    std::string toString();
    void set(int row, int col, float v);
    
    friend std::ostream& operator<<(std::ostream &out, Matrix4x4 &m);
    
  private:
    std::vector<float> values;
    // float* values;
  };
  
  class Vector3
  {
  public:
    Vector3() {}
    Vector3(float x, float y, float z);
    Vector3(float *arr);
    Vector3(const Vector3 &v);
    Vector3(const Vector4 &v);
    ~Vector3() {}

    Vector3 operator+(const Vector3 &other) const;
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator*(const double d) const;
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;

    Vector3 normalize() const;
    
    std::string toString();
    
    friend std::ostream& operator<<(std::ostream &out, Vector3 &v);
    
    //private:
    float x;
    float y;
    float z;
  };
  
  class Vector4
  {
  public:
    Vector4() {}
    Vector4(const Vector4 &v);
    Vector4(float x, float y, float z, float w);
    Vector4(float *arr);
    ~Vector4(){}

    float& operator[](int pos) ;
    Vector4 operator-(const Vector4 &other) const;
    
    std::string toString();
    friend std::ostream& operator<<(std::ostream &out, Vector4 &v);
    
    //private:
    float x;
    float y;
    float z;
    float w;
  };

  class Vector2
  {
  public:
    Vector2():x(0), y(0) {}
    Vector2(const Vector2 &v);
    Vector2(float n_x, float n_y): x(n_x), y(n_y) {}
    Vector2(float *arr);
    ~Vector2() {}

    float x;
    float y;
    
    std::string toString();
    friend std::ostream& operator<<(std::ostream &out, Vector2 &v);

  };

  class Vector2i
  {
  public:
    Vector2i(int n_x, int n_y): x(n_x), y(n_y) {}
    Vector2i():Vector2i(0, 0) {}
    Vector2i(const Vector2i &v);
    Vector2i(int *arr);
    ~Vector2i() {}

    int x;
    int y;

    std::string toString();

    Vector2i operator+(const Vector2i &other) const;
    Vector2i operator*(const int &other) const;
    friend std::ostream& operator<<(std::ostream &out, Vector2i &v);
    friend bool operator== (const Vector2i &v1, const Vector2i &v2);
    friend bool operator!= (const Vector2i &v1, const Vector2i &v2);
    friend bool operator<(const Vector2i &v1, const Vector2i &v2);
  };
  bool operator== (const Vector2i &v1, const Vector2i &v2);
  bool operator!= (const Vector2i &v1, const Vector2i &v2);
  bool operator< (const Vector2i &v1, const Vector2i &v2);
  
  Matrix4x4 translate(const Matrix4x4 &m, const Vector3 &v);
  Matrix4x4 lookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up);
  Matrix4x4 perspective(const float zNear, const float zFar, const float fov_h, const float fov_v);
  Matrix4x4 rotate(const Matrix4x4 &m, float angle, const Vector3 &axis);
  Matrix4x4 scale(const Matrix4x4 &m, float sx, float sy, float sz);
  Matrix4x4 ortho(float left, float right, float bottom, float top, float nearVal, float farVal);

  std::vector<float> toFloatVector(const std::vector<matc::Vector3> &v);
  std::vector<float> toFloatVector(const std::vector<matc::Vector2> &v);

} // namespace matc
#endif // MATH_CORE_HPP
