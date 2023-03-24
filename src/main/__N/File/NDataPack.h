#ifndef __NDATAPACK_H
#define __NDATAPACK_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NDataPack.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../NFlags.h"
#include "../Containers/NArray.h"

#include "NDataPackKey.h"
#include "NDataPackNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct NDATAPACK NDATAPACK;
typedef struct NDATAPACKHEADER NDATAPACKHEADER;
typedef struct NDATAPACKER NDATAPACKER;

typedef Nu32(*NDP_WRITE)(void* pelement, NDATAPACKER* pdatapacker, FILE* pfile);	// pointeur sur une fonction 'C' pour ecrire 'un élément du type de celui géré par le serializale referencant ce pointeur sur fonction
typedef Nu32(*NDP_SREAD)(void* pvoid, NDATAPACKER* pdatapacker, FILE* pfile);		// pointeur sur une fonction 'C' pour lire 'un élément du type de celui géré par le serializale referencant ce pointeur sur fonction

struct NDATAPACKHEADER
{
	Nu32	Key;						// Unique KeyCode
	Nu32	ElementSize;				// ... Taille en octet d'un objet ( doit bien sur etre egal à "m_pArrayOfObject->elementSize" si "m_pArrayOfObject" existe ). Erreur si = 0 !!!
	Nu32	ArrayOfPtrSize;				// ... de l'array de pointeurs, ( = nombre de pointeur written/read ) ! m_size = nombre d'objets aloués en une fois présents dans 'm_pArrayOfObject' + nombre d'objet isolé alloué 1 par 1
	Nu32	ArrayOfDataPackNodesSize;
};

#define FLAG_NDATAPACK_LOCKED							BIT_0

struct NDATAPACK
{
	Nu8						Flags;
	Nu8						AvailableNu8;
	Nu16					AvailableNu16;
	NDATAPACKHEADER			Header;					// Header content written and re-written in file at Headerposition
													// ... ! To avoid any useless data duplication ( in the header + in NSERIAL structure ),
													// ... "ElementSize" and "Declared" members are only in the header.
	Nu32					Processed;				// nombre de pointeurs sérializés traités ( written or read )
	NARRAY					ArrayOfPtr;				// Array de tous les pointeurs serializés de 'Key' : index du NLSERIAL dans m_serializableArray
													// cet array contient TOUS les pointeurs de type type_id y compris ceux des objets présents dans m_array !
	NARRAY					ArrayOfDataPackNodes;	// ArrayOf 'NSERIALNODE' struct. Each NSERIALNODE struct register:
													//								A Lonely Pointer to an object
													//								A pointer to an  Array/ MemBloc of Objects/Ptrs to Objects
	Nu32					HeaderPosition;			// SerialHeader position in file.


	NDP_WRITE				Write;					// function pointer to write each element managed by this serial.
	NDP_SREAD				SetupAndRead;			// function pointer to setup and read each element managed by this serial.
};




NDATAPACK*	NSetupDataPack(NDATAPACK* pdp, const Nu32 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread);
NDATAPACK*	NCreateDataPack(const Nu32 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread);
void		NClearDataPack(NDATAPACK* pdp);
void		NDeleteDataPack(NDATAPACK* pdp);

void		NInsertDataPackNode(NDATAPACK* pdp, const Nu32  cfg, const void* padress, const Nu32 membloc_object_count);
/*
#define NDeclareObject_UserPreAllocObj(pserial,ptr)													NDeclareSerialNode(pserial, USER_PREALLOC_OBJ, (void*)ptr, 0, NULL)
#define NDeclareObject_SerializerAllocObj(pserial,ptr)												NDeclareSerialNode(pserial, SERIAL_ALLOC_OBJ, (void*)ptr, 0, NULL)

#define NDeclareArrayOfObjects_UserPreAllocArrayAndObj(pserial,parray)								NDeclareSerialNode(pserial, USER_PREALLOC_ARRAY_AND_OBJ, (void*)parray, 0, NULL)
#define NDeclareArrayOfObjects_UserPreAllocArray_SerializerAllocObj(pserial,parray)					NDeclareSerialNode(pserial, USER_PREALLOC_ARRAY__SERIAL_ALLOC_OBJ, (void*)parray, 0, NULL)
#define NDeclareArrayOfObjects_SerializerAllocArrayAndObj(pserial,parray)							NDeclareSerialNode(pserial, SERIAL_ALLOC_ARRAY_AND_OBJ, (void*)parray, 0, NULL)

#define NDeclareMemBlocOfObjects_UserPreAllocMemBlocAndObj(pserial,pmbloc,objcount)					NDeclareSerialNode(pserial, USER_PREALLOC_MBLOC_AND_OBJ, (void*)pmbloc,objcount, NULL)
#define NDeclareMemBlocOfObjects_UserPreAllocMemBloc_SerializerAllocObj(pserial,pmbloc,objcount)	NDeclareSerialNode(pserial, USER_PREALLOC_MBLOC__SERIAL_ALLOC_OBJ, (void*)pmbloc,objcount, NULL)
#define NDeclareMemBlocOfObjects_SerializerAllocMemBlocAndObj(pserial,pmbloc,objcount)				NDeclareSerialNode(pserial, SERIAL_ALLOC_MBLOC_AND_OBJ, (void*)pmbloc,objcount, NULL)

#define NDeclareArrayOfPointers_UserPreAllocArrayAndPtr(pserial,parray)								NDeclareSerialNode(pserial, USER_PREALLOC_ARRAY_AND_PTR, (void*)parray, 0, NULL)
#define NDeclareArrayOfPointers_UserPreAllocArray_SerialzerAllocPtr(pserial,parray)					NDeclareSerialNode(pserial, USER_PREALLOC_ARRAY__SERIAL_ALLOC_PTR, (void*)parray, 0, NULL)
#define NDeclareArrayOfPointers_SerializerAllocArrayAndPtr(pserial,parray)							NDeclareSerialNode(pserial, SERIAL_ALLOC_ARRAY_AND_PTR, (void*)parray, 0, NULL)

#define NDeclareMemBlocOfPointers_UserPreAllocMemBlocAndPtr(pserial,pmbloc,objcount)				NDeclareSerialNode(pserial, USER_PREALLOC_MBLOC_AND_PTR, (void*)pmbloc,objcount, NULL)
#define NDeclareMemBlocOfPointers_UserPreAllocMemBloc_SerialzerAllocPtr(pserial,pmbloc,objcount)	NDeclareSerialNode(pserial, USER_PREALLOC_MBLOC__SERIAL_ALLOC_PTR, (void*)pmbloc,objcount, NULL)
#define NDeclareMemBlocOfPointers_SerializerAllocMemBlocAndPtr(pserial,pmbloc,objcount)				NDeclareSerialNode(pserial, SERIAL_ALLOC_MBLOC_AND_PTR, (void*)pmbloc,objcount, NULL)
*/
// 


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NSERIAL_H

