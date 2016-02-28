#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <toolbox/image/image.h>

#include <toolbox/image/jpeg.h>
#include <toolbox/image/png.h>
#include <toolbox/image/tga.h>



//TODO:不要リンクを防ぐためこのメソッドは別ファイルにする
IMAGE* IMAGE::New(const char* path){
	//ファイルを開いて...
	return New(open(path, O_RDONLY));
}

IMAGE* IMAGE::New(int fd){
	if(fd < 0){ return 0; }

	//内容チェック、読み込み
	static IMAGE* (* const news[])(int) = {
		&PNG::New,
		&TGA::New,
		&JPEG::New,
		0,
	};
	for(IMAGE* (* const *i)(int) = news; i; i++){
		//Newで途中になっている可能性もあるので頭出ししとく
		lseek(fd, 0, SEEK_SET);

		//正常そうならfdを読んでIMAGE*が返ってくる
		if(IMAGE* image = (**i)(fd)){
			close(fd);
			return image;
		}
	}
	close(fd);
	return 0;
}


IMAGE::IMAGE() :
	buffer(0),
	constBuffer(0),
	width(0),
	height(0),
	depth(0){}

IMAGE::IMAGE(const IMAGE& org) :
	buffer(malloc(org.Size())),
	constBuffer(0),
	width(org.Width()),
	height(org.Height()),
	depth(org.Depth()){
	memcpy(buffer, org.buffer ? org.buffer : org.constBuffer, Size());
}

IMAGE::IMAGE(const IMAGE& org, int x, int y, unsigned w, unsigned h) :
	buffer(malloc(w * h * org.Depth())),
	constBuffer(0),
	width(w),
	height(h),
	depth(org.Depth()){

	//TODO:もうちょっとマシなアルゴリズムに書き換える
	for(unsigned yd(0); yd < h; ++yd){
		for(unsigned xd(0); xd < w; ++xd){
			void* const d(GetPoint(xd, yd));
			const void* const s(org.GetConstPoint(x + xd, y + yd));
			if(d){
				if(s){
					memcpy(d, s, depth);
				}else{
					memset(d, 0, depth);
				}
			}
		}
	}
}

IMAGE::IMAGE(const void* org, unsigned w, unsigned h, unsigned d) :
	buffer(0),
	constBuffer(org),
	width(w),
	height(h),
	depth(d){}

IMAGE::IMAGE(unsigned w, unsigned h, unsigned d) :
	buffer(malloc(w * h * d)),
	constBuffer(0),
	width(w),
	height(h),
	depth(d){}

IMAGE::~IMAGE(){
	if(buffer){
		free(buffer);
	}
}

void IMAGE::operator=(const IMAGE& org){
	if(buffer){
		free(buffer);
	}
	constBuffer = 0;
	const unsigned s(org.Size());
	buffer = malloc(s);
	width = org.width;
	height = org.height;
	depth = org.depth;
	memcpy(buffer, org.buffer ? org.buffer : org.constBuffer, s);
}


void IMAGE::AssignBuffer(void* newBuffer){
	if(buffer){
		free(buffer);
	}
	buffer = newBuffer;
}

void IMAGE::AssignBuffer(unsigned w, unsigned h, unsigned d){
	if(buffer){
		free(buffer);
	}
	buffer = malloc(w * h * d);
	width = w;
	height = h;
	depth = d;
}


IMAGE::Pixel IMAGE::operator[](int y){
	if(!buffer || height <= (unsigned)y){
		return Pixel();
	}
	return Pixel((void*)(((char*)buffer) + y * width * depth), *this);
}
IMAGE::Pixel IMAGE::Pixel::operator[](int x){
	if(!p || (*i).width <= (unsigned)x){
		return Pixel();
	}
	return Pixel(*i, p, (unsigned)x);
}
void IMAGE::Pixel::operator=(unsigned color){
	if(!i){ return; }
	const unsigned unmask(~0U << ((*i).depth * 8));
	unsigned& target(*((unsigned*)p));
	target = (target & unmask) | (color & ~unmask);
}
IMAGE::Pixel::operator unsigned() const{
	if(!i){ return 0; }
	const unsigned unmask(~0U << ((*i).depth * 8));
	unsigned& target(*((unsigned*)p));
	return target & ~unmask;
}



bool IMAGE::IsInRange(unsigned x, unsigned y) const{
	return x < width && y < height;
}
unsigned IMAGE::GetOffset(unsigned x, unsigned y) const{
	return (y * width + x) * depth;
}
void* IMAGE::GetPoint(unsigned x, unsigned y){
	if(!IsInRange(x, y)){ return 0; }
	return (void*)((char*)buffer + GetOffset(x, y));
}
const void* IMAGE::GetConstPoint(unsigned x, unsigned y) const{
	if(!IsInRange(x, y)){ return 0; }
	return (const void*)((const char*)Buffer() + GetOffset(x, y));
}


void IMAGE::Update(const IMAGE& org, int x, int y){
	if(!buffer){
		throw "IMAGE:constなIMAGEをUpdateしようとした。";
	}


	//TODO:もっとマシなアルゴリズムに書き換える
	for(unsigned yy(0); yy < org.height; ++yy){
		for(unsigned xx(0); xx < org.width; ++xx){
			void* const d(GetPoint(xx + x, yy + y));
			const void* const s(org.GetConstPoint(xx, yy));
			if(d){
				if(s){
					memcpy(d, s, depth);
				}else{
					memset(d, 0, depth);
				}
			}
		}
	}
}


void IMAGE::FlipVertical(){
	if(!buffer){ return; }

	void* t(malloc(width * depth));
	if(!t){
		return;
	}

	const unsigned lineSize(width * depth);

	for(unsigned v(0); v < height / 2; ++v){
		void* const s(&((char*)buffer)[lineSize * v]);
		void* const d(&((char*)buffer)[lineSize * (height - v - 1)]);
		memcpy(t, s, lineSize);
		memcpy(s, d, lineSize);
		memcpy(d, t, lineSize);
	}
	free(t);
}

void IMAGE::FlipHorizontal(){
	if(!buffer){ return; }

	for(unsigned y(0); y < height; ++y){
		Pixel s((*this)[y]);
		Pixel d((*this)[y][width - 1]);
		for(unsigned x(0); x < width / 2; ++x, ++s, --d){
			const unsigned t(s);
			s = (unsigned)d;
			d = t;
		}
	}
}

void IMAGE::Rotate90(){
	if(!buffer){ return; }

	if(height == 1){
		height = width;
		width = 1;
		return;
	}
	IMAGE newImage(height, width, depth);
	for(unsigned y(0); y < height; ++y){
		Pixel s((*this)[y]);
		for(unsigned x(0); x < width; ++x, ++s){
			newImage[x][height - y - 1] = (unsigned)s;
		}
	}
	free(buffer);
	buffer = newImage.buffer;
	width = newImage.width;
	height = newImage.height;
	newImage.buffer = 0;
}

void IMAGE::Rotate180(){
	if(!buffer){ return; }

	for(unsigned y(0); y < (height + 1) / 2; ++y){
		Pixel p((*this)[y]);
		Pixel r((*this)[height - y - 1][width - 1]);
		const unsigned w(y != height - y ? width : width / 2);
		for(unsigned x(0); x < w; ++x, ++p, --r){
			const unsigned t(p);
			p =(unsigned) r;
			r = t;
		}
	}
}

void IMAGE::Rotate270(){
	if(!buffer){ return; }

	if(width == 1){
		width = height;
		height = 1;
		return;
	}
	IMAGE newImage(height, width, depth);
	for(unsigned y(0); y < height; ++y){
		Pixel s((*this)[y]);
		for(unsigned x(width - 1); x < width; --x, ++s){
			newImage[x][y] = (unsigned)s;
		}
	}
	free(buffer);
	buffer = newImage.buffer;
	width = newImage.width;
	height = newImage.height;
	newImage.buffer = 0;
}




