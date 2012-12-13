// =======================================================================================
//                                      Transform
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Transform Component
///        
/// # Transform
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <AglMatrix.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <Component.h>

#include <InstanceVertex.h>

//struct xXxVector3 // HACK: DESTROY THIS, and replace with a real one!
//{
//	float x;
//	float y;
//	float z;
//};

class Transform: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Sets all private members to 0 (zero).
	/// \returns 
	///-----------------------------------------------------------------------------------
	Transform();

	///-----------------------------------------------------------------------------------
	/// Sets the position. Remaining members are set to 0 (zero).
	/// \param p_posX
	/// \param p_posY
	/// \param p_posZ
	/// \returns 
	///-----------------------------------------------------------------------------------
	Transform( float p_posX, float p_posY, float p_posZ );
	
	///-----------------------------------------------------------------------------------
	/// creates a transform with position, scale and rotation
	/// \param p_translation
	/// \param p_rotation
	/// \param p_scale
	/// \return 
	///-----------------------------------------------------------------------------------
	Transform(AglVector3 p_translation, AglQuaternion p_rotation, AglVector3 p_scale);

	~Transform();

	///-----------------------------------------------------------------------------------
	/// \returns The translation vector.
	///-----------------------------------------------------------------------------------
	AglVector3 getTranslation() const;
	
	///-----------------------------------------------------------------------------------
	/// \param p_translation The new translation as a vector
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setTranslation( const AglVector3 p_translation );

	///-----------------------------------------------------------------------------------
	/// \returns The scale vector.
	///-----------------------------------------------------------------------------------
	AglVector3 getScale() const;

	///-----------------------------------------------------------------------------------
	/// \param p_scale The new scale as a vector
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setScale( const AglVector3 p_scale );

	///-----------------------------------------------------------------------------------
	/// \returns The rotation quaternion.
	///-----------------------------------------------------------------------------------
	AglQuaternion getRotation() const;

	///-----------------------------------------------------------------------------------
	/// \param p_rotation The new rotation as a quaternion
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setRotation( const AglQuaternion p_rotation );

	///-----------------------------------------------------------------------------------
	/// Getter that fetches the pre-calculated matrix that is the sum of all vectors 
	/// \returns A pointer to the transform matrix.
	///----------------------------------------------------------------------------------
	AglMatrix getMatrix() const;

	///-----------------------------------------------------------------------------------
	/// Get the translated transform matrix packaged in a InstanceVertex.
	/// \returns The translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceVertex getInstanceVertex() const;

	///-----------------------------------------------------------------------------------
	/// Get a reference to the translated transform matrix packaged in a InstanceVertex.
	/// If the data is manipulated it will change the internal InstanceVertex. This will 
	/// be overwritten the next time a setter is called.
	/// \returns A reference to the translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceVertex& getInstanceVertexRef() ;

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the translated transform matrix packaged in a InstanceVertex.
	/// If the data is manipulated it will change the internal InstanceVertex. This will 
	/// be overwritten the next time a setter is called.
	/// \returns A pointer to the translated transform matrix as a InstanceVertex.
	///----------------------------------------------------------------------------------
	InstanceVertex* getInstanceVertexPtr() ;

private:
	///-----------------------------------------------------------------------------------
	/// Calculates the composited matrix for all translation members
	/// \returns void
	///-----------------------------------------------------------------------------------
	void calcCompMatrix();

private:
	AglMatrix m_compositionMatrix;
	InstanceVertex m_instanceVertex;
	//AglMatrix m_transposedCompositionMatrix;
	AglVector3 m_translation;
	AglVector3 m_scale;
	AglQuaternion m_rotation;
};