#ifndef __ETHER_RENDER_H__
#define __ETHER_RENDER_H__

namespace ether {
	
	class Render {

	public:
		void Prepare();
		void Execute(const Vao& vao);

	public:
		Vector3 BackColor;
	};
}

#endif // __ETHER_RENDER_H__


