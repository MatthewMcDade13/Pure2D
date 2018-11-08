#ifndef PURE2D_SHADER_H
#define PURE2D_SHADER_H

#include <cinttypes>
#include <cstddef>
#include <Pure2D/Define.h>

#if 0

namespace pure
{
	template<typename T> struct Vec4;
	template<typename T> struct Vec3;
	struct Mat4;

	struct PURE2D_API Shader
	{
		uint32_t id_;

		int modelMatLoc_;
		int mvpMatLoc_;

		static Shader create(const char* vertShaderPath, const char* fragShaderPath);
		static Shader createSrc(const char* vertSrc, const char* fragSrc);
		static void createFragShader(char* outBuffer, const char *effectSrc);
		static void createVertShader(char* outBuffer, const char* positionSrc, bool isInstanced);
		static Shader createDefault(bool isInstanced = false);
		static void createDefaultFragShader(char* outBuffer);
		static void createDefaultVertShader(char* outBuffer, bool isInstanced);

		static size_t getFragShaderSize(size_t inBufferCount);
		static size_t getVertShaderSize(size_t inBufferCount);
		static size_t getDefaultFragShaderSize();
		static size_t getDefaultVertShaderSize(bool isInstanced);

		void bind() const;

		int getLocation(const char *uniform) const;

		void setUniform(int location, const Vec4<float> &vec) const;
		void setUniform(int location, const Vec3<float> &vec) const;
		void setUniform(int location, const Mat4 &matrix, bool transpose = false) const;
		void setUniform(int location, float val) const;
		void setUniform(int location, int val) const;

		void setUniform(const char* uniform, const Vec4<float>& vec) const;
		void setUniform(const char* uniform, const Vec3<float>& vec) const;
		void setUniform(const char* uniform, const Mat4& matrix, bool transpose = false) const;
		void setUniform(const char* uniform, float val) const;
		void setUniform(const char* uniform, int val) const;

		void free();
	};
}
#else
#include <cinttypes>
#include <vector>
#include <cstddef>
#include <Pure2D/Define.h>

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

		static Shader create(const char* vertShaderPath, const char* fragShaderPath);
		static Shader createSrc(const char* vertSrc, const char* fragSrc);
		static void createFragShader(char* outBuffer, const char *effectSrc);
		static void createVertShader(char* outBuffer, const char* positionSrc, bool isInstanced);
		static Shader createDefault(bool isInstanced = false);
		static void createDefaultFragShader(char* outBuffer);
		static void createDefaultVertShader(char* outBuffer, bool isInstanced);

		static size_t getFragShaderSize(size_t inBufferCount);
		static size_t getVertShaderSize(size_t inBufferCount);
		static size_t getDefaultFragShaderSize();
		static size_t getDefaultVertShaderSize(bool isInstanced);

		void bind() const;

		int getLocation(const char *uniform) const;

		void setUniformIndx(int index, const Vec4<float> &vec) const;
		void setUniformIndx(int index, const Vec3<float> &vec) const;
		void setUniformIndx(int index, const Mat4 &matrix, bool transpose = false) const;
		void setUniformIndx(int index, float val) const;
		void setUniformIndx(int index, int val) const;

		void setUniform(int location, const Vec4<float> &vec) const;
		void setUniform(int location, const Vec3<float> &vec) const;
		void setUniform(int location, const Mat4 &matrix, bool transpose = false) const;
		void setUniform(int location, float val) const;
		void setUniform(int location, int val) const;

		void setUniform(const char* uniform, const Vec4<float>& vec) const;
		void setUniform(const char* uniform, const Vec3<float>& vec) const;
		void setUniform(const char* uniform, const Mat4& matrix, bool transpose = false) const;
		void setUniform(const char* uniform, float val) const;
		void setUniform(const char* uniform, int val) const;

		void free();

	private:
		uint32_t m_id;
	};
}

#endif

#endif // PURE2D_SHADER_H
