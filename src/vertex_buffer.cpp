#include "ether.h"

namespace ether {

	void IVertexBuffer::ReleaseBuffer() {
		glDeleteBuffers(1, &id);
		id = 0;
	}

	void IVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}

	void IVertexBuffer::UnBind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	
	
	//void VertexBuffer::BindElementArrayByffer() {
	//	glGenBuffers(1, &id);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size(), data, GL_STATIC_DRAW);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	//void VertexBuffer::EnableAttributeArray() const {
	//	if (id == 0)
	//		return;
	//	if (this->type == VertexBuffer::Type::Indices)
	//		return;
	//	glEnableVertexAttribArray(attributeNumber);
	//
	//	if (this->type == VertexBuffer::Type::Texture) {
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, id);
	//	}
	//}
	//
	//void VertexBuffer::DisableAttributeArray() const {
	//	if (id == 0)
	//		return;
	//	if (this->type == VertexBuffer::Type::Indices)
	//		return;
	//	glDisableVertexAttribArray(attributeNumber);
	//}
}
