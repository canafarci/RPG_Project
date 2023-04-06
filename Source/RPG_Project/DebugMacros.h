#pragma once
#include "DrawDebugHelpers.h"
//MACROS
#define DRAW_SPHERE(Location, Radius) if (GetWorld()) DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, FColor::Cyan, false, 2.f);
#define DRAW_SPHERE_SINGLE(Location, Radius) if (GetWorld()) DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, FColor::Red, false, -1.f);
#define DRAW_LINE(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Magenta, true);
#define DRAW_LINE_SINGLE(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 20, FColor::Yellow, true);
#define DRAW_POINT_SINGLE(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 20, FColor::Blue, false, -1.f);
#define DRAW_VECTOR(Start, End) if (GetWorld()) \
{ \
DRAW_LINE(Start, End); \
DRAW_POINT(End); \
}