/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "VSTransform.h"

#include <cmath>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform::VSTransform(VSTransform* parent)
{
  m_LocalTransform = VTK_PTR(vtkTransform)::New();

  setParent(parent);
  setupSignals();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform::VSTransform(const VSTransform& copy)
{
  m_LocalTransform = VTK_PTR(vtkTransform)::New();
  m_LocalTransform->DeepCopy(copy.m_LocalTransform);

  setParent(copy.getParent());
  setOriginPosition(copy.getOriginPosition().data());
  setupSignals();
}

// -----------------------------------------------------------------------------
VSTransform::~VSTransform() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setupSignals()
{
  connect(this, &VSTransform::emitPosition, this, [=] {
    emit updatedPosition();
    emit valuesChanged();
  });
  connect(this, &VSTransform::emitRotation, this, [=] {
    emit updatedRotation();
    emit valuesChanged();
  });
  connect(this, &VSTransform::emitScale, this, [=] {
    emit updatedScale();
    emit valuesChanged();
  });
  connect(this, &VSTransform::emitAll, this, [=] {
    emit updatedPosition();
    emit updatedRotation();
    emit updatedScale();
    emit valuesChanged();
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::operator=(const VSTransform& copy)
{
  m_LocalTransform->DeepCopy(copy.m_LocalTransform);
  emit emitAll();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setParent(VSTransform* parent)
{
  if(nullptr != m_Parent)
  {
    disconnect(m_Parent, SIGNAL(emitPosition()), this, SIGNAL(emitPosition()));
    disconnect(m_Parent, SIGNAL(emitRotation()), this, SIGNAL(emitRotation()));
    disconnect(m_Parent, SIGNAL(emitScale()), this, SIGNAL(emitScale()));
    disconnect(m_Parent, SIGNAL(emitAll()), this, SIGNAL(emitAll()));
  }

  m_Parent = parent;

  if(nullptr != parent)
  {
    connect(m_Parent, SIGNAL(emitPosition()), this, SIGNAL(emitPosition()));
    connect(m_Parent, SIGNAL(emitRotation()), this, SIGNAL(emitRotation()));
    connect(m_Parent, SIGNAL(emitScale()), this, SIGNAL(emitScale()));
    connect(m_Parent, SIGNAL(emitAll()), this, SIGNAL(emitAll()));
  }

  emit emitAll();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform* VSTransform::getParent() const
{
  return m_Parent;
}
// sadfasdf
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getPosition()
{
  std::array<double, 3> position;
  getGlobalTransform()->GetPosition(position.data());
  return position;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getLocalPosition()
{
  std::array<double, 3> position;
  m_LocalTransform->GetPosition(position.data());
  return position;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getRotation()
{
  if(nullptr == m_Parent)
  {
    return getLocalRotation();
  }

  std::array<double, 3> rotation;
  getGlobalTransform()->GetOrientation(rotation.data());

  for(int i = 0; i < 3; i++)
  {
    if(abs(rotation[i]) < 0.0001)
    {
      rotation[i] = 0.0;
    }
  }

  return rotation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getLocalRotation()
{
  std::array<double, 3> rotation;
  m_LocalTransform->GetOrientation(rotation.data());

  for(int i = 0; i < 3; i++)
  {
    if(abs(rotation[i]) < 0.0001)
    {
      rotation[i] = 0.0;
    }
  }

  return rotation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getScale()
{
  std::array<double, 3> scale;
  getGlobalTransform()->GetScale(scale.data());
  return scale;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::array<double, 3> VSTransform::getLocalScale()
{
  std::array<double, 3> scale;
  m_LocalTransform->GetScale(scale.data());
  return scale;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::translate(const std::array<double, 3>& delta)
{
  m_LocalTransform->Translate(delta.data());
  emit emitPosition();
  emit updatedLocalPosition();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::rotate(double amount, const std::array<double, 3>& axis)
{
  rotate(amount, axis.data());
}
// -----------------------------------------------------------------------------
void VSTransform::rotate(double amount, const double* axis)
{
  m_LocalTransform->RotateWXYZ(amount, axis);
  emit emitRotation();
  emit updatedLocalRotation();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::scale(double amount)
{
  m_LocalTransform->Scale(amount, amount, amount);
  emit emitScale();
  emit updatedLocalScale();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::scale(const std::array<double, 3>& amount)
{
  m_LocalTransform->Scale(amount.data());
  emit emitScale();
  emit updatedLocalScale();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkTransform) VSTransform::getGlobalTransform()
{
  VTK_NEW(vtkTransform, transform);
  transform->DeepCopy(m_LocalTransform);

  if(nullptr != m_Parent)
  {
    transform->SetInput(m_Parent->getGlobalTransform());
  }

  return transform;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkTransform) VSTransform::getLocalTransform()
{
  VTK_NEW(vtkTransform, transform);
  transform->DeepCopy(m_LocalTransform);

  updateTransform(transform);

  return transform;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkTransform* VSTransform::createTransform(const double* position, const double* rotation, const double* scale)
{
  vtkTransform* transform = vtkTransform::New();

  // Translate
  transform->Translate(position);

  // Only rotate if there is a value to rotate by
  if(abs(rotation[2]) >= 0.0001)
  {
    transform->RotateZ(rotation[2]);
  }
  if(abs(rotation[1]) >= 0.0001)
  {
    transform->RotateY(rotation[1]);
  }
  if(abs(rotation[0]) >= 0.0001)
  {
    transform->RotateX(rotation[0]);
  }

  // Scale
  transform->Scale(scale);

  // Set the hard value and reset the pipelined transformation
  updateTransform(transform);

  return transform;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalValues(const std::array<double, 3>& position, const std::array<double, 3>& rotation, const std::array<double, 3>& scale)
{
  m_LocalTransform->Identity();

  // Translate
  m_LocalTransform->Translate(position.data());

  // Only rotate if there is a value to rotate by
  if(abs(rotation[2]) >= 0.0001)
  {
    m_LocalTransform->RotateZ(rotation[2]);
  }
  if(abs(rotation[1]) >= 0.0001)
  {
    m_LocalTransform->RotateY(rotation[1]);
  }
  if(abs(rotation[0]) >= 0.0001)
  {
    m_LocalTransform->RotateX(rotation[0]);
  }

  // Scale
  m_LocalTransform->Scale(scale.data());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getPositionVector()
{
  std::array<double, 3> positionPtr = getPosition();
  std::vector<double> outputVector = {positionPtr[0], positionPtr[1], positionPtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getRotationVector()
{
  std::array<double, 3> rotationPtr = getRotation();
  std::vector<double> outputVector = {rotationPtr[0], rotationPtr[1], rotationPtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getScaleVector()
{
  std::array<double, 3> scalePtr = getScale();
  std::vector<double> outputVector = {scalePtr[0], scalePtr[1], scalePtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getLocalPositionVector()
{
  std::array<double, 3> localPositionPtr = getLocalPosition();
  std::vector<double> outputVector = {localPositionPtr[0], localPositionPtr[1], localPositionPtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getLocalRotationVector()
{
  std::array<double, 3> localRotationPtr = getLocalRotation();
  std::vector<double> outputVector = {localRotationPtr[0], localRotationPtr[1], localRotationPtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> VSTransform::getLocalScaleVector()
{
  std::array<double, 3> localScalePtr = getLocalScale();
  std::vector<double> outputVector = {localScalePtr[0], localScalePtr[1], localScalePtr[2]};
  return outputVector;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalPositionVector(std::vector<double> positionVector)
{
  setLocalPosition({positionVector[0], positionVector[1], positionVector[2]});
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalRotationVector(std::vector<double> rotationVector)
{
  setLocalRotation({rotationVector[0], rotationVector[1], rotationVector[2]});
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalScaleVector(std::vector<double> scaleVector)
{
  setLocalScale({scaleVector[0], scaleVector[1], scaleVector[2]});
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalPosition(const std::array<double, 3>& position)
{
  std::array<double, 3> rotation;
  std::array<double, 3> scale;

  m_LocalTransform->GetOrientation(rotation.data());
  m_LocalTransform->GetScale(scale.data());

  setLocalValues(position, rotation, scale);

  emit emitPosition();
  emit updatedLocalPosition();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalRotation(const std::array<double, 3>& rotation)
{
  std::array<double, 3> position;
  std::array<double, 3> scale;

  m_LocalTransform->GetPosition(position.data());
  m_LocalTransform->GetScale(scale.data());

  setLocalValues(position, rotation, scale);

  emit emitRotation();
  emit updatedLocalRotation();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setLocalScale(const std::array<double, 3>& scale)
{
  std::array<double, 3> position;
  std::array<double, 3> rotation;

  m_LocalTransform->GetPosition(position.data());
  m_LocalTransform->GetOrientation(rotation.data());

  setLocalValues(position, rotation, scale);

  emit emitScale();
  emit updatedLocalScale();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkTransform) VSTransform::getLocalizeTransform()
{
  VTK_PTR(vtkTransform) transform = getGlobalTransform();
  transform->Inverse();

  return transform;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkTransform) VSTransform::getGlobalizeTransform()
{
  return getGlobalTransform();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkTransform) VSTransform::getTransposedTransform()
{
  VTK_PTR(vtkTransform) transpose = getGlobalTransform();
  VTK_NEW(vtkMatrix4x4, matrix);
  transpose->GetTranspose(matrix);
  transpose->SetMatrix(matrix);

  return transpose;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizePoint(std::array<double, 3>& point)
{
  std::array<double, 3> p = {point[0], point[1], point[2]};
  getLocalizeTransform()->TransformPoint(point.data(), p.data());
  point.swap(p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizePoints(vtkPoints* points)
{
  getLocalizeTransform()->TransformPoints(points, points);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizeNormal(const std::array<double, 3>& normal)
{
  std::array<double, 3> n = {normal[0], normal[1], normal[2]};
  getLocalizeTransform()->TransformNormal(normal.data(), n.data());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizeNormals(vtkDataArray* normals)
{
  getLocalizeTransform()->TransformNormals(normals, normals);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizePlane(vtkPlane* plane)
{
  if(nullptr == plane)
  {
    return;
  }

  double* normalPtr = plane->GetNormal();
  double* originPtr = plane->GetOrigin();

  Array3Type normal = {normalPtr[0], normalPtr[1], normalPtr[2]};
  Array3Type origin = {originPtr[0], originPtr[1], originPtr[2]};

  localizeNormal(normal);
  localizePoint(origin);

  plane->SetNormal(normal.data());
  plane->SetOrigin(origin.data());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizePlanes(vtkPlanes* planes)
{
  planes->SetTransform(getGlobalizeTransform());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::localizeTransform(vtkTransform* transform)
{
  transform->SetInput(getLocalizeTransform());
  updateTransform(transform);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizePoint(const std::array<double, 3>& point)
{
  std::array<double, 3> p = {point[0], point[1], point[2]};
  getGlobalizeTransform()->TransformPoint(point.data(), p.data());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizePoints(vtkPoints* points)
{
  getGlobalizeTransform()->TransformPoints(points, points);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizeNormal(const std::array<double, 3>& normal)
{
  std::array<double, 3> n = {normal[0], normal[1], normal[2]};
  getGlobalizeTransform()->TransformNormal(normal.data(), n.data());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizeNormals(vtkDataArray* normals)
{
  getGlobalizeTransform()->TransformNormals(normals, normals);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizePlane(vtkPlane* plane)
{
  if(nullptr == plane)
  {
    return;
  }

  double* normal = plane->GetNormal();
  double* origin = plane->GetOrigin();

  globalizeNormal({normal[0], normal[1], normal[2]});
  globalizePoint({origin[0], origin[1], origin[2]});

  plane->SetNormal(normal);
  plane->SetOrigin(origin);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizePlanes(vtkPlanes* planes)
{
  planes->SetTransform(getLocalizeTransform());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::globalizeTransform(vtkTransform* transform)
{
  transform->SetInput(getGlobalizeTransform());
  updateTransform(transform);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::updateTransform(vtkTransform* transform)
{
  VTK_NEW(vtkMatrix4x4, matrix);
  matrix->DeepCopy(transform->GetMatrix());

  transform->SetInput(nullptr);
  transform->SetMatrix(matrix);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform::Array3Type VSTransform::getOriginPosition() const
{
  return m_OriginPosition;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setOriginPosition(double* originPosition)
{
  m_OriginPosition = {originPosition[0], originPosition[1], originPosition[2]};
}
void VSTransform::setOriginPosition(const Array3Type& originPosition)
{
  m_OriginPosition = originPosition;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform::Array3Type VSTransform::getOriginRotation() const
{
  return m_OriginRotation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setOriginRotation(double* originRotation)
{
  m_OriginRotation = {originRotation[0], originRotation[1], originRotation[2]};
}
void VSTransform::setOriginRotation(const Array3Type& originRotation)
{
  m_OriginRotation = originRotation;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSTransform::Array3Type VSTransform::getOriginScale() const
{
  return m_OriginScale;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSTransform::setOriginScale(double* originScale)
{
  m_OriginScale = {originScale[0], originScale[1], originScale[2]};
}
void VSTransform::setOriginScale(const Array3Type& originScale)
{
  m_OriginScale = originScale;
}
