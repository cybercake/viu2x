/* Copyright (C) Hao Qin. All rights reserved. */

#include "Transformation.h"
#include "Exceptions.h"

namespace v2x {

	Transformation2D::Param::~Param() {

	}

	///////////////////////////////////
	// Transformation2D::ParamOffset //
	///////////////////////////////////

	Transformation2D::ParamOffset::ParamOffset() {}
	Transformation2D::ParamOffset::ParamOffset(const Transformation2D::ParamOffset & param) : m_offset(param.m_offset) {}
	Transformation2D::ParamOffset::ParamOffset(const Vector2D & offset) : m_offset(offset) {}
	Transformation2D::ParamOffset::~ParamOffset() {}

	Matrix<3, 3> Transformation2D::ParamOffset::getTransformationMatrix() const {
		Matrix<3, 3> result;
		result[0][0] = 1;
		result[1][1] = 1;
		result[2][2] = 1;

		result[0][2] = m_offset.x();
		result[1][2] = m_offset.y();

		return result;
	}

	Vector2D Transformation2D::ParamOffset::transform(const Vector2D & v) const { return v + m_offset; }

	Transformation2D::Param::Shared Transformation2D::ParamOffset::mutiply(Transformation2D::Param::ConstShared t) const {

		if (nullptr == t)
			return Transformation2D::Param::Shared(new Transformation2D::ParamGeneral(getTransformationMatrix()));

		Transformation2D::ParamOffset::ConstShared test = std::dynamic_pointer_cast<const Transformation2D::ParamOffset>(t);
		if (nullptr != test)
			return Transformation2D::Param::Shared(new Transformation2D::ParamOffset(m_offset + test->m_offset));

		return Transformation2D::Param::Shared(new Transformation2D::ParamGeneral(getTransformationMatrix() * t->getTransformationMatrix()));
	}

	Transformation2D::Param::Shared Transformation2D::ParamOffset::clone() const {
		return Transformation2D::Param::Shared(new Transformation2D::ParamOffset(m_offset));
	}

	////////////////////////////////////
	// Transformation2D::ParamGeneral //
	////////////////////////////////////

	Transformation2D::ParamGeneral::ParamGeneral() {}
	Transformation2D::ParamGeneral::ParamGeneral(const Transformation2D::ParamGeneral & param) : m_matrix(param.m_matrix) {}
	Transformation2D::ParamGeneral::ParamGeneral(const Matrix<3, 3> & matrix) : m_matrix(matrix) {}
	Transformation2D::ParamGeneral::~ParamGeneral() {}

	Matrix<3, 3> Transformation2D::ParamGeneral::getTransformationMatrix() const {
		return m_matrix;
	}

	Vector2D Transformation2D::ParamGeneral::transform(const Vector2D & v) const {

		Vector2D result(
			v.x() * m_matrix[0][0] + v.x() * m_matrix[0][1] + m_matrix[0][2],
			v.x() * m_matrix[1][0] + v.y() * m_matrix[1][1] + m_matrix[1][2]);
		double k = m_matrix[2][0] + m_matrix[2][1] + m_matrix[2][2];
		if (k != 1)
			return result / k;
		else return result;
	}

	Transformation2D::Param::Shared Transformation2D::ParamGeneral::mutiply(Transformation2D::Param::ConstShared t) const {
		Matrix<3, 3> m = m_matrix * t->getTransformationMatrix();
		return Transformation2D::Param::Shared(new Transformation2D::ParamGeneral(m)); // tbd!
	}

	Transformation2D::Param::Shared Transformation2D::ParamGeneral::clone() const {
		return Transformation2D::Param::Shared(new Transformation2D::ParamGeneral(m_matrix));
	}

	//////////////////////
	// Transformation2D //
	//////////////////////

	Transformation2D::Transformation2D() {}
	Transformation2D::Transformation2D(const Transformation2D & source) {
		if (source.m_param != nullptr)
			m_param = source.m_param->clone();
	}
	Transformation2D::Transformation2D(Transformation2D::Param::ConstShared param) : m_param(param) {}
	Transformation2D::~Transformation2D() {}

	Transformation2D Transformation2D::fromOffset(const Vector2D & offset) {
		Transformation2D result;
		result.m_param.reset(new ParamOffset(offset));
		return result;
	}

	Matrix<3, 3> Transformation2D::getTransformationMatrix() const {
		if (m_param == nullptr)
		{
			Matrix<3, 3> result;
			result[0][0] = 1;
			result[1][1] = 1;
			result[2][2] = 1;
			return result;
		}
		else return m_param->getTransformationMatrix();
	}

	Vector2D Transformation2D::transform(const Vector2D & v) const {
		if (m_param == nullptr)
			return v;
		else return m_param->transform(v);
	}

	Transformation2D Transformation2D::multiply(const Transformation2D & t) const {
		if (m_param == nullptr)
			return t;
		else return Transformation2D(m_param->mutiply(t.m_param));
	}

	bool Transformation2D::hasTranslationOnly() const {
		if (m_param == nullptr)
			return true;

		ParamOffset::ConstShared test = std::dynamic_pointer_cast<const ParamOffset>(m_param);
		return test != nullptr;
	}
}