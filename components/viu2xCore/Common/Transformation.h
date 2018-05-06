/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Object.h"
#include "Matrix.hpp"

namespace v2x {

	// A container of all kinds of 2D transformation parameters.
	//
	// The idea is to keep the parameter simple as long as there is only 
	// translations. In this case there will be no multiplication while 
	// transforming.
	class Transformation2D
	{
	public:
		Transformation2D();
		Transformation2D(const Transformation2D & source);
		virtual ~Transformation2D();

		static Transformation2D fromOffset(const Vector2D & offset);

		Matrix<3, 3> getTransformationMatrix() const;
		Vector2D transform(const Vector2D & v) const;
		Transformation2D multiply(const Transformation2D & t) const;

		bool hasTranslationOnly() const;

	protected:

		// A virtual definition of all kinds of 2D transformation parameters.
		class Param {
		public:
			DEFINE_POINTERS(Param);

			virtual ~Param();

			virtual Matrix<3, 3> getTransformationMatrix() const = 0;
			virtual Vector2D transform(const Vector2D & v) const = 0;
			virtual Param::Shared mutiply(Param::ConstShared t) const = 0;

			virtual Param::Shared clone() const = 0;
		};

		// 2D transformation parameter for offset.
		class ParamOffset : public Param {
		public:
			DEFINE_POINTERS(ParamOffset);

			ParamOffset();
			ParamOffset(const ParamOffset & param);
			ParamOffset(const Vector2D & offset);
			virtual ~ParamOffset();

			Matrix<3, 3> getTransformationMatrix() const;
			Vector2D transform(const Vector2D & v) const;
			Param::Shared mutiply(Param::ConstShared t) const;

			Param::Shared clone() const;

		private:
			Vector2D m_offset;
		};

		// A virtual definition of all kinds of 2D transformation parameters.
		class ParamGeneral : public Param {
		public:
			DEFINE_POINTERS(ParamGeneral);

			ParamGeneral();
			ParamGeneral(const ParamGeneral & param);
			ParamGeneral(const Matrix<3, 3> & matrix);
			virtual ~ParamGeneral();

			Matrix<3, 3> getTransformationMatrix() const;
			Vector2D transform(const Vector2D & v) const;
			Param::Shared mutiply(Param::ConstShared t) const;

			Param::Shared clone() const;

		private:
			Matrix<3, 3> m_matrix;
		};

		Transformation2D(Param::ConstShared param);

	private:
		Param::ConstShared m_param;
	};

}