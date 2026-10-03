#include "backend.h"

#include "ca.h"

void runCA(Lattice *lat, const char *rules, int nLats, int latsPerRule)
{
  cpuRunCA(lat,rules,nLats,latsPerRule);
}
