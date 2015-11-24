/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "Vector2D.hpp"
#include "Vector3D.hpp"
#include "Object.h"
#include "Matrix.hpp"

namespace v2x {

	// A container of all kinds of 2D transformation parameters.
	class Transformation2D
	{
	protected:

		// A virtual definition of all kinds of 2D transformation parameters.
		class Param {
		public:
			DEFINE_POINTERS(Param);

			virtual ~Param();

			virtual Matrix getTransformationMatrix() const = 0;
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

			Matrix getTransformationMatrix() const;
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
			ParamGeneral(const Matrix & matrix);
			virtual ~ParamGeneral();

			Matrix getTransformationMatrix() const;
			Vector2D transform(const Vector2D & v) const;
			Param::Shared mutiply(Param::ConstShared t) const;

			Param::Shared clone() const;

		private:
			Matrix m_matrix;
		};

		Transformation2D(Param::ConstShared param);

	public:
		Transformation2D();
		Transformation2D(const Transformation2D & source);
		virtual ~Transformation2D();

		Matrix getTransformationMatrix() const;
		Vector2D operator * (const Vector2D & v) const;
		Transformation2D operator * (const Transformation2D & t) const;

	private:
		Param::ConstShared m_param;
	};

}