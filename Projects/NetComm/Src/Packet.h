// =======================================================================================
//                                      Packet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # Packet
/// Detailed description...
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <vector>

#include <AglVector3.h>
#include <AglQuaternion.h>
#include <AglMatrix.h>
#include <Windows.h>
#include <string>

using namespace std;

class Packet
{
public:
	Packet( const Packet& p_other );
	///-----------------------------------------------------------------------------------
	/// Creates a new packet from raw data.
	/// Sets the raw data of the packet. It is possible to stream out so much data
	///	that equals to the byte size p_size - 1, since one byte is allocated to
	///	the packet size itself.
	/// \param p_senderId
	/// \param p_data
	/// \param p_size
	/// \return 
	///-----------------------------------------------------------------------------------
	Packet( int p_senderId, char* p_data, unsigned int p_size );

	///-----------------------------------------------------------------------------------
	/// Creates a packet with its sender id.
	/// \param p_senderId
	/// \return 
	///-----------------------------------------------------------------------------------
	Packet( char p_packetType );


	~Packet();

	/// Gets a pointer to the raw data that is generated by the packet content.
	char* getDataPtr();

	/// Returns the size of the raw data. The size is actually 1 byte more than
	/// the total sum of whatever is streamed in to the packet.
	/// For example, you stream in 3x integers (3*4 = 12 bytes) and the
	/// getDataSize() method will return 13.
	unsigned int getDataSize() const;

	bool isEmpty() const;

	///-----------------------------------------------------------------------------------
	/// Clears the data array and sets it to size 1. The first position of the array is
	/// reserved for the length of the array.
	/// Used when packing and unpacking the data array.
	/// \return void
	///-----------------------------------------------------------------------------------
	void clear();

	///-----------------------------------------------------------------------------------
	/// Returns the id of the sender.
	/// \return int
	///-----------------------------------------------------------------------------------
	int getSenderId() const;


	///-----------------------------------------------------------------------------------
	/// Returns the packet type, it's the overall packet type and not be confused with 
	/// EntityType
	/// \return char
	///-----------------------------------------------------------------------------------
	char getPacketType() const;

	unsigned int getUniquePacketIdentifier() const;
	void setUniquePacketIdentifier( unsigned int p_uniquePacketIdentifier );

	///-----------------------------------------------------------------------------------
	/// Sets the identity of the sender.
	/// \param p_senderId
	/// \return void
	///-----------------------------------------------------------------------------------
	void setSenderId( int p_senderId );

	Packet& operator << (const bool& p_data);
	Packet& operator << (const char& p_data);
	Packet& operator << (const unsigned char& p_data);
	Packet& operator << (const short& p_data);
	Packet& operator << (const unsigned short& p_data);
	Packet& operator << (const int& p_data);
	Packet& operator << (const unsigned int& p_data);
	Packet& operator << (const float& p_data);
	Packet& operator << (const double& p_data);
	Packet& operator << (const AglVector3& p_data);
	Packet& operator << (const AglVector4& p_data);
	Packet& operator << (const AglMatrix& p_data);
	Packet& operator << (const AglQuaternion& p_data);
	Packet& operator << (const SYSTEMTIME& p_data);
	Packet& operator << (const string& p_data);
	
	Packet& operator >> (bool& p_data);
	Packet& operator >> (char& p_data);
	Packet& operator >> (unsigned char& p_date);
	Packet& operator >> (short& p_data);
	Packet& operator >> (unsigned short& p_data);
	Packet& operator >> (int& p_data);
	Packet& operator >> (unsigned int& p_data);
	Packet& operator >> (float& p_data);
	Packet& operator >> (double& p_data);
	Packet& operator >> (AglMatrix& p_data);
	Packet& operator >> (AglVector3& p_data);
	Packet& operator >> (AglVector4& p_data);
	Packet& operator >> (AglQuaternion& p_data);
	Packet& operator >> (SYSTEMTIME& p_data);
	Packet& operator >> (string& p_data);

	///-----------------------------------------------------------------------------------
	/// Provide a way to set the raw byte data from the outside. Needed for testing!
	/// \param p_data
	/// \param p_size
	/// \return void
	///-----------------------------------------------------------------------------------
	void setDataTest(char* p_data, unsigned int p_size);

	void WriteData(void* p_data, unsigned int p_dataSize);
	void ReadData(void* p_data, unsigned int p_dataSize);

private:

	/// Sets the raw data of the packet. It is possible to stream out so much data
	/// that equals to the byte size p_size - 1, since one byte is allocated to
	/// the packet size itself.
	void setData(char* p_data, unsigned int p_size);

	///-----------------------------------------------------------------------------------
	/// Writes the header variables to the beginning of the data array.
	/// \param p_data
	/// \param p_headerSize
	/// \return void
	///-----------------------------------------------------------------------------------
	void writeHeaderData(char* p_data);

	///-----------------------------------------------------------------------------------
	/// Reads from the beginning of the data array to the header variables.
	/// \param p_data
	/// \param p_headerSize
	/// \return void
	///-----------------------------------------------------------------------------------
	void readHeaderData(char* p_data);

private:
	int m_readPos;
	vector<char> m_data;
	unsigned int m_dataSize;

	// Variable that is set on a received packet and identifies its sender.
	int m_senderId;

	// Header data (stored in the byte buffer).
	unsigned char m_packetSize;
	char m_packetType;
	unsigned int m_uniquePacketIdentifier;

public:
	// HEADER_SIZE must be equal to the byte size of the header variables combined.
	static const int HEADER_SIZE = 6;
	static const unsigned int PACKET_BUFFER_SIZE = 256;

};