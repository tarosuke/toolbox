#pragma once

#include <GL/glew.h>
#include <GL/gl.h>


namespace GL{

	class Matrix{
	public:
		Matrix(){};
		Matrix(const float*);

		float* GetBody(){ return raw; };

		void Identity();
		void Transpose(const float[4][4]);
		void TransposeAffine(const float[3][4]);

		void Invert();
		void InvertAffine(){ Invert(); };

	private:
		union{
			float raw[16];
			float m[4][4];
		};
	};

}
