// This file is part of Hermes2D.
//
// Hermes2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Hermes2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hermes2D.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __H2D_EXACT_SOLUTION_H
#define __H2D_EXACT_SOLUTION_H

#include "solution.h"

namespace Hermes
{
  namespace Hermes2D
  {
    /// @ingroup meshFunctions
    /// \brief Represents an exact solution of a PDE.
    ///
    /// ExactSolution represents an arbitrary user-specified function defined on a domain (mesh),
    /// typically an exact solution to a PDE. This can be used to compare an approximate solution
    /// with an exact solution (see DiffFilter).
    template<typename Scalar>
    class HERMES_API ExactSolution : public Solution<Scalar>
    {
    public:
      ExactSolution(MeshSharedPtr mesh);
      virtual ~ExactSolution() {};

      /// Dimension of result - either 1 or 2.
      virtual unsigned int get_dimension() const = 0;

      virtual MeshFunction<Scalar>* clone() const;

      inline std::string getClassName() const { return "ExactSolution"; }

      /// Saves the exact solution to an XML file.
      virtual void save(const char* filename) const;
#ifdef WITH_BSON
      virtual void save_bson(const char* filename) const;
#endif
      /// Function returning the integration order that
      /// should be used when integrating the function.
      virtual Hermes::Ord ord(double x, double y) const = 0;

      protected:
      /// For scaling of the solution.
      Scalar exact_multiplicator;
      template<typename T> friend class Solution;
    };

    /// @ingroup meshFunctions
    /// These classes are abstract (pure virtual destructor).
    /// The user is supposed to subclass them (see e.g. NIST benchmarks).
    template<typename Scalar>
    class HERMES_API ExactSolutionScalar : public ExactSolution<Scalar>
    {
    public:
      ExactSolutionScalar(MeshSharedPtr mesh);
      virtual ~ExactSolutionScalar() {};

      /// For Scalar-valued solutions this returns 1.
      virtual unsigned int get_dimension() const;

      /// Function returning the value.
      virtual Scalar value (double x, double y) const = 0;

      /// Function returning the derivatives.
      virtual void derivatives (double x, double y, Scalar& dx, Scalar& dy) const = 0;

      /// Function returning the value and derivatives.
      Scalar exact_function (double x, double y, Scalar& dx, Scalar& dy) const {
        derivatives (x, y, dx, dy);
        return value (x, y);
      };
    };


    /// @ingroup meshFunctions.
    /// Serves for postprocessing of element-wise constant values (such as the error in adaptivity).
    /// The second template parameter ValueType must be a type castable to Scalar.
    template<typename Scalar, typename ValueType>
    class HERMES_API ExactSolutionConstantArray : public ExactSolutionScalar<Scalar>
    {
    public:
      /// Constructor.
      /// \param[in] valueArray Array of element-wise values, sorted according to the elements' ids.
      ExactSolutionConstantArray(MeshSharedPtr mesh, ValueType* valueArray, bool deleteArray = false);
      virtual ~ExactSolutionConstantArray();

      virtual MeshFunction<Scalar>* clone() const;

      /// Function returning the value.
      virtual Scalar value (double x, double y) const;
      virtual Ord ord(double x, double y) const;

      /// Function returning the derivatives.
      virtual void derivatives (double x, double y, Scalar& dx, Scalar& dy) const;

      inline std::string getClassName() const { return "ExactSolutionConstantArray"; }

      void setArray(ValueType* valueArray);

    protected:
      /// Array of the values.
      ValueType* valueArray;

      /// Delete the array.
      bool deleteArray;

      template<typename T> friend class Solution;
    };
    

    /// @ingroup meshFunctions
    template<typename Scalar>
    class HERMES_API ExactSolutionVector : public ExactSolution<Scalar>
    {
    public:
      ExactSolutionVector(MeshSharedPtr mesh);
      virtual ~ExactSolutionVector() {};

      /// For vector-valued solutions this returns 2.
      virtual unsigned int get_dimension() const;

      /// Function returning the value.
      virtual Scalar2<Scalar> value (double x, double y) const = 0;

      /// Function returning the derivatives.
      virtual void derivatives (double x, double y, Scalar2<Scalar>& dx, Scalar2<Scalar>& dy) const = 0;

      /// Function returning the value and derivatives.
      virtual Scalar2<Scalar> exact_function(double x, double y, Scalar2<Scalar>& dx, Scalar2<Scalar>& dy) const {
        derivatives (x, y, dx, dy);
        return value (x, y);
      };
    };
    
    /// @ingroup meshFunctions
    template<typename Scalar>
    class HERMES_API ConstantSolution : public ExactSolutionScalar<Scalar>
    {
    public:
      ConstantSolution(MeshSharedPtr mesh, Scalar constant);
      virtual ~ConstantSolution() {};

      virtual Scalar value (double x, double y) const;

      virtual void derivatives (double x, double y, Scalar& dx, Scalar& dy) const;

      virtual Ord ord(double x, double y) const;
      virtual MeshFunction<Scalar>* clone() const;

      /// Saves the exact solution to an XML file.
      void save(const char* filename) const;
#ifdef WITH_BSON
      void save_bson(const char* filename) const;
#endif

    protected:
      Scalar constant;
    };

    /// @ingroup meshFunctions
    template<typename Scalar>
    class HERMES_API ZeroSolution : public ExactSolutionScalar<Scalar>
    {
    public:
      ZeroSolution(MeshSharedPtr mesh);
      virtual ~ZeroSolution() {};

      virtual Scalar value (double x, double y) const;

      virtual void derivatives (double x, double y, Scalar& dx, Scalar& dy) const;

      virtual Ord ord(double x, double y) const;
      virtual MeshFunction<Scalar>* clone() const;
    };

    /// @ingroup meshFunctions
    template<typename Scalar>
    class HERMES_API ConstantSolutionVector : public ExactSolutionVector<Scalar>
    {
    public:
      ConstantSolutionVector(MeshSharedPtr mesh, Scalar constantX, Scalar constantY);
      virtual ~ConstantSolutionVector() {};

      virtual Scalar2<Scalar> value (double x, double y) const;

      virtual void derivatives (double x, double y, Scalar2<Scalar>& dx, Scalar2<Scalar>& dy) const;

      virtual Ord ord(double x, double y) const;
      virtual MeshFunction<Scalar>* clone() const;

      /// Saves the exact solution to an XML file.
      void save(const char* filename) const;
#ifdef WITH_BSON
      void save_bson(const char* filename) const;
#endif
    protected:
      Scalar constantX;
      Scalar constantY;
    };

    /// @ingroup meshFunctions
    template<typename Scalar>
    class HERMES_API ZeroSolutionVector : public ExactSolutionVector<Scalar>
    {
    public:
      ZeroSolutionVector(MeshSharedPtr mesh);
      virtual ~ZeroSolutionVector() {};

      virtual Scalar2<Scalar> value (double x, double y) const;

      virtual void derivatives (double x, double y, Scalar2<Scalar>& dx, Scalar2<Scalar>& dy) const;

      virtual Ord ord(double x, double y) const;
      virtual MeshFunction<Scalar>* clone() const;
    };

    /// @ingroup meshFunctions
    /// Eggshell function.
    class HERMES_API ExactSolutionEggShell : public ExactSolutionScalar<double>
    {
    public:
      /// \param[in] polynomialOrder The polynomial order used for the space where the solution of the
      /// internal Laplace equation is sought.
      ExactSolutionEggShell(MeshSharedPtr mesh, int polynomialOrder);
      virtual ~ExactSolutionEggShell() {};

      /// Function returning the value.
      virtual double value (double x, double y) const;

      /// Function returning the derivatives.
      virtual void derivatives (double x, double y, double& dx, double& dy) const;

      /// Function returning the integration order that
      /// should be used when integrating the function.
      virtual Hermes::Ord ord(double x, double y) const;

      MeshFunction<double>* clone() const;
    };

    /// @ingroup meshFunctions
    /// Function operating on previous nonlinear solutions in assembling (u_ext)
    template<typename Scalar>
    class HERMES_API UExtFunction : public MeshFunction<Scalar>
    {
    public:
      /// \param[in] polynomialOrder The polynomial order used for the space where the solution of the
      /// internal Laplace equation is sought.
      UExtFunction(MeshSharedPtr mesh);
      virtual ~UExtFunction() {};

      /// Function returning the value.
#ifndef H2D_USE_SECOND_DERIVATIVES
      virtual void value (Scalar* values, Scalar* dx, Scalar* dy, Scalar result[3]) const = 0;
      virtual void ord(Hermes::Ord* values, Hermes::Ord* dx, Hermes::Ord* dy, Hermes::Ord result[3]) const = 0;
#else
      virtual Scalar value (Scalar* values, Scalar* dx, Scalar* dy, Scalar* dxx, Scalar* dxy, Scalar* dyy, Scalar result[6]) const = 0;
      virtual Hermes::Ord ord(Hermes::Ord* values, Hermes::Ord* dx, Hermes::Ord* dy, Hermes::Ord* dxx, Hermes::Ord* dxy, Hermes::Ord* dyy, Hermes::Ord result[6]) const = 0;
#endif

      virtual Func<Scalar>* get_pt_value(double x, double y, bool use_MeshHashGrid = false, Element* e = nullptr)
      {
        throw Exceptions::Exception("UExtFunction is only usable in assembling, not for getting point values.");
        return nullptr;
      }

      virtual void precalculate(int order, int mask) {};

      MeshFunction<Scalar>* clone() const = 0;
    };
  }
}
#endif
