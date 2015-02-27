#pragma once


class IMAGE{
public:
	IMAGE();
	IMAGE(const IMAGE&);
	IMAGE(const IMAGE&, int x, int y, unsigned w, unsigned h);
	IMAGE(const void*, unsigned w, unsigned h, unsigned bytesDepth);
	IMAGE(unsigned w, unsigned h, unsigned bytesDepth);

	static IMAGE* New(const char*); //ファイル読み込み

	virtual ~IMAGE();

	void FlipVertical();
	void FlipHorizonal();
	void Rotate90();
	void Rotate180();
	void Rotate270();
	void Update(const IMAGE&, int x, int y); // overwrite image
//TODO 	void Draw(const IMAGE&, int x, int y); //draw image

	void operator=(const IMAGE&);

	unsigned Pixels() const{ return width * height; };
	unsigned Size() const{ return Pixels() * depth; };
	unsigned Width() const{ return width; };
	unsigned Height() const{ return height; };
	unsigned Depth() const{ return depth; };
	const void* Buffer() const{ return buffer ? buffer : constBuffer; };
	void* WritableBuffer() const{ return buffer; };
protected:
	void AssignBuffer(void*);
	void AssignBuffer(unsigned w, unsigned h, unsigned d);
	bool IsInRange(unsigned x, unsigned y) const;
	unsigned GetOffset(unsigned x, unsigned y) const;
	void* GetPoint(unsigned, unsigned);
	const void* GetConstPoint(unsigned, unsigned) const;
private:
	void* buffer;
	const void* constBuffer;
	unsigned width;
	unsigned height;
	unsigned depth;

	struct B3{ unsigned char data[3]; };
	struct B4{ unsigned char data[4]; };
	template<typename T> void FH(T*);

	class Pixel{
	public:
		Pixel() : p(0), i(0){};
		Pixel(void* p, IMAGE& i) : p(p), i(&i){};
		Pixel operator[](int);
		void operator++(){ 	p = (void*)((char*)p + (* i).depth); }
		void operator--(){ p = (void*)((char*)p - (* i).depth); };
		void operator=(unsigned);
		operator unsigned() const;
	private:
		Pixel(IMAGE& i, void* p, unsigned o) :
			p((void*)((char*)p + o * i.depth)),
			i(&i){};

		void* p;
		IMAGE* const i;
	};

	Pixel operator[](int); //get leftside of line

};
