// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceLocation.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#ifdef _NANDROID
#include "lib/N/System/NAndroid.h"
#endif

#include "./NServiceGeographicLocation.h"

void NStartGeographicLocation()
{
#ifdef _NANDROID
	NBuildJNICall("StartLocation");
#endif
}
void NStopGeographicLocation()
{
#ifdef _NANDROID
	NBuildJNICall("StopLocation");
#endif
}
/*
Check with Christophe to define the best way in N philosophy ( trough NEVENT like TOUCH )
void NztDecl AndroidGetLocation ( double *latitude, double *longitude )
{
	*latitude = LocationValues[0];
	*longitude = LocationValues[1];
}
double NztDecl AndroidGetLatitude ( void )
{
	return LocationValues[0];
}
double NztDecl AndroidGetLongitude ( void )
{
	return LocationValues[1];
}
*/
