#pragma once


extern "C"{
	struct dirent;
}

namespace TB{

	class Directory{
		Directory(const Directory&);
		void operator=(const Directory&);
	public:
		class ITOR{
			ITOR();
			ITOR(const ITOR&);
			void operator=(const ITOR&);
		public:
			ITOR(Directory&);
			ITOR& operator++();
			bool IsEnd(){ return index < 0 ||  dir.numOfEntries <= index; };
			operator bool(){ return !IsEnd(); };
			const char* Name();
			bool IsDir();
			bool IsReg();
		private:
			int index;
			Directory& dir;
		};

		Directory(const char* path="./");
		~Directory();
	private:
		dirent** entries;
		int numOfEntries;

		static int DefaultFilter(const dirent*);
	};

}
