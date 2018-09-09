#ifndef PURE3D_SHADER_H
#define PURE3D_SHADER_H

#include <cinttypes>
#include "Define.h"

namespace pure
{
	template<typename T> struct Vec4;
	template<typename T> struct Vec3;
	struct Mat4;

	struct PURE2D_API Shader
	{
		uint32_t id_;

		static Shader create(const char* vertShaderPath, const char* fragShaderPath);
		static Shader createSrc(const char* vertSrc, const char* fragSrc);
		void bind() const;

		uint32_t getLocation(const char* uniform) const;

		void setUniform(uint32_t location, const Vec4<float>& vec) const;
		void setUniform(uint32_t location, const Vec3<float>& vec) const;
		void setUniform(uint32_t location, const Mat4& matrix, bool transpose = false) const;
		void setUniform(uint32_t location, float val) const;
		void setUniform(uint32_t location, int val) const;

		void setUniform(const char* uniform, const Vec4<float>& vec) const;
		void setUniform(const char* uniform, const Vec3<float>& vec) const;
		void setUniform(const char* uniform, const Mat4& matrix, bool transpose = false) const;
		void setUniform(const char* uniform, float val) const;
		void setUniform(const char* uniform, int val) const;

		void free();
	};
}

#endif // PURE3D_SHADER_H
