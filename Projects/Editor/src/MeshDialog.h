#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <AntTweakBar.h>
#include "Scene.h"

class MeshDialog
{
private:
	TwBar* m_dialog;

	int m_meshIndex;
private:
	static void TW_CALL SetWireframe(const void *value, void *clientData);
	static void TW_CALL GetWireframe(void *value, void *clientData);
	static void TW_CALL SetDrawNormals(const void *value, void *clientData);
	static void TW_CALL GetDrawNormals(void *value, void *clientData);
	static void TW_CALL SetName(const void *value, void *clientData);
	static void TW_CALL GetName(void *value, void *clientData);

public:
	MeshDialog();
	~MeshDialog();
	void show();
	void hide();
	void setMesh(int pIndex);
};

#endif