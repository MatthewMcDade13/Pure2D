#ifndef PURE2D_SHADER_H
#define PURE2D_SHADER_H

#include <cinttypes>
#include <vector>
#include <cstddef>
#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec2.h>

namespace pure
{
	template<typename T> struct Vec4;
	template<typename T> struct Vec3;
	struct Mat4;

	struct PURE2D_API Shader
	{
		// Use this as a lookup table to get uniform locations
		// Index 0 and 1 are reserved for model mat and mvp mat locations respectively.
		// Start new additions at DEFAULT_LOC_COUNT to avoid conflicts
		std::vector<int> locations;
		inline uint32_t id() const { return m_id; }

		enum { MODEL_MAT_LOC = 0, MVP_MAT_LOC, DEFAULT_LOC_COUNT };

		Shader(uint32_t id = 0);

		// creates default shader.
		static Shader make(bool isInstanced = false);
		static Shader fromFile(const char* vertShaderPath, const char* fragShaderPath);
		static Shader fromSrc(const char* vertSrc, const char* fragSrc);
		static Shader fromTemplate(const char* positionSrc, const char* effectSrc, bool isInstanced = false);

		void bind() const;

		int storeLocation(const char* locationName);
		int getLocation(const char *uniform) const;

		void setUniformIndx(int index, const Vec4<float> &vec) const;
		void setUniformIndx(int index, const Vec3<float> &vec) const;
		void setUniformIndx(int index, Vec2f vec) const;
		void setUniformIndx(int index, const Mat4 &matrix, bool transpose = false) const;
		void setUniformIndx(int index, float val) const;
		void setUniformIndx(int index, int val) const;

		void setUniform(int location, const Vec4<float> &vec) const;
		void setUniform(int location, const Vec3<float> &vec) const;
		void setUniform(int location, Vec2f vec) const;
		void setUniform(int location, const Mat4 &matrix, bool transpose = false) const;
		void setUniform(int location, float val) const;
		void setUniform(int location, int val) const;

		void setUniform(const char* uniform, const Vec4<float>& vec) const;
		void setUniform(const char* uniform, const Vec3<float>& vec) const;
		void setUniform(const char* uniform, Vec2f vec) const;
		void setUniform(const char* uniform, const Mat4& matrix, bool transpose = false) const;
		void setUniform(const char* uniform, float val) const;
		void setUniform(const char* uniform, int val) const;

		void free();

	private:
		uint32_t m_id;
	};
}

#endif // PURE2D_SHADER_H
