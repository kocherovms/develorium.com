/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <stdexcept>
#include "manager.h"

Manager::Manager() {
	qmlRegisterType<Portal>("com.develorium.Portal", 1, 0, "Portal");
	master_.reset(new QQuickView(QUrl("qrc:/master.qml")));
	slave_.reset(new QQuickView(QUrl("qrc:/slave.qml")));

	if(false || 
	   !QObject::connect(master_.data(), SIGNAL(openglContextCreated(QOpenGLContext *)), 
			     this, SLOT(onMasterOpenGlContextCreated(QOpenGLContext *)), Qt::DirectConnection) ||
	   !QObject::connect(master_.data(), SIGNAL(afterRendering()), 
			     this, SLOT(onMasterAfterRendering()), Qt::DirectConnection) ||
	   !QObject::connect(this, SIGNAL(newTextureReady(int)), 
			     this, SLOT(onNewTextureReady(int)), Qt::QueuedConnection) ||
	   !QObject::connect(slave_.data(), SIGNAL(openglContextCreated(QOpenGLContext *)), 
			     this, SLOT(onSlaveOpenGlContextCreated(QOpenGLContext *)), Qt::DirectConnection) ||
	   false)
		throw std::runtime_error("Failed to connect objects");

	master_->setResizeMode(QQuickView::SizeRootObjectToView);
	master_->setTitle("Master");
	master_->show();
}

void Manager::onMasterOpenGlContextCreated(QOpenGLContext *) {
	slave_->setTitle("Slave");
	// Show slave window after master window in order to make OpenGL context's creation predictable
	slave_->show();
}

void Manager::onSlaveOpenGlContextCreated(QOpenGLContext * theContext) {
	QOpenGLContext * masterContext = master_->openglContext();
	Q_ASSERT(masterContext);
	theContext->setShareContext(masterContext);
	theContext->create();
}

void Manager::onMasterAfterRendering() {
	// Locate free (non-busy) texture where to upload data
	const auto textureIt = std::find_if(std::begin(textures_), std::end(textures_), [](const Portal::TexturePtr & theTexture) { 
		return !theTexture || !theTexture->isBusy; 
	});

	if(textureIt == std::end(textures_))
		return; // no free textures so far
	
	const int textureIndex = std::distance(std::begin(textures_), textureIt);
	Portal::TexturePtr & t = textures_[textureIndex];
	// Create or recreate texture if needed
	if(!t || !t->id || t->size != master_->size()) {
		t.reset(new Portal::Texture); // will call glDeleteTextures as well
		t->size = master_->size();
		glGenTextures(1, &t->id);

		if(t->id) {
			glBindTexture(GL_TEXTURE_2D, t->id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->size.width(), t->size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			// failed to allocate texture
			t.reset();
			return;
		}
	}
	// Upload current framebuffer's content to texture
	Q_ASSERT(t);
	t->isBusy = 1;
	glBindTexture(GL_TEXTURE_2D, t->id);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, t->size.width(), t->size.height());
	glBindTexture(GL_TEXTURE_2D, 0);
	emit newTextureReady(textureIndex);
}

void Manager::onNewTextureReady(const int theIndex) {
	Q_ASSERT(slave_);
	Portal * portal = slave_->rootObject()->findChild<Portal *>("portal");
	
	if(portal) {
		const Portal::TexturePtr & t = textures_[theIndex];
		Q_ASSERT(t->isBusy);
		portal->setTexture(t);
	}
}
