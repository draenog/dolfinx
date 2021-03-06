// Copyright (C) 2011-2018 Anders Logg and Garth N. Wells
//
// This file is part of DOLFINX (https://www.fenicsproject.org)
//
// SPDX-License-Identifier:    LGPL-3.0-or-later

#include "PETScOperator.h"
#include "PETScVector.h"
#include <cassert>
#include <dolfinx/common/log.h>
#include <petscvec.h>

using namespace dolfinx;
using namespace dolfinx::la;

//-----------------------------------------------------------------------------
PETScOperator::PETScOperator(Mat A, bool inc_ref_count) : _matA(A)
{
  assert(A);
  if (inc_ref_count)
    PetscObjectReference((PetscObject)_matA);
}
//-----------------------------------------------------------------------------
PETScOperator::PETScOperator(PETScOperator&& A)
    : _matA(std::exchange(A._matA, nullptr))
{
}
//-----------------------------------------------------------------------------
PETScOperator::~PETScOperator()
{
  // Decrease reference count (PETSc will destroy object once reference
  // counts reached zero)
  if (_matA)
    MatDestroy(&_matA);
}
//-----------------------------------------------------------------------------
PETScOperator& PETScOperator::operator=(PETScOperator&& A)
{
  std::swap(_matA, A._matA);

  return *this;
}
//-----------------------------------------------------------------------------
std::array<std::int64_t, 2> PETScOperator::size() const
{
  assert(_matA);
  PetscInt m(0), n(0);
  PetscErrorCode ierr = MatGetSize(_matA, &m, &n);
  if (ierr != 0)
    petsc_error(ierr, __FILE__, "MetGetSize");
  return {{m, n}};
}
//-----------------------------------------------------------------------------
PETScVector PETScOperator::create_vector(std::size_t dim) const
{
  assert(_matA);
  PetscErrorCode ierr;

  Vec x = nullptr;
  if (dim == 0)
  {
    ierr = MatCreateVecs(_matA, nullptr, &x);
    if (ierr != 0)
      petsc_error(ierr, __FILE__, "MatCreateVecs");
  }
  else if (dim == 1)
  {
    ierr = MatCreateVecs(_matA, &x, nullptr);
    if (ierr != 0)
      petsc_error(ierr, __FILE__, "MatCreateVecs");
  }
  else
  {
    LOG(ERROR) << "Cannot initialize PETSc vector to match PETSc matrix. "
               << "Dimension must be 0 or 1, not " << dim;
    throw std::runtime_error("Invalid dimension");
  }

  return PETScVector(x, false);
}
//-----------------------------------------------------------------------------
Mat PETScOperator::mat() const { return _matA; }
//-----------------------------------------------------------------------------
