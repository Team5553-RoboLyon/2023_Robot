#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NOTES:
// + User is going to use NUT_BindNf32Builder_xxx , which is actually a Setup 
// + ...of a decorated form of NUT_Nf32_BUILDER.
// + So function NUT_SetupNf32Builder, NUT_CreateNf32Builder, NUT_DeleteNf32Builder don't exist.
// + For the same reasons, 
// + NUT_SetupNf32BuilderCore, NUT_CreateNf32BuilderCore, NUT_DeleteNf32BuilderCore don't exist.
// + ...
// + To stay Cristal clear, implementation of 'NUT_SetupNf32Builder' and 'NUT_SetupNf32BuilderCore' is made but in comments.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
NUT_Nf32_BUILDER_CORE* NUT_SetupNf32BuilderCore(NUT_Nf32_BUILDER_CORE *pfbuildercore, const Nu8 nf32set_size)
{
	Nmem0(pfbuildercore,NUT_Nf32_BUILDER_CORE);
	pfbuildercore->Flags			= 0;
	pfbuildercore->Nf32SetSize		= nf32set_size;
	pfbuildercore->UpdateCount		= 0;
	pfbuildercore->Start			= NUT_Nf32Builder_NULL_Start;
	pfbuildercore->End				= NUT_Nf32Builder_NULL_End;
	pfbuildercore->Update			= NUT_Nf32Builder_NULL_Update;
	pfbuildercore->Clear			= NUT_Nf32Builder_NULL_Clear;
	pfbuildercore->Cache			= 0.0f;

	return pfbuildercore;
}
*/
/*
NUT_Nf32_BUILDER* NUT_SetupNf32Builder(NUT_Nf32_BUILDER *pfbuilder, const Nu8 nf32set_size)
{
	memset((NBYTE*)pfbuilder+sizeof(NUT_Nf32_BUILDER_CORE), 0, sizeof(NUT_Nf32_BUILDER) - sizeof(NUT_Nf32_BUILDER_CORE) );
	return (NUT_Nf32_BUILDER*)NUT_SetupNf32BuilderCore((NUT_Nf32_BUILDER_CORE*)pfbuilder, nf32set_size);
}
*/
void NUT_ClearNf32ProcessCore(NUT_Nf32_PROCESS_CORE *pfcore)
{
	Nmem0(pfcore,NUT_Nf32_PROCESS_CORE);
}

void NUT_ClearNf32Process(NUT_Nf32_PROCESS *pfproc)
{
	pfproc->Core.Clear(pfproc);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + The Default Nf32Builder ... NULL
// + 
// + Never used, so in comments for a while ... TO BE DELETED
/*
Nf32 NUT_Nf32Builder_NULL_Update(NUT_Nf32_BUILDER *pfbuilder)
{
	return pfbuilder->Core.Cache = 0.0f;
}

void NUT_Nf32Builder_NULL_Start(NUT_Nf32_BUILDER *pfbuilder,const Nu32 sequence_size)
{
	pfbuilder->Core.Cache	= 0.0f;
}

void NUT_Nf32Builder_NULL_End(NUT_Nf32_BUILDER *pfbuilder)
{
// nothing;	
}

void NUT_Nf32Builder_NULL_Clear(NUT_Nf32_BUILDER *pfbuilder)
{
	NUT_ClearNf32BuilderCore((NUT_Nf32_BUILDER_CORE*)pfbuilder);
}
*/
