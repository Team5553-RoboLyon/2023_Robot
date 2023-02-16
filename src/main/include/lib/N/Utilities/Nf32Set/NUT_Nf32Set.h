#ifndef __NUT_NF32SETBUILDER_H
#define __NUT_NF32SETBUILDER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Nf32Set.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NSpline.h"
#include "../NUT_X.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
#define CONSTANT_NUT_Nf32SET_SIZEMAX			8
// OPERANDs (to be used with NUT_BindNf32Builder_Operand )
typedef enum
{
	NUT_Nf32SET_SET = 0,
	NUT_Nf32SET_ADD,
	NUT_Nf32SET_SCALE,
	NUT_Nf32SET_BLEND,
	NUT_Nf32SET_MIN,
	NUT_Nf32SET_MAX,

	// ------------------------
	NUT_Nf32SET_OPERAND_ENUM_SIZE
}NUT_Nf32SET_OPERAND_ENUM;

typedef union NUT_Nf32_PROCESS NUT_Nf32_PROCESS;
typedef Nu32 (*NUT_Nf32_PROCESS_BUFSIZE)(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size);
typedef void (*NUT_Nf32_PROCESS_UPDATE)(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size);
typedef void (*NUT_Nf32_PROCESS_CLEAR)(NUT_Nf32_PROCESS *pfproc);
typedef void (*NUT_Nf32_OPERAND)(Nf32 *pdst, const Nf32 src);
typedef void (*NUT_Nf32_OPERAND_CST)(Nf32 *pdst, const Nf32 src, const Nf32 cst);

typedef struct  
{
	Nf32						Factor;
	NUT_Nf32_OPERAND			Operand;	
	NUT_Nf32_OPERAND_CST		OperandCst;	
}NUT_Nf32_SCOPE;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NUT_Nf32_BUILDER
// +
// + Purpose NUT_Nf32_BUILDER is building 1 Nf32 thanks to a specific process
// + describe with 4 embed functions. (UPDATE,START,END,CLEAR).
// + Because the builder is suppose to be call several times, potentially to obtain the same value,
// + a Cache of one Nf32 is also present.
// +
#define FLAG_NUT_Nf32_PROCESS_USE_FACTOR			BIT_0		// OFF if all the constant tuple member are different than 1 at binding. Otherwise, ON.
typedef struct 
{
	Nu8							TargetId0;			//|
	Nu8							TargetId1;			//|
	Nu8							Flags;				//|
	Nu8							Nf32SetSize;		//|

	NUT_Nf32_PROCESS_BUFSIZE	Start;				// All Starts Call will done before all Updates. Main purpose is returning size of the buffer needed to work.
	NUT_Nf32_PROCESS_UPDATE		Update;
	NUT_Nf32_PROCESS_CLEAR		Clear;
	
	NUT_Nf32_SCOPE				*pScope;

	Nf32						*pSharedBuffer;
}NUT_Nf32_PROCESS_CORE;

typedef struct
{
	NUT_Nf32_PROCESS_CORE		Core;
	// Specific for Constant
	Nf32						Constant;
}NUT_Nf32_BUILDER_CONSTANT;

typedef struct
{
	NUT_Nf32_PROCESS_CORE		Core;
	// Specific for Constant
	Nf32						*pTuple;
}NUT_Nf32_BUILDER_CONSTANT_TUPLE;

typedef struct
{
	NUT_Nf32_PROCESS_CORE	Core;
	// Specific for Rand
	Nu32					KeyEvery;
	Nu32					k;
	Nf32					Inter;
}NUT_Nf32_BUILDER_RAND;
typedef struct
{
	NUT_Nf32_PROCESS_CORE	Core;
	// Specific for Copy
	Nu8						SrcId;
}NUT_Nf32_BUILDER_COPY;
typedef struct
{
	NUT_Nf32_PROCESS_CORE				Core;
	// Specific for Spline
	NUT_2DSPLINE_PERSISTENT_XRESULT		PersistentXResult;
}NUT_Nf32_BUILDER_SPLINE;

typedef struct
{
	NUT_Nf32_PROCESS_CORE		Core;
	// Specific for Lerp
	Nf32						A;
	Nf32						B;
}NUT_Nf32_BUILDER_LERP;

#define FLAG_NUT_Nf32_BUILDER_STAIRWAY_IS_STEP_NUMBER	BIT_0
typedef struct
{
	NUT_Nf32_PROCESS_CORE		Core;
	// Specific for Lerp
	Nf32						Start;
	Nf32						Step;
	Nu32						StepEvery_or_StepNumber;
	
	Nu32						Flags;
	Nu32						StepEvery;
}NUT_Nf32_BUILDER_STAIRWAY;

typedef struct
{
	NUT_Nf32_PROCESS_CORE		Core;
	// Specific for Sinusoid
	Nf32						StartAngle;
	Nf32						EndAngle;
	Nf32						SineShift;
	Nu32						Count;
	Nf32						CountMax;
}NUT_Nf32_BUILDER_SINUSOID;

typedef struct
{
	NUT_Nf32_PROCESS_CORE	Core;
	// Specific for SMOOTH
	Nu16					Range;
	Nu16					Pass;
}NUT_Nf32_FILTER_SMOOTH;

typedef struct
{
	NUT_Nf32_PROCESS_CORE	Core;
	// Specific for WARP
	Nf32					Min;
	Nf32					Max;
}NUT_Nf32_FILTER_WARP;

typedef struct
{
	NUT_Nf32_PROCESS_CORE	Core;
	// Specific for CLAMP
	Nf32					Min;
	Nf32					Max;
}NUT_Nf32_FILTER_CLAMP;


union NUT_Nf32_PROCESS
{
	NUT_Nf32_PROCESS_CORE			Core;
	// Builders
	NUT_Nf32_BUILDER_CONSTANT		Constant;
	NUT_Nf32_BUILDER_CONSTANT_TUPLE	Tuple;
	NUT_Nf32_BUILDER_COPY			Copy;
	NUT_Nf32_BUILDER_SINUSOID		FastSinusoid;
	NUT_Nf32_BUILDER_LERP			Lerp;
	NUT_Nf32_BUILDER_RAND			Rand;
	NUT_Nf32_BUILDER_SINUSOID		Sinusoid;
	NUT_Nf32_BUILDER_SPLINE			Spline;
	NUT_Nf32_BUILDER_STAIRWAY		Stairway;
	// Filters
	NUT_Nf32_FILTER_SMOOTH			Smooth;
	NUT_Nf32_FILTER_WARP			Warp;
	NUT_Nf32_FILTER_CLAMP			Clamp;
};


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NUT_Nf32SET_BUILDER
// +
// + Purpose NUT_Nf32SET_BUILDER is describe how building the range of all the Nf32 include into a Nf32Set.
// + 'range of all the Nf32 include into a Nf32Set...' doesn't mean all the Nf32 of the sequence.
// + Just all the Nf32 of the Nf32Set. 
// + Example with a sequence of 10 NCOLORS ( 4 Nf32 by NCOLOR )
// +	NCOLOR is a Nf32Set of 4 Nf32.
// +	So a sequence of 10 NCOLORS is a sequence of 10 Nf32Sets.
// +	Purpose of NUT_Nf32SET_BUILDER here, is describe how to build the 4 components of NCOLOR
// +	From that, function 'NUT_BuildNf32SetSequence' is going to apply repeatedly this process description to build the whole sequence. 
// +	User is going to 'Bind' simple Instructions to the NUT_Nf32SET_BUILDER to describe this building Process.
#define FLAG_NUT_Nf32SET_BUILDER_SCOPE_FACTOR_UPDATED				BIT_0
typedef struct 		
{			   
	Nu8							Flags;					//| 
	Nu8							Nf32SetSize;			//| Size of Nf32Set that the Nf32SetBuilder is suppose to work with.
													
	Nu8							ProcessListCapacity;//| Current Capacity of the Nf32Process List.
	Nu8							ProcessListSize;	//|	Current Size of the Nf32Process List.
	NUT_Nf32_PROCESS			*pProcess;			//| List of  Nf32Process binded with. ( Builder+Filter )
	NUT_Nf32_SCOPE				*pCurrentScope;
	NUT_Nf32_SCOPE				*pScopeStack;
	Nu32						ScopeStackSize;		//| No capacity ! realloc are going to be made each time size evolve
	Nf32						*pSharedBuffer;
}NUT_Nf32SET_BUILDER;

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							  Nf32Set Builder										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
// Principle is simple.
// User create a sequential description of processes
// And launch them by calling "NUT_BuildNf32SetSequence".
// Purpose of Nf32Set Builder is ... building set of Nf32 with a size (number of consecutive Nf32 values) is define at setup/creation of "NUT_Nf32SET_BUILDER"
//	Example:	"NUT_CreateNf32SetBuilder(4)" means create a builder to build a range of Nf32 sets. Each of them has a size of 4 Nf32.
//				We can say create a Nf32Set builder with 4 Channels.
// Then, to build processes, user has to "bind" things to each of these 4 channels.
// So, all the Binding function have this form:
//				NUT_BindNf32_xxx( targetid0,targetid1, ... )
//				[targetid0,targetid1] represents a range of channel index. 
//				NUT_BindNf32_xxx( 0,1, ... ) means Bind to channel 0 to 1 (included )
//				NUT_BindNf32_xxx( 0,0, ... ) means Bind to channel 0
//
// Description is sequential, and will be executed in the description order.
// The calculus follow this scheme
// 	
// [Channel X]  [Operand] [Builder] X [Factor]
//

NUT_Nf32SET_BUILDER*	NUT_SetupNf32SetBuilder( NUT_Nf32SET_BUILDER *pnf32setbuilder, const Nu8 fsetsize );
NUT_Nf32SET_BUILDER*	NUT_CreateNf32SetBuilder(const Nu8 setsize);
void					NUT_ClearNf32SetBuilder(NUT_Nf32SET_BUILDER *pnf32setbuilder);
void					NUT_DeleteNf32SetBuilder(NUT_Nf32SET_BUILDER *pnf32setbuilder);
NUT_Nf32SET_BUILDER*	NUT_ResetNf32SetBuilder( NUT_Nf32SET_BUILDER *pnf32setbuilder, const Nu8 fsetsize );

void					NUT_BindNf32_Factor(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const Nf32 constant  );
// inline void				NUT_BindNf32_FactorDefault(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr){NUT_BindNf32_Factor(targetid0,targetid1,pfsetbldr,1.0f);}
void					NUT_BindNf32_FactorTuple(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const Nf32 *pconstant_tuple  );
void					NUT_BindNf32_Operand(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const NUT_Nf32SET_OPERAND_ENUM operand  );
//inline void				NUT_BindNf32_OperandDefault(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr){NUT_BindNf32_Operand(targetid0,targetid1, pfsetbldr,NUT_Nf32SET_SET);}
void					NUT_BindNf32_OperandTuple(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const NUT_Nf32SET_OPERAND_ENUM *poperand_tuple  );

void					NUT_BindNf32_BuilderConstant(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 constant );
void					NUT_BindNf32_BuilderConstantTuple(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 *pconstant_tuple );
void					NUT_BindNf32_BuilderCopy(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu8 srcid);
void					NUT_BindNf32_BuilderLerp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 a, const Nf32 b);
void					NUT_BindNf32_BuilderRand( const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 randkey_every );
void					NUT_BindNf32_BuilderRandOAAT( const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 randkey_every );
void					NUT_BindNf32_BuilderSinusoid(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 rad_angle_start, const Nf32 rad_angle_end, const Nf32 sine_shifting );
void					NUT_BindNf32_BuilderFastSinusoid(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 angle_start, const Nu32 angle_end, const Nf32 sine_shifting );
void					NUT_BindNf32_BuilderSpline(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const NSPLINE *pspline);
void					NUT_BindNf32_BuilderStairway(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 start, const Nf32 step,const Nu32 stepevery_or_stepnumber, const Nbool is_step_number);

void					NUT_BindNf32_FilterClamp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 min, const Nf32 max );
void					NUT_BindNf32_FilterSmooth(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu16 range, const Nu16 pass);
void					NUT_BindNf32_FilterWarp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 min, const Nf32 max );

void					NUT_BuildNf32SetSequence(Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size, NUT_Nf32SET_BUILDER *psetbuilder );

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + LOWLEVEL
// +
void					NUT_ClearNf32ProcessCore(NUT_Nf32_PROCESS_CORE *pfcore);
void					NUT_ClearNf32Process(NUT_Nf32_PROCESS *pfproc);
NUT_Nf32_PROCESS*		NUT_GetNf32Process( const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pnf32setbldr, const Nbool usescope);
void					NUT_GetNf32OperandHandle(NUT_Nf32SET_OPERAND_ENUM operand_id, NUT_Nf32_OPERAND *poperand, NUT_Nf32_OPERAND_CST *poperand_cst);

#ifndef _DEBUG
#define					NUT_fPrintfNf32Set(a,b,c,d,e)
#endif
#ifdef _DEBUG
// To print in a file. To activate it remove comment (//) before '_DEBUG_fPrintfNf32SetFCT(a,b,c,d,e)'
#define					NUT_fPrintfNf32Set(a,b,c,d,e)	_DEBUG_fPrintfNf32SetFCT(a,b,c,d,e)
// Never use the function directly ... use the define just above ... ( in that way it will works only in debug mode )
void					_DEBUG_fPrintfNf32SetFCT(const Nchar *filename,const Nf32 *pfirstnf32set, const Nu8 nf32setsize,const Nu32 stridetonext, const Nu32 sequence_size );
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUT_NF32SETBUILDER_H

