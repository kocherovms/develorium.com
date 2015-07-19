/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <QMutexLocker>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include "portal.h"

/* Portal::Texture */

Portal::Texture::~Texture() {
	if(id) glDeleteTextures(1, &id);
}

/* Portal::TextureNode */

class Portal::TextureNode : public QObject, public QSGSimpleTextureNode {
	Q_OBJECT
public:
	TextureNode(Portal & thePortal): portal_(thePortal), window_(portal_.window()) {
		sgTexture_.reset(window_->createTextureFromId(0, QSize(1, 1)));
		setTexture(sgTexture_.data());
		setFiltering(QSGTexture::Linear);
	}
public slots:
	void prepareNode(const QRectF & theGeometry) {
		QMutexLocker locker(&portal_.lock_);
		QStack<TexturePtr> pendingTextures;
		pendingTextures.swap(portal_.pendingTextures_);
		locker.unlock();

		if(pendingTextures.isEmpty())
			return;

		TexturePtr veryLastTexture = pendingTextures.pop();
		
		if(occupiedTexture_ != veryLastTexture) {
			if(occupiedTexture_) 
				pendingTextures.push(occupiedTexture_); // set this texture for releasing

			occupiedTexture_ = veryLastTexture;
		}

		const uint occupiedTextureId = occupiedTexture_ ? occupiedTexture_->id : 0;
		// Construct a scene graph node
		if(sgTexture_ && static_cast<uint>(sgTexture_->textureId()) != occupiedTextureId) 
			sgTexture_.reset();

		if(!sgTexture_) {
			if(occupiedTextureId) {
				Q_ASSERT(occupiedTexture_);
				sgTexture_.reset(window_->createTextureFromId(occupiedTextureId, occupiedTexture_->size));
				setTexture(sgTexture_.data());
				setRect(theGeometry);
				setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
			} else {
				sgTexture_.reset(window_->createTextureFromId(0, QSize(1, 1)));
				setTexture(sgTexture_.data());
			}
		}
		// Release all uneeded textures, i.e. mark them as ready for uploading an image's data
		std::for_each(pendingTextures.begin(), pendingTextures.end(), [](TexturePtr & theTexture) {
			theTexture->isBusy = 0;
		});
	}
private:
	Portal & portal_;
	QQuickWindow * window_ = 0;
	TexturePtr occupiedTexture_;
	QScopedPointer<QSGTexture> sgTexture_;
};

/* Portal */

Portal::Portal() {
	setFlag(ItemHasContents, true);
}

Portal::~Portal() {
}

void Portal::setTexture(const TexturePtr & theTexture) {
	if(!theTexture->isBusy)
		throw std::runtime_error("Texture is not marked as busy");

	QMutexLocker locker(&lock_);
	pendingTextures_.push(theTexture);
	locker.unlock();
	update();
}

QSGNode * Portal::updatePaintNode(QSGNode * theOldNode, UpdatePaintNodeData *) {
	TextureNode * node = dynamic_cast<TextureNode *>(theOldNode);

	if(!node)
		node = new TextureNode(*this);

	node->prepareNode(boundingRect());
	return node;
}

#include "portal.moc"
