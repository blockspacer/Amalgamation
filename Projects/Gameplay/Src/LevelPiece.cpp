#include "LevelPiece.h"

#include <ConnectionPointCollection.h>
#include "Transform.h"

LevelPiece::LevelPiece( ConnectionPointCollection* p_connectionPoints,
					   AglMeshHeader* p_meshHeader, 
					   Transform* p_transform )
{
	m_connectionPoints	= p_connectionPoints;
	m_transform			= p_transform;
	m_meshHeader		= p_meshHeader;

	int maxChildCount = m_connectionPoints->m_collection.size();
	m_childSlotsOccupied.resize(maxChildCount);
	m_children.resize(maxChildCount, nullptr);
}

vector<int> LevelPiece::findFreeConnectionPointSlots()
{
	vector<int> freeSlots;
	// Check for unoccupied slots
	for (int i = 0; i < m_children.size(); i++)
	{
		if ( !m_childSlotsOccupied[i] )
			freeSlots.push_back(i);
	}
	return freeSlots;
}

Transform* LevelPiece::getTransform()
{
	return m_transform;
}

int LevelPiece::getMeshId()
{
	return m_connectionPoints->m_meshId;
}

Transform* LevelPiece::getChild( int p_inSlot )
{
	return m_children[p_inSlot];
}

void LevelPiece::setChild( int p_inSlot, Transform* p_transform )
{
	m_children[p_inSlot]			= p_transform;
	m_childSlotsOccupied[p_inSlot]	= true;
}

void LevelPiece::connectTo( LevelPiece* p_targetPiece, int p_targetSlot )
{
	// From Proto \\ Jarl & Alex

	// Set this connection to be occupied!
	p_targetPiece->setChild(p_targetSlot, m_transform);
	setChild(0, p_targetPiece->getTransform());

	return;
	// From Proto \\ Anton & Alex

	// Set the position of this piece to the position of the target, with the offset of
	// the target connection point and this connection point
	// connectionPoints are assumed to be in local space relative its parent (piece).
	// Pieces are assumed to be in global space.

	AglMatrix thisLocalConnectMat = getConnectionPointMatrix(0);

	// Target connection point matrix in world space
	AglMatrix targetGlobalConnectMat = p_targetPiece->getConnectionPointMatrix(p_targetSlot, Space_GLOBAL);
	AglQuaternion rotation;
	AglVector3 scale, translation;
	targetGlobalConnectMat.toComponents(scale, rotation, translation);

	AglMatrix thisNewGlobalMat = targetGlobalConnectMat * 
									thisLocalConnectMat.inverse();

	// TODO: Fetch new code from sprint 4 that has AglMatrix::GetRotation()
	thisNewGlobalMat.toComponents(scale, rotation, translation);

	rotation *= AglQuaternion::constructFromAxisAndAngle(thisLocalConnectMat.GetRight(), 3.141592f);
	m_transform->setRotation(rotation);
	

	// Positioning
	// -----------
	// Get distance from this piece and its connection point. This can be found by taking
	// the length of the connection point's position since it is defined in local space.
	AglVector3 thisConnectionPointPosition = thisLocalConnectMat.GetTranslation();
	float length = thisConnectionPointPosition.length();
	AglVector3 globalTargetConnectionPointPosition = targetGlobalConnectMat.
													GetTranslation();

	// Move this piece using the forward direction of the target point connection
	m_transform->setTranslation( globalTargetConnectionPointPosition + 
								targetGlobalConnectMat.GetForward() * length);

	//m_transform->setTranslation( p_targetPiece->getTransform()->getTranslation() +
	//							targetConnectionPointMatrix.GetTranslation() +
	//							thisConnectionPointMatrix.GetTranslation() );

	// Set this connection to be occupied!
	p_targetPiece->setChild(p_targetSlot, m_transform);
	setChild(0, p_targetPiece->getTransform());
}

AglMatrix LevelPiece::getConnectionPointMatrix( int p_vectorIndex, E_Space p_inSpace/*=Space_LOCAL*/ )
{
	if (p_inSpace == Space_LOCAL)
		return AglMatrix( m_connectionPoints->m_collection[p_vectorIndex].transform );
	else
		return AglMatrix( m_connectionPoints->m_collection[p_vectorIndex].transform ) *
				m_transform->getMatrix();
}
