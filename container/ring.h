/** リングバッファのFIFO
 * バッファに実体をコピーするタイプのFIFOなのでオブジェクトより基本型向き
 * NOTE:オブジェクトを格納するなら引数なしコンストラクタとコピーコンストラクタが必要
 *
 * テンプレート引数Tはバッファの型、Sは読み出し型
 * emptyはバッファが空の時に読み出される値、elementsはバッファの要素数
 * SはTを包摂する必要がある(T=unsigned char、S=intなど)。
 * NOTE:Tをsignedな型にするのは推奨しないが、するならevには区別できる値を設定すること
 * 例えばデータがcharならT=char、S=int、ev=0x8000000など。
 */
#pragma once

namespace wO{

	template<typename T, typename S, S ev = -1, unsigned elements=8> class Ring{
	public:
		Ring() : in(0), out(0){};

		static const S empty = ev;

		/** 読み出し
		 * 空の場合emptyで指定した値が返る
		 */
		operator T(){
			out %= elements;
			in %= elements;
			if(out != in){
				return buff[out++];
			}
			return empty;
		}

		/** 書き込み
		 * 偽が返ったらバッファオーバーフロー
		 */
		bool operator=(T v){
			out %= elements;
			in %= elements;
			const unsigned i((in + 1) % elements);
			if(i != out){
				buff[in] = v;
				in = i;
				return true;
			}
			return false;
		}

	private:
		T buff[elements];
		unsigned in;
		unsigned out;
	};

}
