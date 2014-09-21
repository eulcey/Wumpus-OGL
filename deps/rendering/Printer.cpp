#include "Printer.hpp"

bool Printer::visit(ModelNode& model)
{
  printTab(count);
  std::cout << "printing model: " << model << std::endl;
  return true;
}

bool Printer::visit(CubeNode& cube)
{
  printTab(count);
  std::cout << "printing cube: " << cube << std::endl;
  return true;
}

bool Printer::visit(TransformNode& transform)
{
  count = printTab(count);
  std::cout << "printing transform: " << transform << std::endl;
  return true;
}

bool Printer::visit(CameraNode& camera)
{
  printTab(count);
  std::cout << "printing camera: " << camera << std::endl;
  return true;
}

 bool Printer::visit(LightNode& light)
{
  printTab(count);
  std::cout << "printing light: " << light << std::endl;
  return true;
}

bool Printer::visit(MaterialNode& material)
{
  printTab(count);
  std::cout << "printing material: " << material << std::endl;
  return true;
}

bool Printer::leave(TransformNode& transform)
{
  count--;
  return true;
}

int Printer::printTab(int tabs)
{
  for(int i = 0; i < tabs; i++)
    {
      std::cout << "    ";
    }
  //  std::cout << tabs << std::endl;
  return tabs + 1;
}
