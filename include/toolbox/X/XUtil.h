/** X utilities
 */
#pragma once

#include <X11/Xlib.h>


namespace TB{


	/** サブウインドウの反復子
	 * 環状になっていて初期化時に無効要素を指しているJavaスタイル
	 * 順：for(XSubWindowJtor i(d,w); ++i;);
	 * 逆：for(XSubWindowJtor i(d,w); --i;);
	 */
	class XSubWindowJtor{
		XSubWindowJtor();
		XSubWindowJtor(const XSubWindowJtor&);
		void operator=(const XSubWindowJtor&);

	public:
		XSubWindowJtor(::Display* d, Window w) : enable(false){
			Window root, parent;
			if((enable = !!XQueryTree(d, w,&root, &parent, &list, &total))){
				i = total;
			}
		};
		~XSubWindowJtor(){
			if(enable && list){
				XFree(list);
			}
		};

		operator bool(){ return enable && i < total; };
		Window operator*(){ return i < total ? list[i] : 0; };

		/** increment/decrement
		 * NOTE:ポストインクリメント/デクリメントはない
		 */
		bool operator++(){
			i = total <= i ? 0 : i + 1;
			return enable && (i < total);

		};
		bool operator--(){
			i = total < i ? total : i - 1;
			return enable && (i < total);
		}

	private:
		bool enable;
		unsigned total;
		unsigned i;
		Window* list;
	};



}
