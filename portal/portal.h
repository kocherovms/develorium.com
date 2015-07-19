/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef PORTAL_H_
#define PORTAL_H_

#include <QStack>
#include <QMutex>
#include <QSharedPointer>
#include <QQuickItem>

class Portal: public QQuickItem {
public:
	struct Texture {
		~Texture();
		uint id = 0;
		QSize size;
		char isBusy = 0;
	};
	typedef QSharedPointer<Texture> TexturePtr;
public:
	Portal();
	~Portal();
public slots:
	// must be called from main thread
	void setTexture(const TexturePtr & theTexture);
protected:
	QSGNode * updatePaintNode(QSGNode *, UpdatePaintNodeData *);
private:
	QMutex lock_;
	QStack<TexturePtr> pendingTextures_;

	class TextureNode;
	friend class TextureNode;
};

#endif
