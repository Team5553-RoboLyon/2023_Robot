#ifdef _NANDROID // it must be _NANDROID but it's _NWINDOWS for a while to dev
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NAndroid.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
// File used into Android Studio Only
// Way for NEngine to post (from C side) tasks that are going to be handle by Android (on JAVA Side).
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "./NAndroid.h"
#include "lib/N/NErrorHandling.h"

#include <jni.h>

//
extern JNIEnv *MainJNIEnv;
extern jobject JavaObj;

void NBuildJNICall(const char *fnct_name)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "()V");
		if (m)
		{
			MainJNIEnv->CallVoidMethod(JavaObj, m);
		}
	}
}

void NBuildJNICall_Ns32(const char *fnct_name, const Ns32 param_Ns32)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(I)V");
		if (m)
		{
			MainJNIEnv->CallVoidMethod(JavaObj, m, (jint)param_Ns32);
		}
	}
}

void NBuildJNICall_Nf32(const char *fnct_name, const Nf32 param_Nf32)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(F)V");
		if (m)
		{
			MainJNIEnv->CallVoidMethod(JavaObj, m, (jfloat)param_Nf32);
		}
	}
}

void NBuildJNICall_4Nf32(const char *fnct_name, const Nf32 parama_Nf32, const Nf32 paramb_Nf32, const Nf32 paramc_Nf32, const Nf32 paramd_Nf32)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(FFFF)V");
		if (m)
		{
			MainJNIEnv->CallVoidMethod(JavaObj, m, (jfloat)parama_Nf32, (jfloat)paramb_Nf32, (jfloat)paramc_Nf32, (jfloat)paramd_Nf32);
		}
	}
}

void NBuildJNICall_String(const char *fnct_name, const Nchar *param_str)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(Ljava/lang/String;)V");
		if (m)
		{
			jstring jstr = MainJNIEnv->NewStringUTF(param_str);
			MainJNIEnv->CallVoidMethod(JavaObj, m, jstr);
		}
	}
}

void NBuildJNICall_String_Nf32(const char *fnct_name, const Nchar *param_str, const Nf32 param_Nf32)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(Ljava/lang/String;F)V");
		if (m)
		{
			jstring jstr = MainJNIEnv->NewStringUTF(param_str);
			MainJNIEnv->CallVoidMethod(JavaObj, m, jstr, (jfloat)param_Nf32);
		}
	}
}

void NBuildJNICall_2String(const char *fnct_name, const Nchar *parama_str, const Nchar *paramb_str)
{
	NErrorIf(!MainJNIEnv, NERROR_SYSTEM_CHECK);
	jclass c = NULL;
	jmethodID m = NULL;
	c = MainJNIEnv->GetObjectClass(JavaObj);
	if (c)
	{
		m = MainJNIEnv->GetMethodID(c, fnct_name, "(Ljava/lang/String;Ljava/lang/String;)V");
		if (m)
		{
			jstring a_str = MainJNIEnv->NewStringUTF(parama_str);
			jstring b_str = MainJNIEnv->NewStringUTF(paramb_str);
			MainJNIEnv->CallVoidMethod(JavaObj, m, a_str, b_str);
		}
	}
}

#endif //  _NANDROID