#pragma once


namespace TB{

	template<typename T> class Median{
	public:
		Median() : index(0){};
		Median(T v) : index(0){
			buffer[0] = buffer[1] = buffer[2] = v;
		};
		T operator=(T v){
			//格納
			index %= 3;
			buffer[index++] = v;

			//選択
			if(buffer[0] < buffer[1]){
				if(buffer[2] <= buffer[0]){ //cab
					return buffer[0];
				}
				if(buffer[1] <= buffer[2]){ //abc
					return buffer[1];
				}
				//acb
				return buffer[2];
			}
			//buffer[1] <= buffer[0]
			if(buffer[2] <= buffer[1]){ //cba
				return buffer[1];
			}
			if(buffer[0] <= buffer[2]){ //bac
				return buffer[0];
			}
			//bca
			return buffer[2];
		};
		void Reset(T v){
			buffer[0] = buffer[1] = buffer[2] = v;
		};
	private:
		unsigned index;
		T buffer[3];
	};


}
