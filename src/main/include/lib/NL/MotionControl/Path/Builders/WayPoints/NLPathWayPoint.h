#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"

typedef struct
{
#define DEFAULT_NLPATH_WAYPOINT_RR 1.0f
#define DEFAULT_NLPATH_WAYPOINT_ICR_FROM 0.0f
#define DEFAULT_NLPATH_WAYPOINT_ICR_TO 0.0f

	NVEC2f32 p;
	NVEC2f32 u;

	// Param
	Nf32 rr;	// RadiusRatio: "0.0f < radiusRatio <= 1"  , permet de moduler la taille des "4 cercles de base" utilisés pour constuire les sections.
				// Le rayon des 4 cercles d'une section est modulé par le radiusRatio du 2eme Waypoint de la section ( psection->pwaypoint_to ).
	Nf32 icr_A; // InsertedCircleRatio of part A of the section "0.0f <= in <= 1.0", coefficient permettant d'insérer un arc de cercle entre les les deux clothoides symétriques à sommet.
				// utilisé par le premier cercle de la section dont ce Waypoint est le pwayPoint_from.
	Nf32 icr_C; // InsertedCircleRatio of part C of the section "0.0f <= in <= 1.0", coefficient permettant d'insérer un arc de cercle entre les les deux clothoides symétriques à sommet.
				// utilisé par le premier cercle de la section dont ce Waypoint est le pwayPoint_to.
} NLPATH_WAYPOINT;
