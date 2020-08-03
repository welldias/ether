#ifndef __ETHER_RENDER_H__
#define __ETHER_RENDER_H__

namespace ether {
	
	class Render {

	public:
		enum class Mode {
			Point = 0,
			Line, 
			Fill,
		};

		Render();
		void Init();
		void Execute(const Vao& vao);

		void SetMode(Render::Mode mode);
		Render::Mode GetMode();

	public:
		Vector3 BackColor;
		Mode mode;
	};
}

#endif // __ETHER_RENDER_H__


