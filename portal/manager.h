/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QSize>
#include <QOpenGLContext>
#include <QScopedPointer>
#include <QQuickView>
#include "portal.h"

class Portal;
class QQuickView;

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
signals:
	void newTextureReady(int theIndex);
private slots:
	void onMasterOpenGlContextCreated(QOpenGLContext * theContext);
	void onSlaveOpenGlContextCreated(QOpenGLContext * theContext);
	void onMasterAfterRendering();
	void onNewTextureReady(int theIndex);
private:
	QScopedPointer<QQuickView> master_;
	QScopedPointer<QQuickView> slave_;
	enum { PortalTexturesCount = 4 };
	static_assert(PortalTexturesCount > 1, "Number of portal textures must be greater than 1, otherwise there will be no free texture ever");
	Portal::TexturePtr textures_[PortalTexturesCount];
};

#endif
