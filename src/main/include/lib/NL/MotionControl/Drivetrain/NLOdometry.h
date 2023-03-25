#pragma once
#include "../../../N/NType.h"

// https://fr.wikipedia.org/wiki/Odom%C3%A9trie
// https://mouniere.blogspot.com/2007/10/cours-et-mthode-leon-n5-la-conduite-dun.html

// w			= rotation angle	/ vitesse angulaire
// v			= translation		/ vitesse
// axletrack	= dist. entre les points de contact au sol des roues droite et gauche.

#define NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(left,right)					( ( (right)+(left) )/2.0f )
#define NLODOMETRY_DRIVETRAIN_W_FROM_WHEELS(left,right,axletrack)		( ( (right)-(left) )/(axletrack) )
#define NLODOMETRY_L_WHEEL_FROM_DRIVETRAIN(w,v,axletrack)				( (v) - ( (w) * (axletrack) )/2.0f )		
#define NLODOMETRY_R_WHEEL_FROM_DRIVETRAIN(w,v,axletrack)				( (v) + ( (w) * (axletrack) )/2.0f )
